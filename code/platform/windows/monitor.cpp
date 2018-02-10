#include "monitor.h"

extern win32_state win32State;

internal i32 CompareVideoModes(display_video_mode *mode1, display_video_mode *mode2)
{
    if(mode1->bitsPerPixel != mode2->bitsPerPixel)
    {
        return mode1->bitsPerPixel - mode2->bitsPerPixel;
    }

    if(mode1->width != mode2->width)
    {
        return mode1->width - mode2->width;
    }
    
    if(mode1->height != mode2->height)
    {
        return mode1->height - mode2->height;
    }

    return mode1->refreshRate - mode2->refreshRate;
}

internal void GetWindowSize(DWORD style, DWORD exStyle,
                            i32 clientWidth, i32 clientHeight,
                            i32 *width, i32 *height)
{
    RECT rect = {0, 0, clientWidth, clientHeight};
    AdjustWindowRectEx(&rect, style, FALSE, exStyle);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

internal void GetMonitorPosition(display_monitor *monitor, i32 *x, i32 *y)
{
    DEVMODE devMode = {};
    devMode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettingsEx(monitor->adapterName, ENUM_CURRENT_SETTINGS, &devMode, EDS_ROTATEDMODE);
    *x = devMode.dmPosition.x;
    *y = devMode.dmPosition.y;
}

internal void GetCurrentVideoMode(display_monitor *monitor, display_video_mode *mode)
{
    DEVMODE devMode = {};
    devMode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(monitor->adapterName, ENUM_CURRENT_SETTINGS, &devMode);
    mode->width = devMode.dmPelsWidth;
    mode->height = devMode.dmPelsHeight;
    mode->refreshRate = devMode.dmDisplayFrequency;
    mode->bitsPerPixel = devMode.dmBitsPerPel;
}

internal b32 SetVideoMode(display_monitor *monitor, display_video_mode *mode)
{
    if(!mode)
    {
        return ChangeDisplaySettingsEx(monitor->adapterName,
                                       NULL,
                                       NULL,
                                       CDS_FULLSCREEN,
                                       NULL) == DISP_CHANGE_SUCCESSFUL;
    }
    else
    {
        display_video_mode current;
        GetCurrentVideoMode(monitor, &current);
        if(CompareVideoModes(&current, mode) == 0)
        {
            return true;
        }

        DEVMODE devMode = {};
        devMode.dmSize = sizeof(DEVMODE);
        devMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL|DM_DISPLAYFREQUENCY;
        devMode.dmPelsWidth = mode->width;
        devMode.dmPelsHeight = mode->height;
        devMode.dmBitsPerPel = mode->bitsPerPixel;
        devMode.dmDisplayFrequency = mode->refreshRate;
    
        return ChangeDisplaySettingsEx(monitor->adapterName,
                                       &devMode,
                                       NULL,
                                       CDS_FULLSCREEN,
                                       NULL) == DISP_CHANGE_SUCCESSFUL;
    }
}

internal void SetMonitorVideoModes(display_monitor *monitor)
{
    monitor->videoModeCount = 0;

    DEVMODE devMode = {};
    devMode.dmSize = sizeof(DEVMODE);
    for(u32 i = 0;;i++)
    {
        if(!EnumDisplaySettings(monitor->adapterName, i, &devMode))
        {
            break;
        }
        if(devMode.dmBitsPerPel < 15
           || (monitor->modesPruned && ChangeDisplaySettingsEx(monitor->adapterName,
                                                               &devMode,
                                                               NULL,
                                                               CDS_TEST,
                                                               NULL) != DISP_CHANGE_SUCCESSFUL))
        {
            continue;
        }

        display_video_mode *mode = monitor->videoModes + monitor->videoModeCount;
        mode->width = devMode.dmPelsWidth;
        mode->height = devMode.dmPelsHeight;
        mode->refreshRate = devMode.dmDisplayFrequency;
        mode->bitsPerPixel = devMode.dmBitsPerPel;
        
        u32 m;
        for(m = 0; m < monitor->videoModeCount; m++)
        {
            if(CompareVideoModes(mode, monitor->videoModes + m) == 0)
            {
                break;
            }
        }
        if(m == monitor->videoModeCount)
        {
            monitor->videoModeCount++;
        }
    }

    if(!monitor->videoModeCount)
    {
        display_video_mode *mode = monitor->videoModes + monitor->videoModeCount;
        GetCurrentVideoMode(monitor, mode);
        monitor->videoModeCount++;
    }
}

internal BOOL CALLBACK SetMonitorHandle(HMONITOR handle,
                                        HDC dc,
                                        RECT *rect,
                                        LPARAM data)
{
    MONITORINFOEX monitorInfo = {};
    monitorInfo.cbSize = sizeof(monitorInfo);

    if(GetMonitorInfo(handle, (MONITORINFO *)&monitorInfo))
    {
        display_monitor *monitor = (display_monitor *)data;
        if(strcmp(monitorInfo.szDevice, monitor->adapterName) == 0)
        {
            monitor->handle = handle;
        }
    }
    
    return TRUE;
}

internal void SetMonitor(DISPLAY_DEVICE *adapter,
                         DISPLAY_DEVICE *monitor)
{
    char *name = monitor
        ? monitor->DeviceString
        : adapter->DeviceString;

    for(u32 i = 0; i < win32State.monitorCount; i++)
    {
        display_monitor *d = win32State.monitors + i;
        if(strcmp(d->name, name) == 0)
        {
            d->active = true;
            d->isPrimary = adapter->StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE;
            return;
        }
    }
            
    Assert(win32State.monitorCount < MAX_MONITOR_COUNT);
    win32State.monitorCount++;
    display_monitor *newMonitor = &win32State.monitors[win32State.monitorCount - 1];
    memcpy(newMonitor->name, name, ArrayCount(newMonitor->name));
    memcpy(newMonitor->adapterName, adapter->DeviceName, ArrayCount(newMonitor->adapterName));
    newMonitor->active = true;
    newMonitor->isPrimary = adapter->StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE;
    newMonitor->modesPruned = adapter->StateFlags & DISPLAY_DEVICE_MODESPRUNED;

    DEVMODE devMode = {};
    devMode.dmSize = sizeof(devMode);
    EnumDisplaySettings(adapter->DeviceName, ENUM_CURRENT_SETTINGS, &devMode);

    RECT rect = {};
    rect.left = devMode.dmPosition.x;
    rect.top = devMode.dmPosition.y;
    rect.right = devMode.dmPosition.x + devMode.dmPelsWidth;
    rect.bottom = devMode.dmPosition.y + devMode.dmPelsHeight;
    EnumDisplayMonitors(NULL, &rect, SetMonitorHandle, (LPARAM)newMonitor);
}

inline b32 IsFullscreen(HWND window)
{
    DWORD style = GetWindowLong(window, GWL_STYLE);
    return ((style & ~WS_OVERLAPPEDWINDOW_NORESIZE) == style)
        && win32State.currentMonitor;
}

internal void SetFullscreen(HWND window)
{
    GetWindowPlacement(window, &win32State.windowPosition);
            
    display_monitor *monitor = win32State.monitors + win32State.displaySettings.desiredMonitorIndex;
    display_video_mode * videoMode = monitor->videoModes + win32State.displaySettings.desiredVideoModeIndex;
    
    SetVideoMode(monitor, videoMode);
    
    DWORD style = GetWindowLong(window, GWL_STYLE);
    SetWindowLong(window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW_NORESIZE);
    
    display_video_mode currentMode;
    GetCurrentVideoMode(monitor, &currentMode);
    i32 monitorLeft, monitorTop;
    GetMonitorPosition(monitor, &monitorLeft, &monitorTop);
    SetWindowPos(window, HWND_TOPMOST,
                 monitorLeft,
                 monitorTop,
                 currentMode.width,
                 currentMode.height,
                 SWP_NOCOPYBITS|SWP_NOOWNERZORDER|SWP_FRAMECHANGED);

    win32State.currentMonitor = monitor;
    win32State.currentMonitor->window = window;
}

internal void SetWindowedMode(HWND window)
{
    SetVideoMode(win32State.monitors + 0, NULL);
    
    DWORD style = GetWindowLong(window, GWL_STYLE);
    SetWindowLong(window, GWL_STYLE, style|WS_OVERLAPPEDWINDOW_NORESIZE);
    
    SetWindowPlacement(window, &win32State.windowPosition);
    SetWindowPos(window, 0, 0, 0, 0, 0,
                 SWP_NOCOPYBITS|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED);

    if(win32State.currentMonitor)
    {
        win32State.currentMonitor->window = NULL;
        win32State.currentMonitor = NULL;
    }
}

internal void ToggleFullscreen(HWND window)
{
    if(IsFullscreen(window))
    {
        SetWindowedMode(window);
    }
    else
    {
        SetFullscreen(window);
    }
}

internal void GetClientSize(HWND window, i32 *width, i32 *height)
{
    RECT clientRect;
    GetClientRect(window, &clientRect);
    *width = clientRect.right;
    *height = clientRect.bottom;
}

internal void SetClientSize(HWND window, i32 width, i32 height)
{
    DWORD style = GetWindowLong(window, GWL_STYLE)|WS_OVERLAPPEDWINDOW_NORESIZE;
    DWORD exStyle = GetWindowLong(window, GWL_EXSTYLE);
    GetWindowSize(style, exStyle, width, height, &width, &height);
    if(!IsFullscreen(window))
    {
        GetWindowPlacement(window, &win32State.windowPosition);
    }
    win32State.windowPosition.rcNormalPosition.right = win32State.windowPosition.rcNormalPosition.left + width;
    win32State.windowPosition.rcNormalPosition.bottom = win32State.windowPosition.rcNormalPosition.top + height;
    
    SetWindowedMode(window);
}

internal void PollMonitors(HWND window)
{
    OutputDebugStringA("Polling for monitors...\n");
    
    for(u32 i = 0; i < win32State.monitorCount; i++)
    {
        win32State.monitors[i].active = false;
        win32State.monitors[i].isPrimary = false;
    }
    
    DISPLAY_DEVICE adapter;
    for(i32 adapterIndex = 0;;adapterIndex++)
    {
        adapter = {sizeof(DISPLAY_DEVICE)};
        if(!EnumDisplayDevices(NULL, adapterIndex, &adapter, 0))
        {
            break;
        }
        if(!(adapter.StateFlags & DISPLAY_DEVICE_ACTIVE))
        {
            continue;
        }
        
        DISPLAY_DEVICE monitor;
        i32 monitorIndex;
        for(monitorIndex = 0;;monitorIndex++)
        {
            monitor = {sizeof(DISPLAY_DEVICE)};
            if(!EnumDisplayDevices(adapter.DeviceName, monitorIndex, &monitor, 0))
            {
                break;
            }
            if(!(monitor.StateFlags & DISPLAY_DEVICE_ACTIVE))
            {
                continue;
            }

            SetMonitor(&adapter, &monitor);
        }

        // NOTE: sometimes an active adapter does not have any monitors
        // so we create a monitor from it anyways
        if(monitorIndex == 0)
        {
            SetMonitor(&adapter, NULL);
        }        
    }

    char buffer[128];
    
    for(u32 i = 0; i < win32State.monitorCount; i++)
    {
        display_monitor *monitor = &win32State.monitors[i];
        if(!monitor->active)
        {
            sprintf_s(buffer, "%s disconnected!!!\n", monitor->name);
            OutputDebugStringA(buffer);
            
            if(win32State.currentMonitor == monitor)
            {
                ToggleFullscreen(window);
            }
            memcpy(monitor, win32State.monitors + win32State.monitorCount - 1, sizeof(display_monitor));
            win32State.monitorCount--;
            i--;
        }
    }

    for(u32 i = 0; i < win32State.monitorCount; i++)
    {
        display_monitor *monitor = win32State.monitors + i;
        
        sprintf_s(buffer, "%s %s\n", monitor->name, monitor->isPrimary ? "(PRIMARY)" : "");
        OutputDebugStringA(buffer);

        SetMonitorVideoModes(monitor);

        // for(u32 m = 0; m < monitor->videoModeCount; m++)
        // {
        //     display_video_mode *mode = monitor->videoModes + m;

        //     sprintf_s(buffer, "%d x %d %dHz %dbpp\n", mode->width, mode->height, mode->refreshRate, mode->bitsPerPixel); 
        //     OutputDebugStringA(buffer);            
        // }
    }
}

