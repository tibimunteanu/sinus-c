#if !defined(MONITOR_H)

#define WS_OVERLAPPEDWINDOW_NORESIZE        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX

struct display_video_mode
{
    u32 width;
    u32 height;
    u32 refreshRate;
    u32 bitsPerPixel;
};

struct display_monitor
{
    HMONITOR handle;
    HWND window;
    char name[128];
    char adapterName[32];
    b32 isPrimary;
    b32 active;
    b32 modesPruned;
    u32 videoModeCount;
    display_video_mode videoModes[64];
};

struct display_settings
{
    u32 desiredMonitorIndex;
    u32 desiredVideoModeIndex;
};

#define MONITOR_H
#endif
