#if !defined(DIRECTINPUT_H)

global const GUID IID_IDirectInput8W_ =    {0xbf798031,0x483a,0x4da2,{0xaa,0x99,0x5d,0x64,0xed,0x36,0x97,0x00}};
global const GUID IID_IDirectInput8A_ =    {0xbf798030,0x483a,0x4da2,{0xaa,0x99,0x5d,0x64,0xed,0x36,0x97,0x00}};
global const GUID GUID_XAxis_ =            {0xa36d02e0,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_YAxis_ =            {0xa36d02e1,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_ZAxis_ =            {0xa36d02e2,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_RxAxis_ =           {0xa36d02f4,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_RyAxis_ =           {0xa36d02f5,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_RzAxis_ =           {0xa36d02e3,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_Slider_ =           {0xa36d02e4,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_POV_ =              {0xa36d02f2,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_Button_ =           {0xa36d02f0,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_Unknown_ =          {0xa36d02f3,0xc9f3,0x11cf,{0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00}};
global const GUID GUID_DEVINTERFACE_HID_ = {0x4d1e55b2,0xf16f,0x11cf,{0x88,0xcb,0x00,0x11,0x11,0x00,0x00,0x30}};

#define GUID_DEVINTERFACE_HID GUID_DEVINTERFACE_HID_
#define IID_IDirectInput8A IID_IDirectInput8A_
#define GUID_XAxis GUID_XAxis_
#define GUID_YAxis GUID_YAxis_
#define GUID_ZAxis GUID_ZAxis_
#define GUID_RxAxis GUID_RxAxis_
#define GUID_RyAxis GUID_RyAxis_
#define GUID_RzAxis GUID_RzAxis_
#define GUID_Slider GUID_Slider_
#define GUID_POV GUID_POV_
#define GUID_Button GUID_Button_
#define GUID_Unknown GUID_Unknown_
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <xinput.h>

#ifndef DIDFT_OPTIONAL
#define DIDFT_OPTIONAL 0x80000000
#endif

#define GAMEPAD_AXIS 1
#define GAMEPAD_BUTTON 2
#define GAMEPAD_HAT 3
#define HAT_CENTERED    0x00
#define HAT_UP          0x01
#define HAT_RIGHT       0x02
#define HAT_DOWN        0x04
#define HAT_LEFT        0x08
#define HAT_RIGHTUP     (HAT_RIGHT|HAT_UP)
#define HAT_RIGHTDOWN   (HAT_RIGHT|HAT_DOWN)
#define HAT_LEFTUP      (HAT_LEFT|HAT_UP)
#define HAT_LEFTDOWN    (HAT_LEFT|HAT_DOWN)

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 2
#define BUTTON_Y 3
#define BUTTON_LEFT_SHOULDER 4
#define BUTTON_RIGHT_SHOULDER 5
#define BUTTON_BACK 6
#define BUTTON_START 7
#define BUTTON_GUIDE 8
#define BUTTON_LEFT_STICK 9
#define BUTTON_RIGHT_STICK 10
#define BUTTON_DPAD_UP 11
#define BUTTON_DPAD_RIGHT 12
#define BUTTON_DPAD_DOWN 13
#define BUTTON_DPAD_LEFT 14
#define AXIS_LEFT_X 0
#define AXIS_LEFT_Y 1
#define AXIS_RIGHT_X 2
#define AXIS_RIGHT_Y 3
#define AXIS_LEFT_TRIGGER 4
#define AXIS_RIGHT_TRIGGER 5

typedef HRESULT WINAPI direct_input_8_create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);
typedef DWORD WINAPI x_input_get_state(DWORD dwUserIndex, XINPUT_STATE *pState);
typedef DWORD WINAPI x_input_set_state(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);
typedef DWORD WINAPI x_input_get_capabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities);

struct gamepad_mapping_element
{
    u8 type;
    u8 value;
};

struct gamepad_mapping
{
    char name[128];
    char guid[33];
    gamepad_mapping_element axes[6];
    gamepad_mapping_element buttons[15];
};

struct gamepad_object
{
    u32 offset;
    u32 type;
};

struct gamepad_state
{
    r32 axes[16];
    b32 buttons[48];
};

struct gamepad
{
#define GAMEPAD_MAX_OBJECTS 64
#define GAMEPAD_BUFFER_ITEMS 64
    IDirectInputDevice8A *device;
    DWORD index;
    DWORD packetNumber;
    GUID guidInstance;
    u32 objectCount;
    gamepad_object objects[GAMEPAD_MAX_OBJECTS];
    gamepad_mapping *mapping;
    
    char guid[33];
    char name[256];
    u32 axisCount;
    u32 sliderCount;
    u32 buttonCount;
    u32 povCount;
    gamepad_state states[2];
    gamepad_state *prevState;
    gamepad_state *state;
    b32 buffered;
    b32 active;
};

#define DIRECTINPUT_H
#endif
