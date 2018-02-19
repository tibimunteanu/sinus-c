#include "assets.h"

internal void AssetLruInsertAtFront(asset_manager *assets, asset_lru *assetLru)
{
    asset_lru *sentinel = assets->assetLruSentinel;
    assetLru->prev = sentinel;
    assetLru->next = sentinel->next;
    assetLru->prev->next = assetLru;
    assetLru->next->prev = assetLru;
}

internal void AssetLruRemove(asset_lru *assetLru)
{
    assetLru->prev->next = assetLru->next;
    assetLru->next->prev = assetLru->prev;
    assetLru->prev = assetLru->next = 0;
}

internal void AssetLruMoveToFront(asset_manager *assets, asset_lru *assetLru)
{
    AssetLruRemove(assetLru);
    AssetLruInsertAtFront(assets, assetLru);
}

internal void *AllocateAssetMemory(asset_manager *assets, u32 size, u32 newAssetIndex, asset_types assetType)
{
    void *result = GAAllocate(&assets->assetsAllocator, size);
    while(!result)
    {
        asset_lru *assetLru = assets->assetLruSentinel->prev;
        if(assetLru == assets->assetLruSentinel)
        {
            break;
        }
        AssetLruRemove(assetLru);
        asset_handle *asset = assets->assets + assetLru->assetIndex;
        Assert(asset->state == AssetState_Loaded);
        asset->state = AssetState_Unloaded;

        memory_block *block = 0;
        switch(asset->meta.typeId)
        {
            case AssetType_Texture:
            {
                block = GAFree(&assets->assetsAllocator, asset->texture.sprites);
                asset->texture.sprites = 0;
                asset->texture.content = 0;
            } break;

            case AssetType_Sound:
            {
                block = GAFree(&assets->assetsAllocator, asset->sound.content);
                asset->sound.content = 0;
            } break;

            case AssetType_Shader:
            {
                block = GAFree(&assets->assetsAllocator, asset->shader.vertexSource);
                asset->shader.vertexSource = 0;
                asset->shader.fragmentSource = 0;
            } break;

            InvalidDefaultCase;
        };
        result = GAAllocate(&assets->assetsAllocator, size, block);
    }
    if(result)
    {
        asset_lru *assetLru = PushStruct(assets->arena, asset_lru, {true, 4});
        assetLru->assetIndex = newAssetIndex;
        AssetLruInsertAtFront(assets, assetLru);
    }
    return result;
}

internal asset_handle *GetAsset(asset_manager *assets, char *fileName)
{
    asset_handle *result = 0;
    for(u32 assetIndex = 0; assetIndex < assets->assetCount; assetIndex++)
    {
        asset_handle *asset = assets->assets + assetIndex;
        if(StringsAreEqual(asset->meta.fileName, fileName))
        {
            result = asset;
            break;
        }
    }
    return result;
}

internal void LoadTexture(asset_manager *assets, u32 assetIndex)
{
    asset_handle *asset = assets->assets + assetIndex;
    if(asset->state == AssetState_Unloaded)
    {
        asset->state = AssetState_Queued;
        u32 spritesDataSize = asset->meta.texture.spritesLength * sizeof(sprite_meta);
        u32 dataSize = asset->meta.texture.width * asset->meta.texture.height * 4;
        void *textureContent = AllocateAssetMemory(assets, spritesDataSize + dataSize, assetIndex, AssetType_Texture);
        asset->texture.sprites = (sprite_meta *)textureContent;
        asset->texture.content = (u8 *)asset->texture.sprites + spritesDataSize;
        file_handle *fileHandle = &assets->files[asset->fileIndex].handle;
        platform.ReadDataFromFile(fileHandle, asset->meta.dataOffset, spritesDataSize, asset->texture.sprites);
        platform.ReadDataFromFile(fileHandle, asset->meta.dataOffset + spritesDataSize, dataSize, asset->texture.content);
        asset->state = AssetState_Loaded;
    }
}

internal void LoadTexture(asset_manager *assets, char* fileName)
{
    asset_handle *asset = GetAsset(assets, fileName);
    if(asset)
    {
        LoadTexture(assets, asset->assetIndex);
    }
}

internal void LoadSound(asset_manager *assets, u32 assetIndex)
{

}

internal void LoadSound(asset_manager *assets, char *fileName)
{
    asset_handle *asset = GetAsset(assets, fileName);
    if(asset)
    {
        LoadSound(assets, asset->assetIndex);
    }
}

internal void LoadShader(asset_manager *assets, u32 assetIndex)
{
    asset_handle *asset = assets->assets + assetIndex;
    if(asset->state == AssetState_Unloaded)
    {
        asset->state = AssetState_Queued;
        u32 vertexSourceLength = asset->meta.shader.vertexSourceLength;
        u32 fragmentSourceLength = asset->meta.shader.fragmentSourceLength;
        char *vertexSource = (char *)AllocateAssetMemory(assets, vertexSourceLength + fragmentSourceLength, assetIndex, AssetType_Shader);
        asset->shader.vertexSource = (char *)vertexSource;
        asset->shader.fragmentSource = (char *)asset->shader.vertexSource + vertexSourceLength;
        file_handle *fileHandle = &assets->files[asset->fileIndex].handle;
        platform.ReadDataFromFile(fileHandle, asset->meta.dataOffset, vertexSourceLength, asset->shader.vertexSource);
        platform.ReadDataFromFile(fileHandle, asset->meta.dataOffset + vertexSourceLength, fragmentSourceLength, asset->shader.fragmentSource);
        asset->state = AssetState_Loaded;
    }
}

internal void LoadShader(asset_manager *assets, char *fileName)
{
    asset_handle *asset = GetAsset(assets, fileName);
    if(asset)
    {
        LoadShader(assets, asset->assetIndex);
    }
}

internal asset_manager *AssetManagerInit(memory_arena *arena, u32 size)
{
    asset_manager *assets = PushStruct(arena, asset_manager);
    GAInit(arena, &assets->assetsAllocator, size, 4096); //TODO: set this based on smallest asset?
    assets->arena = arena;
    assets->assetLruSentinel = PushStruct(arena, asset_lru);
    assets->assetLruSentinel->prev = assets->assetLruSentinel;
    assets->assetLruSentinel->next = assets->assetLruSentinel;
    assets->assetCount = 0;

    file_group fileGroup = platform.GetAllFilesOfTypeBegin(FileType_Asset);
    assets->fileCount = fileGroup.fileCount;
    assets->files = PushArray(arena, assets->fileCount, asset_file);
    for(u32 i = 0; i < assets->fileCount; i++)
    {
        asset_file *file = assets->files + i;
        file->handle = platform.OpenNextFile(&fileGroup);

        ZeroStruct(file->header);
        platform.ReadDataFromFile(&file->handle, 0, sizeof(file->header), &file->header);

        u32 assetTypeArraySize = file->header.assetTypeCount * sizeof(asset_type);
        file->assetTypeArray = (asset_type *)PushSize(arena, assetTypeArraySize);
        platform.ReadDataFromFile(&file->handle, file->header.assetTypeArrayOffset, assetTypeArraySize, file->assetTypeArray);
        
        u32 assetArraySize = file->header.assetCount * sizeof(asset_meta);
        file->assetArray = (asset_meta *)PushSize(arena, assetArraySize);
        platform.ReadDataFromFile(&file->handle, file->header.assetArrayOffset, assetArraySize, file->assetArray);

        if(file->header.magicValue != ASSET_FILE_MAGIC_VALUE)
        {
            platform.AddFileError(&file->handle, "The asset file has an invalid magic value.");
        }
        if(file->header.version > ASSET_FILE_VERSION)
        {
            platform.AddFileError(&file->handle, "The asset file is of a later version.");
        }
        if(file->handle.noErrors)
        {
            assets->assetCount += file->header.assetCount;
        }
        else
        {
            InvalidCodePath;
        }
    }
    platform.GetAllFilesOfTypeEnd(&fileGroup);
    assets->assets = PushArray(arena, assets->assetCount, asset_handle);

    u32 assetCount = 0;
    for(u32 destAssetTypeId = 0; destAssetTypeId < AssetType_Count; destAssetTypeId++)
    {
        asset_type *destAssetType = assets->assetTypes + destAssetTypeId;
        destAssetType->typeId = destAssetTypeId;
        destAssetType->firstAssetIndex = assetCount;
        destAssetType->assetCount = 0;

        for(u32 fileIndex = 0; fileIndex < assets->fileCount; fileIndex++)
        {
            asset_file *file = assets->files + fileIndex;
            if(file->handle.noErrors)
            {
                for(u32 sourceTypeIndex = 0; sourceTypeIndex < file->header.assetTypeCount; sourceTypeIndex++)
                {
                    asset_type *sourceAssetType = file->assetTypeArray + sourceTypeIndex;
                    if(sourceAssetType->typeId == destAssetTypeId)
                    {
                        for(u32 assetIndex = 0; assetIndex < sourceAssetType->assetCount; assetIndex++)
                        {
                            Assert(assetCount < assets->assetCount);
                            asset_handle *asset = assets->assets + assetCount++;
                            asset->assetIndex = destAssetType->firstAssetIndex + destAssetType->assetCount++;
                            asset->fileIndex = fileIndex;
                            asset->state = AssetState_Unloaded;
                            asset->meta = *(file->assetArray + sourceAssetType->firstAssetIndex + assetIndex);
                        }
                    }
                }
            }
        }
    }
    Assert(assetCount == assets->assetCount);
    return assets;
}

