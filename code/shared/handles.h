#ifndef HANDLES_H
#define HANDLES_H

struct handle
{
    u32 index;
    u32 id;
};

struct handle_entry
{
    u32 nextFreeIndex;
    u32 id;
    b32 isActive;
    b32 isEndOfList;
    void *ptr;
};

struct handle_manager
{
    u32 maxLength;
    u32 length;
    handle_entry *entries;
    u32 firstFreeIndex;
    u32 nextId;
};

internal handle_manager *HandleManagerInit(memory_arena *arena, u32 maxLength)
{
    handle_manager *handles = (handle_manager *)PushStruct(arena, handle_manager);
    handles->maxLength = maxLength;
    handles->length = 0;
    handles->firstFreeIndex = 0;
    handles->entries = (handle_entry *)PushArray(arena, maxLength, handle_entry, {true, 4});
    for(u32 i = 0; i < maxLength - 1; i++)
    {
        handles->entries[i].nextFreeIndex = i + 1;
    }
    handles->entries[maxLength - 1].isEndOfList = true;
    return handles;
}

internal handle AddHandle(handle_manager *handles, void *ptr)
{
    Assert(handles->length < handles->maxLength);
    u32 newIndex = handles->firstFreeIndex;
    Assert(newIndex < handles->maxLength);
    handle_entry *entry = handles->entries + newIndex;
    Assert(!entry->isActive && !entry->isEndOfList);
    handles->firstFreeIndex = entry->nextFreeIndex;
    entry->nextFreeIndex = 0;
    entry->id = ++handles->nextId;
    Assert(entry->id != 0);
    entry->isActive = true;
    entry->ptr = ptr;
    handles->length++;

    handle result = {newIndex, entry->id};
    return result;
}

internal void UpdateHandle(handle_manager *handles, handle handle, void *ptr)
{
    handle_entry *entry = handles->entries + handle.index;
    Assert(entry->isActive && entry->id == handle.id);
    entry->ptr = ptr;
}

internal void RemoveHandle(handle_manager *handles, handle handle)
{
    handle_entry *entry = handles->entries + handle.index;
    if(entry->isActive && entry->id == handle.id)
    {
        entry->nextFreeIndex = handles->firstFreeIndex;
        entry->isActive = false;
        handles->firstFreeIndex = handle.index;
        handles->length--;
    }
}

inline void *GetFromHandle(handle_manager *handles, handle handle)
{
    void *result = 0;
    handle_entry *entry = handles->entries + handle.index;
    if(entry->isActive && entry->id == handle.id)
    {
        result = entry->ptr;
    }
    return result;
}

#endif /* HANDLES_H */
