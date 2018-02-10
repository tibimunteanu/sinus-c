#ifndef ASSETS_H
#define ASSETS_H 

#define MAX_ASSETS_PER_ASSET_FILE 4096

enum asset_types
{
    AssetType_Texture,
    AssetType_Sound,

    AssetType_Count
};

enum asset_state
{
    AssetState_Unloaded,
    AssetState_Queued,
    AssetState_Loaded
};

enum filter_modes
{
    FilterModes_Nearest,
    FilterModes_Billinear,
    FilterModes_Trillinear
};

enum wrap_modes
{
    WrapModes_Clamp,
    WrapModes_Repeat
};

#pragma pack(push, 1)

struct asset_file_header
{
#define ASSET_FILE_MAGIC_VALUE AsciiCode('3', '0', 'c', 'm')
    u32 magicValue;
#define ASSET_FILE_VERSION 0
    u32 version;

    u32 assetTypeCount;
    u32 assetCount;
    u64 assetTypeArrayOffset;
    u64 assetArrayOffset;
};

struct asset_type
{
    u32 typeId;
    u32 firstAssetIndex;
    u32 assetCount;
};

struct texture_meta
{
    u32 width;
    u32 height;
    u32 filterMode;
    u32 wrapMode;
    u32 spritesLength;
};

struct sprite_meta
{
#define MAX_SPRITE_NAME_LENGTH 64
    u32 index;
    char name[MAX_SPRITE_NAME_LENGTH];
    recti rect;
    v2 pivot;
    v4 border;
};

struct sound_meta
{
    u32 sampleCount;
    u32 channelCount;
};

struct asset_meta
{
#define MAX_FILE_PATH_LENGTH 256
    u32 typeId;
    char fileName[MAX_FILE_PATH_LENGTH];
    u64 dataOffset;
    union
    {
        texture_meta texture;
        sound_meta sound;
    };
};

#pragma pack(pop)

struct asset_file
{
    file_handle handle;
    asset_file_header header;
    asset_type *assetTypeArray;
    asset_meta *assetArray;
};

struct texture_content
{
    sprite_meta *sprites;
    u8 *content;
};

struct sound_content
{
    u8 *content;
};

struct asset_handle
{
    u32 assetIndex;
    u32 fileIndex;
    u32 state;
    asset_meta meta;
    union
    {
        texture_content texture;
        sound_content sound;
    };
};

struct asset_lru
{
    asset_lru *next;
    asset_lru *prev;
    u32 assetIndex;
};

struct game_assets
{
    memory_arena *arena;
    general_allocator assetsAllocator;
    asset_lru *assetLruSentinel;

    u32 fileCount;
    asset_file *files;
    u32 assetCount;
    asset_handle *assets;
    asset_type assetTypes[AssetType_Count];
};

#endif /* ASSETS_H */
