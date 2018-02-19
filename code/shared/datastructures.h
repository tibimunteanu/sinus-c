#ifndef DATASTRUCTURES_H 
#define DATASTRUCTURES_H 

#include "utils.h"
#include "memory.h"

//HASH SET U32
struct hash_set_entry_u32
{
    u32 key;
    void *value;
    hash_set_entry_u32 *next;
};

struct hash_set_u32
{
    u32 maxLength;
    u32 length;
    hash_set_entry_u32 **entries;
    hash_set_entry_u32 *firstFree;
};

internal hash_set_u32 *HashSetU32Init(memory_arena *arena, u32 maxLength)
{
    Assert(IsPowerOfTwo(maxLength));
    hash_set_u32 *hashSet = (hash_set_u32 *)PushStruct(arena, hash_set_u32, {true, 4});
    hashSet->maxLength = maxLength;
    hashSet->entries = (hash_set_entry_u32 **)PushSize(arena, maxLength * sizeof(void *));
    hash_set_entry_u32 *entries = (hash_set_entry_u32 *)PushArray(arena, maxLength, hash_set_entry_u32);
    for(u32 i = maxLength; i > 0; i--)
    {
        hash_set_entry_u32 *entry = entries + i - 1;
        entry->next = hashSet->firstFree;
        hashSet->firstFree= entry;
    }
    return hashSet;
}

inline hash_set_entry_u32 **HashSetU32GetEntrySlot(hash_set_u32 *hashSet, u32 key)
{
    u32 hashSlotIndex = key & (hashSet->maxLength - 1);
    Assert(hashSlotIndex < hashSet->maxLength);
    hash_set_entry_u32 **hashSlot = hashSet->entries + hashSlotIndex;
    while(*hashSlot && key != (*hashSlot)->key)
    {
        hashSlot = &(*hashSlot)->next;
    }
    return hashSlot;
}

inline void *HashSetU32Get(hash_set_u32 *hashSet, u32 key)
{
    void *result = 0;
    hash_set_entry_u32 *entry = *HashSetU32GetEntrySlot(hashSet, key);
    if(entry)
    {
        result = entry->value;
    }
    return result;
}

internal hash_set_entry_u32 *HashSetU32Add(hash_set_u32 *hashSet, u32 key, void *value)
{
    hash_set_entry_u32 **hashSlot = HashSetU32GetEntrySlot(hashSet, key);
    hash_set_entry_u32 *entry = *hashSlot;
    Assert(!entry);
    Assert(hashSet->firstFree);
    entry = hashSet->firstFree;
    hashSet->firstFree = entry->next;
    entry->next = *hashSlot;
    *hashSlot = entry;
    entry->key = key;
    entry->value = value;
    hashSet->length++;
    return entry;
}

internal void HashSetU32Remove(hash_set_u32 *hashSet, u32 key)
{
    hash_set_entry_u32 **hashSlot = HashSetU32GetEntrySlot(hashSet, key);
    hash_set_entry_u32 *entry = *hashSlot;
    if(entry)
    {
        *hashSlot = entry->next;
        entry->next = hashSet->firstFree;
        hashSet->firstFree = entry;
        hashSet->length--;
    }
}

#endif /* DATASTRUCTURES_H */
