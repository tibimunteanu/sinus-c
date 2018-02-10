#ifndef THREAD_H
#define THREAD_H 

struct work_queue
{
    u32 volatile completionGoal;
    u32 volatile completionCount;
    u32 volatile nextEntryToWrite;
    u32 volatile nextEntryToRead;
    HANDLE semaphoreHandle;
    work_queue_entry entries[256];
};

internal void Win32AddWorkQueueEntry(work_queue *queue, work_queue_callback *callback, void *data)
{
    u32 newNextEntryToWrite = (queue->nextEntryToWrite + 1) % ArrayCount(queue->entries);
    Assert(newNextEntryToWrite != queue->nextEntryToRead);
    work_queue_entry *entry = queue->entries + queue->nextEntryToWrite;
    entry->callback = callback;
    entry->data = data;
    ++queue->completionGoal;
    CompletePreviousWritesBeforeFutureWrites;
    queue->nextEntryToWrite = newNextEntryToWrite;
    ReleaseSemaphore(queue->semaphoreHandle, 1, 0);
}

internal b32 Win32DoNextWorkQueueEntry(work_queue *queue)
{
    b32 shouldSleep = false;
    u32 originalNextEntryToRead = queue->nextEntryToRead;
    u32 newNextEntryToRead = (originalNextEntryToRead + 1) % ArrayCount(queue->entries);
    if(originalNextEntryToRead != queue->nextEntryToWrite)
    {
        u32 index = AtomicCompareExchangeU32(&queue->nextEntryToRead, newNextEntryToRead, originalNextEntryToRead);
        if(index == originalNextEntryToRead)
        {
            work_queue_entry entry = queue->entries[index];
            entry.callback(queue, entry.data);
            AtomicIncrementU32(&queue->completionCount);
        }
    }
    else
    {
        shouldSleep = true;
    }
    return shouldSleep;
}

internal void Win32CompleteWorkQueue(work_queue *queue)
{
    while(queue->completionGoal != queue->completionCount)
    {
        Win32DoNextWorkQueueEntry(queue);
    }
    queue->completionGoal = 0;
    queue->completionCount = 0;
}

DWORD WINAPI Win32ThreadProc(LPVOID lpParameter)
{
    work_queue *queue = (work_queue *)lpParameter;
    for(;;)
    {
        if(Win32DoNextWorkQueueEntry(queue))
        {
            WaitForSingleObjectEx(queue->semaphoreHandle, INFINITE, FALSE);
        }
    }
}

internal void Win32InitWorkQueue(work_queue *queue, u32 threadCount)
{
    queue->completionGoal = 0;
    queue->completionCount = 0;
    queue->nextEntryToWrite = 0;
    queue->nextEntryToRead = 0;
    u32 initialCount = 0;
    queue->semaphoreHandle = CreateSemaphoreEx(0, initialCount, threadCount, 0, 0, SEMAPHORE_ALL_ACCESS);
    for(u32 threadIndex = 0; threadIndex < threadCount; threadIndex++)
    {
        DWORD threadId;
        HANDLE threadHandle = CreateThread(0, 0, Win32ThreadProc, queue, 0, &threadId);
        CloseHandle(threadHandle);
    }
}

#endif /* THREAD_H */
