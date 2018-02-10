struct win32_file_handle
{
    HANDLE fileHandle;
};

struct win32_file_group
{
    HANDLE findHandle;
    WIN32_FIND_DATAW findData;
};

internal file_group Win32GetAllFilesOfTypeBegin(file_types fileType)
{
    file_group result = {};

    //TODO: use an arena for win32 allocations
    win32_file_group *win32FileGroup = (win32_file_group *)VirtualAlloc(0, sizeof(win32_file_group), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    result.platform = win32FileGroup;

    wchar *wildCard = L"*.*";
    switch(fileType)
    {
        case FileType_Asset:
        {
            wildCard = L"*.cma";
        } break;

        case FileType_SaveGame:
        {
            wildCard = L"*.cms";
        } break;

        InvalidDefaultCase;
    }
    result.fileCount = 0;

    WIN32_FIND_DATAW findData;
    HANDLE findHandle = FindFirstFileW(wildCard, &findData);
    while(findHandle != INVALID_HANDLE_VALUE)
    {
        result.fileCount++;
        if(!FindNextFileW(findHandle, &findData))
        {
            break;
        }
    }
    FindClose(findHandle);
    win32FileGroup->findHandle = FindFirstFileW(wildCard, &win32FileGroup->findData);

    return result;
}

internal void Win32GetAllFilesOfTypeEnd(file_group *fileGroup)
{
    win32_file_group *win32FileGroup = (win32_file_group *)fileGroup->platform;
    if(win32FileGroup)
    {
        FindClose(win32FileGroup->findHandle);
        VirtualFree(win32FileGroup, 0, MEM_RELEASE);
    }
}

internal file_handle Win32OpenNextFile(file_group *fileGroup)
{
    file_handle result = {};
    win32_file_group *win32FileGroup = (win32_file_group *)fileGroup->platform;
    if(win32FileGroup->findHandle != INVALID_HANDLE_VALUE)
    {
        //TODO: use an arena for win32 allocations
        win32_file_handle *win32FileHandle = (win32_file_handle *)VirtualAlloc(0, sizeof(win32_file_handle), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        result.platform = win32FileHandle;

        if(win32FileHandle)
        {
            wchar *fileName = win32FileGroup->findData.cFileName;
            win32FileHandle->fileHandle = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
            result.noErrors = win32FileHandle->fileHandle != INVALID_HANDLE_VALUE;
        }
        if(!FindNextFileW(win32FileGroup->findHandle, &win32FileGroup->findData))
        {
            FindClose(win32FileGroup->findHandle);
            win32FileGroup->findHandle = INVALID_HANDLE_VALUE;
        }
    }
    return result;
}

internal void Win32AddFileError(file_handle *fileHandle, char *message)
{
    OutputDebugStringA(message);
    fileHandle->noErrors = false;
}

internal void Win32ReadDataFromFile(file_handle *source, u64 offset, u64 size, void *dest)
{
    if(source->noErrors)
    {
        win32_file_handle *win32FileHandle = (win32_file_handle *)source->platform;
        OVERLAPPED overlapped = {};
        overlapped.Offset = (u32)((offset >> 0) & 0xFFFFFFFF);
        overlapped.OffsetHigh = (u32)((offset >> 32) & 0xFFFFFFFF);
        u32 fileSize32 = SafeTruncateToU32(size);
        DWORD bytesRead;
        if(ReadFile(win32FileHandle->fileHandle, dest, fileSize32, &bytesRead, &overlapped) && fileSize32 == bytesRead)
        {
            //NOTE: success
        }
        else
        {
            Win32AddFileError(source, "Read file failed!");
        }
    }
}

