#include<windows.h>
#include<stdio.h>
#include<dbt.h>
#include<gl/gl.h>

#include "guiddef.h"
#include "../../shared/shared.h"
#include "../../shared/memory.h"
#include "../../shared/utils.h"
#include "../../shared/math.h"
#include "../../game/platform.h"
#include "workqueue.h"
#include "fileio.h"
#include "monitor.h"
#include "input.h"

struct win32_state
{
#define MAX_GAMEPAD_COUNT 16
#define MAX_MONITOR_COUNT 16
    void *memory;

    u32 gamepadCount;
    gamepad gamepads[MAX_GAMEPAD_COUNT];
    u32 gamepadMappingCount;
    gamepad_mapping gamepadMappings[256];
    
    u32 monitorCount;
    display_monitor monitors[MAX_MONITOR_COUNT];
    display_monitor *currentMonitor;
    WINDOWPLACEMENT windowPosition;
    display_settings displaySettings;

    char exeFolderPath[MAX_PATH];
    char exeFileName[MAX_PATH];
    char exeFullPath[MAX_PATH];
    char gameDllFullPath[MAX_PATH];
    char tempGameDllFullPath[MAX_PATH];
    char lockTmpFullPath[MAX_PATH];
};

struct win32_game_dll
{
    b32 isValid;
    HMODULE gameDll;
    FILETIME gameDllLastModified;
    game_update_and_render *UpdateAndRender;
};

#include "monitor.cpp"
#include "opengl.cpp"
#include "input.cpp"
#include "renderer.h"

global b32 running;
win32_state win32State = {};

internal void Win32SetFilePaths()
{
    DWORD sizeofFileName = GetModuleFileNameA(0, win32State.exeFullPath, sizeof(win32State.exeFullPath));
    char *exeFileName = win32State.exeFullPath;
    for(char *c = win32State.exeFullPath; *c; c++)
    {
        if(*c == '\\')
        {
            exeFileName = c + 1;
        }
    }
    CopyString(win32State.exeFileName, exeFileName, StringLength(exeFileName));
    CopyString(win32State.exeFolderPath, win32State.exeFullPath, exeFileName - win32State.exeFullPath);
    CatStrings(win32State.gameDllFullPath, win32State.exeFolderPath, "game.dll");
    CatStrings(win32State.tempGameDllFullPath, win32State.exeFolderPath, "game_temp.dll");
    CatStrings(win32State.lockTmpFullPath, win32State.exeFolderPath, "lock.tmp");
}

internal win32_game_dll Win32LoadGameDll()
{
    win32_game_dll result = {};
    if(!Win32FileExists(win32State.lockTmpFullPath))
    {
        result.gameDllLastModified = Win32GetFileLastModified(win32State.gameDllFullPath);
        CopyFile(win32State.gameDllFullPath, win32State.tempGameDllFullPath, FALSE);
        result.gameDll = LoadLibraryA(win32State.tempGameDllFullPath);
        if(result.gameDll)
        {
            result.UpdateAndRender = (game_update_and_render *)GetProcAddress(result.gameDll, "GameUpdateAndRender");
            result.isValid = result.UpdateAndRender != 0;
        }
    }
    if(!result.isValid)
    {
        result.UpdateAndRender = 0;
    }
    return result;
}

internal void Win32UnloadGameDll(win32_game_dll *game)
{
    if(game->gameDll)
    {
        FreeLibrary(game->gameDll);
        game->gameDll = 0;
    }
    game->isValid = false;
    game->UpdateAndRender = 0;
}

LRESULT CALLBACK Win32MainWindowCallback(HWND window,
                                         UINT message,
                                         WPARAM wParam,
                                         LPARAM lParam)
{
    LRESULT result = 0;
    switch(message)
    {
        case WM_MOVE:
        {
            for(u32 i = 0; i < win32State.gamepadCount; i++)
            {
                gamepad *g = win32State.gamepads + i;
                GetGamepadState(g);
            }
        } break;
        
        case WM_DISPLAYCHANGE:
        {
            PollMonitors(window);
        } break;

        case WM_DEVICECHANGE:
        {
            if(wParam == DBT_DEVICEARRIVAL)
            {
                DEV_BROADCAST_HDR *device = (DEV_BROADCAST_HDR *)lParam;
                if(device && device->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    DIDetectConnectedGamepads(window);
                }
            }
            else if(wParam == DBT_DEVICEREMOVECOMPLETE)
            {
                DEV_BROADCAST_HDR *device = (DEV_BROADCAST_HDR *)lParam;
                if(device && device->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    DIDetectDisconnectedGamepads();
                }
            }
        } break;
        
        case WM_CLOSE:
        case WM_DESTROY:
        {
            running = false;
        } break;

        case WM_SIZE:
        {
            i32 width, height;
            GetClientSize(window, &width, &height);
            RendererSetViewport(0, 0, width, height);
        } break;

        default:
        {
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }

    return(result);
}

int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    Win32SetFilePaths();

    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    windowClass.lpfnWndProc = Win32MainWindowCallback;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = "SinusClass";
    
    if(RegisterClassEx(&windowClass))
    {
        DWORD exStyle = WS_EX_APPWINDOW;
        DWORD style = WS_OVERLAPPEDWINDOW_NORESIZE|WS_VISIBLE;
        i32 width;
        i32 height;
        GetWindowSize(style, exStyle, 960, 540, &width, &height);
        
        HWND window = CreateWindowExA(exStyle,
                                      windowClass.lpszClassName,
                                      "Sinus",
                                      style,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      width,
                                      height,
                                      0,
                                      0,
                                      hInstance,
                                      0);
        if(window)
        {
            ShowWindow(window, SW_SHOW);
            SetForegroundWindow(window);
            SetFocus(window);
            
            DEV_BROADCAST_DEVICEINTERFACE_A dbi = {};
            dbi.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE_A);
            dbi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
            dbi.dbcc_classguid = GUID_DEVINTERFACE_HID;
            RegisterDeviceNotificationA(window, (DEV_BROADCAST_HDR *)&dbi, DEVICE_NOTIFY_WINDOW_HANDLE);

            DetectInstructionSets();
            DIInit(window, hInstance);
            
            HDC dc = GetWindowDC(window);
            HGLRC openGLRC = OpenGLInit(window);
           
            work_queue highPriorityQueue = {};
            Win32InitWorkQueue(&highPriorityQueue, 6);
            work_queue lowPriorityQueue = {};
            Win32InitWorkQueue(&lowPriorityQueue, 2);

            memory_index permanentStorageSize = Megabytes(256);
            memory_index transientStorageSize = Megabytes(256);
    
            win32State.memory = VirtualAlloc(0, permanentStorageSize + transientStorageSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    
            memory_store memoryStore = {};
            memoryStore.permanentStorageSize = permanentStorageSize;
            memoryStore.transientStorageSize = transientStorageSize;
            memoryStore.permanentStorage = win32State.memory;
            memoryStore.transientStorage = (u8 *)win32State.memory + permanentStorageSize;
            memoryStore.highPriorityQueue = &highPriorityQueue;
            memoryStore.lowPriorityQueue = &lowPriorityQueue;
            memoryStore.platform.AddWorkQueueEntry = Win32AddWorkQueueEntry;
            memoryStore.platform.CompleteWorkQueue = Win32CompleteWorkQueue;
            memoryStore.platform.GetAllFilesOfTypeBegin = Win32GetAllFilesOfTypeBegin;
            memoryStore.platform.GetAllFilesOfTypeEnd = Win32GetAllFilesOfTypeEnd;
            memoryStore.platform.OpenNextFile = Win32OpenNextFile;
            memoryStore.platform.AddFileError = Win32AddFileError;
            memoryStore.platform.ReadDataFromFile = Win32ReadDataFromFile;

            PollMonitors(window);
            win32State.windowPosition = {sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(window, &win32State.windowPosition);

            win32_game_dll game = Win32LoadGameDll();
            renderer renderer = {};
            RendererInit(&renderer);
            u32 basicShader = CreateShader(basicShaderVertexSource, basicShaderFragmentSource);
            glUseProgram(basicShader);

            running = true;
            while(running)
            {
                FILETIME newDllWriteTime = Win32GetFileLastModified(win32State.gameDllFullPath);
                b32 reloadGameDll = CompareFileTime(&newDllWriteTime, &game.gameDllLastModified) != 0;
                if(reloadGameDll)
                {
                    Win32CompleteWorkQueue(&highPriorityQueue);
                    Win32CompleteWorkQueue(&lowPriorityQueue);

                    Win32UnloadGameDll(&game);
                    for(u32 i = 0; !game.isValid && i < 100; i++)
                    {
                        game = Win32LoadGameDll();
                        Sleep(100);
                    }
                }
                Assert(game.isValid);

                MSG message;
                while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
                {
                    switch(message.message)
                    {
                        case WM_QUIT:
                        {
                            running = false;
                        } break;

                        case WM_SYSKEYDOWN:
                        case WM_SYSKEYUP:
                        case WM_KEYDOWN:
                        case WM_KEYUP:
                        {
                            u32 vkCode = (u32)message.wParam;
                            b32 wasDown = (message.lParam & (1 << 30)) != 0;
                            b32 isDown = (message.lParam & (1 << 31)) == 0;
                            b32 altWasDown = (message.lParam & (1 << 29));

                            if(isDown != wasDown)
                            {
                                if(vkCode == 'F' && isDown)
                                {
                                    win32State.displaySettings.desiredMonitorIndex = 0;//1;
                                    win32State.displaySettings.desiredVideoModeIndex = 22;//0;
                                    ToggleFullscreen(window);
                                }                                
                                if(vkCode == 'O' && isDown)
                                {
                                    win32State.displaySettings.desiredMonitorIndex = 0;//1;
                                    win32State.displaySettings.desiredVideoModeIndex = 17;//31;
                                    ToggleFullscreen(window);
                                }                                
                                if(vkCode == 'V' && isDown)
                                {
                                    SetClientSize(window, 1366, 768);
                                }
                                if(vkCode == 'B' && isDown)
                                {
                                    SetClientSize(window, 960, 540);
                                }
                            }
                            
                            if(vkCode == VK_F4 && altWasDown)
                            {
                                running = false;
                            }
                        } break;
                        
                        default:
                        {
                            TranslateMessage(&message);
                            DispatchMessageA(&message);
                        } break;
                    }
                }

                game.UpdateAndRender(&memoryStore);

                //test renderer
                RendererClearColor({0.2f, 0.3f, 0.8f, 1.0f});
                RendererBegin(&renderer);
                RendererSubmit(&renderer, {0, 0, 0}, {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f});
                RendererEnd();
                RendererFlush(&renderer);
                SwapBuffers(dc);
            }

            DIRelease();
            OpenGLRelease(openGLRC);
            ReleaseDC(window, dc);
            DestroyWindow(window);
            UnregisterClass(windowClass.lpszClassName, hInstance);
        }
        else
        {
            //TODO: Logging - Could not create window
        }
    }
    else
    {
        //TODO: Logging - Could not register window class
    }
    
    return 0;
}

