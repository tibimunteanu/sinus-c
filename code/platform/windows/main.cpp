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
#include "fileio.cpp"
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
};

#include "monitor.cpp"
#include "opengl.cpp"
#include "input.cpp"

global b32 running;
win32_state win32State = {};

LRESULT CALLBACK MainWindowCallback(HWND window,
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

        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC dc = BeginPaint(window, &paint);
            i32 width, height;
            GetClientSize(window, &width, &height);

            glViewport(0, 0, width, height);
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            SwapBuffers(dc);
            
            EndPaint(window, &paint);
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
    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    windowClass.lpfnWndProc = MainWindowCallback;
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

            running = true;
            while(running)
            {
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

