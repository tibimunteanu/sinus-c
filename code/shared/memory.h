#ifndef MEMORY_H
#define MEMORY_H 

#include "stdlib.h" //TODO: get rid of this
#include "string.h" //TODO: get rid of this

struct memory_arena
{
    u8 *base;
    memory_index size;
    memory_index used;
};

struct memory_arena_push_params
{
    b32 clear;
    u32 alignment;
};

struct memory_block
{
    memory_block *prev;
    memory_block *next;
    memory_index size;
    b32 isUsed;
    u32 marker;
};

struct general_allocator
{
    memory_block *sentinel;
    u32 splitThreshold;
};

#define ZeroStruct(instance) ZeroSize(sizeof(instance), &(instance))
#define ZeroArray(count, ptr) ZeroSize(count * sizeof((ptr)[0]), ptr)
inline void ZeroSize(memory_index size,
                     void* ptr)
{
    memset(ptr, 0, size);
}

#define PushStruct(arena, type, ...) (type *)PushSize(arena, sizeof(type), ## __VA_ARGS__)
#define PushArray(arena, count, type, ...) (type *)PushSize(arena, (count) * sizeof(type), ## __VA_ARGS__)
#define PushCopy(arena, size, source, ...) memcpy(PushSize(arena, size, ## __VA_ARGS__), source, size)
inline void* PushSize(memory_arena *arena,
                      memory_index size,
                      memory_arena_push_params params = {false, 4})
{
    // NOTE: the trick of computing the mask as alignment - 1
    // works only if alignment is a power of 2, but it's ok
    // because alignments should always be a power of 2
    memory_index alignmentOffset = 0;
    memory_index currentPtr = (memory_index)arena->base + arena->used;
    memory_index alignmentMask = params.alignment - 1;
    if(currentPtr & alignmentMask)
    {
        alignmentOffset = params.alignment - (currentPtr & alignmentMask);
    }

    Assert(arena->used + alignmentOffset + size <= arena->size);
    void *result = arena->base + arena->used + alignmentOffset;
    arena->used += alignmentOffset + size;
    if(params.clear)
    {
        ZeroSize(size, result);
    }
    return result;
}

inline char *PushString(memory_arena *arena,
                        char *source)
{
    u32 size = 1;
    for(char *i = source; *i; i++)
    {
        ++size;
    }
    char *result = (char *)PushSize(arena, size);
    for(u32 i = 0; i < size; i++)
    {
        result[i] = source[i];
    }
    return result;
}

inline void InitArena(memory_arena *arena,
                      memory_index size,
                      void* base)
{
    arena->size = size;
    arena->base = (u8 *)base;
    arena->used = 0;
}

inline void InitSubArena(memory_arena *subArena,
                         memory_arena *parentArena,
                         memory_index size,
                         memory_arena_push_params params = {false, 4})
{
    subArena->size = size;
    subArena->base = (u8 *)PushSize(parentArena, size, params);
    subArena->used = 0;
}

inline void ClearArena(memory_arena *arena)
{
    InitArena(arena, arena->size, arena->base);
}

// General purpose allocator
internal memory_block *GAInsertBlock(memory_block *prev, memory_index size, void *memory)
{
#define MEMORY_BLOCK_MARKER AsciiCode('m','e','m','b')
    memory_block *block = (memory_block *)memory;
    block->size = size;
    block->isUsed = false;
    block->marker = MEMORY_BLOCK_MARKER;
    block->prev = prev;
    block->next = prev->next;
    block->prev->next = block;
    block->next->prev = block;
    return block;
}

inline b32 GAIsValidBlockPtr(void *ptr)
{
    b32 result = ((memory_block *)ptr)->marker == MEMORY_BLOCK_MARKER;
    return result;
}

internal memory_block *GAFindFreeBlock(general_allocator *allocator, memory_index minSize)
{
    //TODO: make this match smarter than just stopping on the first unused block of that size
    //maybe we can find another block which can be splitted better
    memory_block *result = 0;
    for(memory_block *block = allocator->sentinel->next; block != allocator->sentinel; block = block->next)
    {
        if(!block->isUsed && block->size >= minSize)
        {
            Assert(GAIsValidBlockPtr(block));
            result = block;
            break;
        }
    }
    return result;
}

internal b32 GAMergeBlocks(general_allocator *allocator, memory_block *prev, memory_block *block)
{
    Assert(GAIsValidBlockPtr(prev) && GAIsValidBlockPtr(block));
    b32 result = false;
    if(prev != allocator->sentinel && block != allocator->sentinel
       && !prev->isUsed && !block->isUsed
       && (u8 *)prev + sizeof(memory_block) + prev->size == (u8 *)block)
    {
        prev->size += sizeof(memory_block) + block->size;
        prev->next = block->next;
        prev->next->prev = prev;
        block->prev = block->next = 0;
        result = true;
    }
    return result;
}

internal void *GAAllocate(general_allocator *allocator, memory_index size, memory_block *block)
{
    Assert(GAIsValidBlockPtr(block));
    void *result = 0;
    if(block && !block->isUsed && block->size >= size)
    {
        block->isUsed = true;
        result = (u8 *)block + sizeof(memory_block);
        memory_index remainingSize = block->size - size;
        if(remainingSize >= allocator->splitThreshold + sizeof(memory_block))
        {
            block->size = size;
            GAInsertBlock(block, remainingSize - sizeof(memory_block), (u8 *)result + size);
        }
    }
    return result;
}

internal void *GAAllocate(general_allocator *allocator, memory_index size)
{
    memory_block *block = GAFindFreeBlock(allocator, size);
    void *result = GAAllocate(allocator, size, block);
    return result;
}

internal memory_block *GAFree(general_allocator *allocator, void *memory)
{
    memory_block *block = (memory_block *)memory - sizeof(memory_block);
    Assert(GAIsValidBlockPtr(block));
    block->isUsed = false;
    memory_block *prev = block->prev;
    if(GAMergeBlocks(allocator, prev, block))
    {
        block = prev;
    }
    GAMergeBlocks(allocator, block, block->next);
    return block;
}

internal void GAInit(memory_arena *arena, general_allocator *allocator, memory_index size, u32 splitThreshold)
{
    allocator->splitThreshold = splitThreshold;
    allocator->sentinel = PushStruct(arena, memory_block);
    allocator->sentinel->size = 0;
    allocator->sentinel->marker = MEMORY_BLOCK_MARKER;
    allocator->sentinel->prev = allocator->sentinel;
    allocator->sentinel->next = allocator->sentinel;
    GAInsertBlock(allocator->sentinel, size, PushSize(arena, size));
}

#endif /* MEMORY_H */
