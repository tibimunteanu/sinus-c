#include "game.h"
#include "assets.cpp"

platform_api platform;

extern "C" void GameUpdateAndRender(memory_store *memory)
{
    platform = memory->platform;

    Assert(sizeof(game_state) <= memory->permanentStorageSize);
    game_state *gameState = (game_state *)memory->permanentStorage;
    if(!gameState->isInitialized)
    {
        InitArena(&gameState->arena, memory->permanentStorageSize - sizeof(game_state), (u8 *)memory->permanentStorage + sizeof(game_state));
        gameState->isInitialized = true;
    }

    Assert(sizeof(tran_state) <= memory->transientStorageSize);
    tran_state *tranState = (tran_state *)memory->transientStorage;
    if(!tranState->isInitialized)
    {
        InitArena(&tranState->arena, memory->transientStorageSize - sizeof(tran_state), (u8 *)memory->transientStorage + sizeof(tran_state));
        tranState->assets = AllocateGameAssets(&tranState->arena, Megabytes(128));
        tranState->isInitialized = true;
    }

    LoadTexture(tranState->assets, "test2.png");
    LoadTexture(tranState->assets, "test3.png");
    LoadTexture(tranState->assets, "test2.png");
}

