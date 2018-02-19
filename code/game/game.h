#ifndef GAME_H
#define GAME_H 

#include "../shared/shared.h"
#include "../shared/memory.h"
#include "../shared/utils.h"
#include "../shared/math.h"
#include "platform.h"
#include "assets.h"
#include "entities.h"

struct game_state
{
    b32 isInitialized;
    memory_arena arena;
    entity_manager *entityManager;
};

struct tran_state
{
    b32 isInitialized;
    memory_arena arena;
    asset_manager *assetManager;
};

#endif /* GAME_H */
