#ifdef __cplusplus
extern "C" {
#endif

#if COMPILER_MSVC
#define CompletePreviousReadsBeforeFutureReads _ReadBarrier()
#define CompletePreviousWritesBeforeFutureWrites _WriteBarrier()
inline u32 AtomicCompareExchangeU32(u32 volatile *value, u32 newValue, u32 expectedValue)
{
    u32 result = _InterlockedCompareExchange((long volatile *)value, newValue, expectedValue);
    return result;
}
inline void AtomicIncrementU32(u32 volatile *value)
{
    _InterlockedIncrement((long volatile *)value);
}
inline u64 AtomicExchangeU64(u64 volatile  *value, u64 newValue)
{
    u64 result = _InterlockedExchange64((__int64 volatile *)value, newValue);
    return result;
}
inline u64 AtomicAddU64(u64 volatile *value, u64 addend)
{
    u64 result = _InterlockedExchangeAdd64((__int64 volatile *)value, addend);
    return result;
}
inline u32 ThreadId(void)
{
    u8 *threadLocalStorage = (u8 *)__readgsqword(0x30);
    u32 threadId = *(u32 *)(threadLocalStorage + 0x48);
    return threadId;
}
#elif COMPILER_LLVM
#define CompletePreviousReadsBeforeFutureReads asm volatile("" ::: "memory")
#define CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory")
inline u32 AtomicCompareExchangeU32(u32 volatile *value, u32 newValue, u32 expectedValue)
{
    u32 result = __sync_val_compare_and_swap(value, expectedValue, newValue);
    return result;
}
inline void AtomicIncrementU32(u32 volatile *value)
{
    NotImplemented;
}
inline u64 AtomicExchangeU64(u64 volatile *value, u64 newValue)
{
    u64 result = __sync_lock_test_and_set(value, newValue);
    return result;
}
inline u64 AtomicAddU64(u64 volatile *value, u64 addend)
{
    u64 result = __sync_fetch_and_add(value, addend);
    return result;
}
inline u32 ThreadId(void)
{
    u32 threadId;
#if defined(__APPLE__) && defined(__x86_64__)
    asm("mov %%gs:0x00,%0" : "=r"(threadId));
#elif defined(__i386__)
    asm("mov %%gs:0x08,%0" : "=r"(threadId));
#elif defined(__x86_64__)
    asm("mov %%fs:0x10,%0" : "=r"(threadId));
#else
#error Unsupported architecture
#endif
    return threadId;
}
#else
#error Unsupported compiler
#endif

typedef enum file_types
{
    FileType_Asset,
    FileType_SaveGame,

    FileType_Count
} file_types;

typedef struct file
{
    u32 size;
    void *content;
} file;

typedef struct file_handle
{
    b32 noErrors;
    void *platform;
} file_handle;

typedef struct file_group
{
    u32 fileCount;
    void *platform;
} file_group;

typedef struct ticket_mutex
{
    u64 volatile ticket;
    u64 volatile serving;
} ticket_mutex;

struct work_queue;

//api functins that the platform offers to the game
typedef void work_queue_callback(work_queue *queue, void *data);
typedef void add_work_queue_entry(work_queue *queue, work_queue_callback *callback, void *data);
typedef void complete_work_queue(work_queue *queue);
typedef file_group get_all_files_of_type_begin(file_types fileType);
typedef void get_all_files_of_type_end(file_group *fileGroup);
typedef file_handle open_next_file(file_group *fileGroup);
typedef void add_file_error(file_handle *file, char *message);
typedef void read_data_from_file(file_handle *file, memory_index offset, memory_index size, void *dest);

typedef struct work_queue_entry
{
    work_queue_callback *callback;
    void * data;
} work_queue_entry;

typedef struct platform_api
{
    //work queue
    add_work_queue_entry *AddWorkQueueEntry;
    complete_work_queue *CompleteWorkQueue;

    //file io
    get_all_files_of_type_begin *GetAllFilesOfTypeBegin;
    get_all_files_of_type_end *GetAllFilesOfTypeEnd;
    open_next_file *OpenNextFile;
    add_file_error *AddFileError;
    read_data_from_file *ReadDataFromFile;
} platform_api;

extern platform_api platform;

struct memory_store
{
    memory_index permanentStorageSize;
    memory_index transientStorageSize;
    void *permanentStorage;
    void *transientStorage;
    work_queue *highPriorityQueue;
    work_queue *lowPriorityQueue;
    platform_api platform;
};

inline void BeginTicketMutex(ticket_mutex *mutex)
{
    u64 ticket = AtomicAddU64(&mutex->ticket, 1);
    while(ticket != mutex->serving);
}

inline void EndTicketMutex(ticket_mutex *mutex)
{
    AtomicAddU64(&mutex->serving, 1);
}

//api functions that the game offers to the platform
typedef void game_update_and_render(memory_store *memory);

#ifdef __cplusplus
}
#endif

