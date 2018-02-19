#include "game.h"
#include "assets.cpp"
#include "entities.cpp"

platform_api platform;

extern "C" void GameUpdateAndRender(memory_store *memory)
{
    platform = memory->platform;

    Assert(sizeof(game_state) <= memory->permanentStorageSize);
    game_state *gameState = (game_state *)memory->permanentStorage;
    if(!gameState->isInitialized)
    {
        InitArena(&gameState->arena, memory->permanentStorageSize - sizeof(game_state), (u8 *)memory->permanentStorage + sizeof(game_state));
        gameState->entityManager = EntityManagerInit(&gameState->arena);
        gameState->isInitialized = true;


        //test code
        handle p1Handle = AddEntity(gameState->entityManager);
        handle p2Handle = AddEntity(gameState->entityManager);
        handle p3Handle = AddEntity(gameState->entityManager);
        entity *p1 = GetEntity(gameState->entityManager, p1Handle);
        entity *p2 = GetEntity(gameState->entityManager, p2Handle);
        entity *p3 = GetEntity(gameState->entityManager, p3Handle);
        FreeEntity(gameState->entityManager, p1Handle);
        p1 = GetEntity(gameState->entityManager, p1Handle);
        handle p4Handle = AddEntity(gameState->entityManager);
        p1 = GetEntity(gameState->entityManager, p1Handle);
        p2 = GetEntity(gameState->entityManager, p2Handle);
        p3 = GetEntity(gameState->entityManager, p3Handle);
        entity *p4 = GetEntity(gameState->entityManager, p4Handle);
        FreeEntity(gameState->entityManager, p3Handle);
        FreeEntity(gameState->entityManager, p4Handle);
        FreeEntity(gameState->entityManager, p2Handle);
        FreeEntity(gameState->entityManager, p1Handle);
        FreeEntity(gameState->entityManager, p2Handle);
    }

    Assert(sizeof(tran_state) <= memory->transientStorageSize);
    tran_state *tranState = (tran_state *)memory->transientStorage;
    if(!tranState->isInitialized)
    {
        InitArena(&tranState->arena, memory->transientStorageSize - sizeof(tran_state), (u8 *)memory->transientStorage + sizeof(tran_state));
        tranState->assetManager = AssetManagerInit(&tranState->arena, Megabytes(128));
        tranState->isInitialized = true;
    }

    LoadShader(tranState->assetManager, "basic.shader");
    LoadTexture(tranState->assetManager, "test2.png");
    LoadTexture(tranState->assetManager, "test3.png");
    LoadTexture(tranState->assetManager, "test2.png");
}

