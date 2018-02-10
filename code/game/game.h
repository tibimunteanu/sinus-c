#ifndef GAME_H
#define GAME_H 

#include "../shared/shared.h"
#include "../shared/memory.h"
#include "../shared/utils.h"
#include "../shared/math.h"
#include "platform.h"
#include "assets.h"

struct game_state
{
    b32 isInitialized;
    memory_arena arena;
};

struct tran_state
{
    b32 isInitialized;
    memory_arena arena;
    game_assets *assets;
};

#endif /* GAME_H */
