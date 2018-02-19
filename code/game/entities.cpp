#include "entities.h"

global u32 entityId = 1;

internal entity_manager *EntityManagerInit(memory_arena *arena)
{
    entity_manager *entityManager = (entity_manager *)PushStruct(arena, entity_manager, {true, 4});
    entityManager->handles = HandleManagerInit(arena, (ComponentType_Count * MAX_COMPONENTS) + MAX_ENTITIES);
    return entityManager;
}

internal handle AddEntity(entity_manager *entityManager)
{
    Assert(entityManager->entitiesLength < ArrayCount(entityManager->entities));
    entity *entity = entityManager->entities + entityManager->entitiesLength++;
    handle handle = AddHandle(entityManager->handles, entity);
    entity->id = handle.id;
    entity->handle = handle;
    return handle;
}

internal void FreeEntity(entity_manager *entityManager, handle entityToFreeHandle)
{
    entity *entityToFree = (entity *)GetFromHandle(entityManager->handles, entityToFreeHandle);
    if(entityToFree)
    {
        RemoveHandle(entityManager->handles, entityToFreeHandle);
        if(entityManager->entitiesLength > 1)
        {
            entity *lastEntity = entityManager->entities + entityManager->entitiesLength - 1;
            *entityToFree = *lastEntity;
            UpdateHandle(entityManager->handles, entityToFree->handle, entityToFree);
        }
        entityManager->entitiesLength--;
    }
}

inline entity *GetEntity(entity_manager *entityManager, handle entityHandle)
{
    entity *result = (entity *)GetFromHandle(entityManager->handles, entityHandle);
    return result;
}

