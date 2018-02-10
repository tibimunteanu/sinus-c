#include<windows.h>
#include<stdio.h>
#include "../shared/shared.h"
#include "../shared/utils.h"
#include "../shared/math.h"
#include "../shared/memory.h"
#include "../game/platform.h"
#include "../game/assets.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) Assert(x)
#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_HDR
#define STBI_ONLY_PNG
#include "stbimage.h"

internal file ReadFile(char *fileName)
{
    file result = {};
    FILE *in = fopen(fileName, "rb");
    if(in)
    {
        fseek(in, 0, SEEK_END);
        result.size = ftell(in);
        fseek(in, 0, SEEK_SET);
        result.content = malloc(result.size);
        fread(result.content, result.size, 1, in);
        fclose(in);
    }
    else
    {
        printf("ERROR: Cannot open file %s.\n", fileName);
    }
    return result;
}

inline b32 ReadRequiredHeader(FILE *file, char *headerText)
{
    char line[256];
    b32 result =false;
    if(fgets(line, sizeof(line), file))
    {
        char *c = SkipWhitespace(line);
        memory_index length = StringLength(headerText);
        if(StringsAreEqual(length, c, headerText) && c[length] == '\n')
        {
            result = true;
        }
    }
    else
    {
        printf("ERROR: Cannot read required header %s from meta file!\n", headerText);
    }
    return result;
}

inline char * SkipToPropValue(FILE *file, char *propName, char *line)
{
    char *c = line;
    memory_index length = StringLengthUntilOneOf(c, "-");
    if(c[length] == '-')
    {
        c += length + 1;
    }
    c = SkipWhitespace(c);
    length = StringLengthUntilOneOf(c, ":");
    if(c[length] == ':' && StringsAreEqual(length, c, propName))
    {
        c += length + 1;
        c = SkipWhitespace(c);
        return c;
    }
    else
    {
        printf("ERROR: Cannot read prop %s from meta file!\n", propName);
    }
    return 0;
}

inline b32 ReadI32Prop(FILE *file, char *propName, i32 *value)
{
    b32 result = false;
    char line[256];
    if(fgets(line, sizeof(line), file))
    {
        char *c = SkipToPropValue(file, propName, line);
        if(c)
        {
            *value = StringParseI32(c, &c);
            result = true;
        }
    }
    else
    {
        printf("ERROR: No more lines in meta file!\n");
    }
    return result;
}

inline b32 ReadU32Prop(FILE *file, char *propName, u32 *value)
{
    b32 result = false;
    char line[256];
    if(fgets(line, sizeof(line), file))
    {
        char *c = SkipToPropValue(file, propName, line);
        if(c)
        {
            *value = StringParseU32(c, &c);
            result = true;
        }
    }
    else
    {
        printf("ERROR: No more lines in meta file!\n");
    }
    return result;
}

inline b32 ReadR32Prop(FILE *file, char *propName, r32 *value)
{
    b32 result = false;
    char line[256];
    if(fgets(line, sizeof(line), file))
    {
        char *c = SkipToPropValue(file, propName, line);
        if(c)
        {
            *value = StringParseR32(c, &c);
            result = true;
        }
    }
    else
    {
        printf("ERROR: No more lines in meta file!\n");
    }
    return result;
}

inline b32 ReadStringProp(FILE *file, char *propName, char *value)
{
    b32 result = false;
    char line[256];
    if(fgets(line, sizeof(line), file))
    {
        char *c = SkipToPropValue(file, propName, line);
        if(c)
        {
            memory_index length = StringLengthUntilOneOf(c, "\n");
            if(c[length] == '\n')
            {
                memcpy(value, c, length);
                value[length] = '\0';
                result = true;
            }
        }
    }
    else
    {
        printf("ERROR: No more lines in meta file!\n");
    }
    return result;
}

inline b32 ReadRectiProp(FILE *file, char *propName, recti *value)
{
    b32 result = false;
    if(ReadRequiredHeader(file, propName)
        && (ReadI32Prop(file, "x", &value->x) || ReadI32Prop(file, "min.x", &value->min.x))
        && (ReadI32Prop(file, "y", &value->y) || ReadI32Prop(file, "min.y", &value->min.y))
        && (ReadI32Prop(file, "width", &value->width) || ReadI32Prop(file, "max.x", &value->max.x))
        && (ReadI32Prop(file, "height", &value->height) || ReadI32Prop(file, "max.y", &value->max.y)))
    {
        result = true;
    }
    else
    {
        printf("ERROR: Cannot read prop %s from meta file!\n", propName);
    }
    return result;
}

inline b32 ReadV2Prop(FILE *file, char *propName, v2 *value)
{
    b32 result = false;
    if(ReadRequiredHeader(file, propName)
        && (ReadR32Prop(file, "x", &value->x) || (ReadR32Prop(file, "u", &value->u)))
        && (ReadR32Prop(file, "y", &value->y) || (ReadR32Prop(file, "v", &value->v))))
    {
        result = true;
    }
    else
    {
        printf("ERROR: Cannot read prop %s from meta file!\n", propName);
    }
    return result;
}

inline b32 ReadV4Prop(FILE *file, char *propName, v4 *value)
{
    b32 result = false;
    if(ReadRequiredHeader(file, propName)
        && (ReadR32Prop(file, "top", &value->top) || (ReadR32Prop(file, "x", &value->x)) || (ReadR32Prop(file, "r", &value->r)))
        && (ReadR32Prop(file, "right", &value->right) || (ReadR32Prop(file, "y", &value->y)) || (ReadR32Prop(file, "g", &value->g)))
        && (ReadR32Prop(file, "bottom", &value->bottom) || (ReadR32Prop(file, "z", &value->z)) || (ReadR32Prop(file, "b", &value->b)))
        && (ReadR32Prop(file, "left", &value->left) || (ReadR32Prop(file, "w", &value->w)) || (ReadR32Prop(file, "a", &value->a))))
    {
        result = true;
    }
    else
    {
        printf("ERROR: Cannot read prop %s from meta file!\n", propName);
    }
    return result;
}

inline b32 ReadSprite(FILE *file, sprite_meta *sprite)
{
    return ReadU32Prop(file, "index", &sprite->index)
        && ReadStringProp(file, "name", sprite->name)
        && ReadRectiProp(file, "rect:", &sprite->rect)
        && ReadV2Prop(file, "pivot:", &sprite->pivot)
        && ReadV4Prop(file, "border:", &sprite->border);
}

internal texture_content LoadPNG(asset_meta *assetMeta)
{
    texture_content result = {};

    char metaExtension[16] = ".meta";
    char metaFileName[MAX_FILE_PATH_LENGTH];
    memcpy(metaFileName, assetMeta->fileName, StringLength(assetMeta->fileName) + 1);
    memcpy(metaFileName + StringLength(assetMeta->fileName), metaExtension, StringLength(metaExtension));
    FILE *metaFile = fopen(metaFileName, "r");
    if(metaFile)
    {
        if(ReadRequiredHeader(metaFile, "type: texture"))
        {
            if(ReadRequiredHeader(metaFile, "settings:")
                && ReadU32Prop(metaFile, "width", &assetMeta->texture.width)
                && ReadU32Prop(metaFile, "height", &assetMeta->texture.height)
                && ReadU32Prop(metaFile, "filterMode", &assetMeta->texture.filterMode)
                && ReadU32Prop(metaFile, "wrapMode", &assetMeta->texture.wrapMode))
            {
                if(ReadRequiredHeader(metaFile, "sprites:"))
                {
                    assetMeta->texture.spritesLength = 0;
                    sprite_meta sprite = {};
                    while(ReadSprite(metaFile, &sprite))
                    {
                        assetMeta->texture.spritesLength++;
                        result.sprites = (sprite_meta *)realloc(result.sprites, sizeof(sprite_meta) * assetMeta->texture.spritesLength);
                        sprite_meta *dest = result.sprites + assetMeta->texture.spritesLength - 1;
                        *dest = sprite;
                    }
                    Assert(assetMeta->texture.spritesLength > 0);
                }
                else
                {
                    printf("ERROR: Cannot read sprites section from meta file %s!\n", metaFileName);
                }
            }
            else
            {
                printf("ERROR: Cannot read settings from meta file %s!\n", metaFileName);
            }
        }
        else
        {
            printf("ERROR: Meta file %s is of an incorrect type!\n", metaFileName);
        }
        fclose(metaFile);
    }
    else
    {
        printf("ERROR: Cannot open meta file %s!\n", metaFileName);
    }

    i32 n, w, h;
    result.content = (u8 *)stbi_load(assetMeta->fileName, &w, &h, &n, 4);

    return result;
}

internal sound_content LoadWAV(char *fileName)
{
    sound_content result = {};
    return result;
}

internal asset_type *GetAssetType(asset_file *assetFile, u32 typeId)
{
    for(u32 i = 0; i < assetFile->header.assetTypeCount; i++)
    {
        asset_type *assetType = assetFile->assetTypeArray + i;
        if(assetType->typeId == typeId)
        {
            return assetType;
        }
    }
    return 0;
}

internal void AddAsset(asset_file *assetFile, char *fileName, u32 typeId)
{
    asset_type *assetType = GetAssetType(assetFile, typeId);
    if(!assetType)
    {
        assetType = assetFile->assetTypeArray + assetFile->header.assetTypeCount;
        assetType->typeId = typeId;
        assetType->firstAssetIndex = assetFile->header.assetCount;
        assetType->assetCount = 0;
        assetFile->header.assetTypeCount++;
    }
    Assert(assetType->assetCount < MAX_ASSETS_PER_ASSET_FILE);
    asset_meta *asset = assetFile->assetArray + assetFile->header.assetCount;
    ZeroStruct(*asset);
    asset->typeId = typeId;
    memcpy(asset->fileName, fileName, StringLength(fileName) + 1);
    assetType->assetCount++;
    assetFile->header.assetCount++;
}

internal void AddTexture(asset_file *assetFile, char *fileName)
{
    AddAsset(assetFile, fileName, AssetType_Texture);
}

internal void AddSound(asset_file *assetFile, char *fileName)
{
    AddAsset(assetFile, fileName, AssetType_Sound);
}

internal void WriteAssetFile(asset_file *assetFile, char *assetFileName)
{
    FILE *out = fopen(assetFileName, "wb");
    if(out)
    {
        u32 headerSize = sizeof(asset_file_header);
        u32 assetTypeArraySize = assetFile->header.assetTypeCount * sizeof(asset_type);

        //write header
        assetFile->header.magicValue = ASSET_FILE_MAGIC_VALUE;
        assetFile->header.version = ASSET_FILE_VERSION;
        assetFile->header.assetTypeArrayOffset = headerSize;
        assetFile->header.assetArrayOffset = headerSize + assetTypeArraySize;
        fwrite(&assetFile->header, sizeof(asset_file_header), 1, out);

        //write asset types
        fwrite(assetFile->assetTypeArray, assetTypeArraySize, 1, out);

        //write asset metas
        u64 dataOffset = assetFile->header.assetArrayOffset + assetFile->header.assetCount * sizeof(asset_meta);
        for(u32 i = 0; i < assetFile->header.assetCount; i++)
        {
            asset_meta *assetMeta = assetFile->assetArray + i;
            if(assetMeta->typeId == AssetType_Texture)
            {
                texture_content textureContent = LoadPNG(assetMeta);
                assetMeta->dataOffset = dataOffset;
                fwrite(assetMeta, sizeof(asset_meta), 1, out);

                u64 currentOffset = ftell(out);
                fseek(out, (u32)dataOffset, SEEK_SET);
                fwrite(textureContent.sprites, sizeof(sprite_meta) * assetMeta->texture.spritesLength, 1, out);
                fwrite(textureContent.content, 4 * assetMeta->texture.width * assetMeta->texture.height, 1, out);
                dataOffset = ftell(out);
                fseek(out, (u32)currentOffset, SEEK_SET);

                free(textureContent.sprites);
                stbi_image_free(textureContent.content);
            }
            else if(assetMeta->typeId == AssetType_Sound)
            {
                //TODO: handle sounds
            }
        }
        fclose(out);
    }
    else
    {
        printf("ERROR: Cannot create asset file.\n");
    }
}

int main(int argCount, char **args)
{
    DetectInstructionSets();

    asset_file assetFile = {};
    assetFile.assetTypeArray = (asset_type *)malloc(sizeof(asset_type) * AssetType_Count);
    assetFile.assetArray = (asset_meta *)malloc(sizeof(asset_meta) * MAX_ASSETS_PER_ASSET_FILE);

    //write first file
    assetFile.header = {};
    AddTexture(&assetFile, "test1.png");
    AddTexture(&assetFile, "test2.png");
    WriteAssetFile(&assetFile, "assetFile.cma");

    //write second file
    assetFile.header = {};
    AddTexture(&assetFile, "test3.png");
    WriteAssetFile(&assetFile, "otherAssetFile.cma");

    free(assetFile.assetTypeArray);
    free(assetFile.assetArray);
}

