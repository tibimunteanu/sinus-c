#include "input.h"
#include "dinputmappings.h"

extern win32_state win32State;
global IDirectInput8* directInput;
global b32 xInput;
global x_input_get_state *XInputGetState_;
global x_input_set_state *XInputSetState_;
global x_input_get_capabilities *XInputGetCapabilities_;
#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_
#define XInputGetCapabilities XInputGetCapabilities_

global DIOBJECTDATAFORMAT gamepadObjectDataFormats[] =
{
    { &GUID_XAxis,DIJOFS_X,DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_YAxis,DIJOFS_Y,DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_ZAxis,DIJOFS_Z,DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_RxAxis,DIJOFS_RX,DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_RyAxis,DIJOFS_RY,DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_RzAxis,DIJOFS_RZ,DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_Slider,DIJOFS_SLIDER(0),DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_Slider,DIJOFS_SLIDER(1),DIDFT_AXIS|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,DIDOI_ASPECTPOSITION },
    { &GUID_POV,DIJOFS_POV(0),DIDFT_POV|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { &GUID_POV,DIJOFS_POV(1),DIDFT_POV|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { &GUID_POV,DIJOFS_POV(2),DIDFT_POV|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { &GUID_POV,DIJOFS_POV(3),DIDFT_POV|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(0),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(1),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(2),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(3),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(4),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(5),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(6),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(7),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(8),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(9),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(10),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(11),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(12),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(13),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(14),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(15),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(16),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(17),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(18),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(19),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(20),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(21),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(22),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(23),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(24),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(25),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(26),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(27),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(28),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(29),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(30),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
    { NULL,DIJOFS_BUTTON(31),DIDFT_BUTTON|DIDFT_OPTIONAL|DIDFT_ANYINSTANCE,0 },
};
global const DIDATAFORMAT gamepadDataFormat =
{
    sizeof(DIDATAFORMAT),
    sizeof(DIOBJECTDATAFORMAT),
    DIDFT_ABSAXIS,
    sizeof(DIJOYSTATE),
    sizeof(gamepadObjectDataFormats) / sizeof(DIOBJECTDATAFORMAT),
    gamepadObjectDataFormats
};

internal b32 IsXInputGamepad(const GUID *guid)
{
    static GUID ValveStreamingGamepad = { MAKELONG(0x28DE, 0x11FF), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID X360WiredGamepad = { MAKELONG(0x045E, 0x02A1), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID X360WirelessGamepad = { MAKELONG(0x045E, 0x028E), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID XOneWiredGamepad = { MAKELONG(0x045E, 0x02FF), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID XOneWirelessGamepad = { MAKELONG(0x045E, 0x02DD), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID XOneNewWirelessGamepad = { MAKELONG(0x045E, 0x02D1), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID XOneSWirelessGamepad = { MAKELONG(0x045E, 0x02EA), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID XOneSBluetoothGamepad = { MAKELONG(0x045E, 0x02E0), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };
    static GUID XOneEliteWirelessGamepad = { MAKELONG(0x045E, 0x02E3), 0x0000, 0x0000, { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 } };

    static const GUID *xInputGamepadGuids[] =
    {
        &ValveStreamingGamepad,
        &X360WiredGamepad,
        &X360WirelessGamepad,
        &XOneWiredGamepad,
        &XOneWirelessGamepad,
        &XOneNewWirelessGamepad,
        &XOneSWirelessGamepad,
        &XOneSBluetoothGamepad,
        &XOneEliteWirelessGamepad
    };

    for(memory_index i = 0; i < ArrayCount(xInputGamepadGuids); i++)
    {
        if(memcmp(guid, xInputGamepadGuids[i], sizeof(GUID)) == 0)
        {
            return true;
        }
    }

    b32 result = false;
    u32 count = 0;
    if(GetRawInputDeviceList(NULL, &count, sizeof(RAWINPUTDEVICELIST)) == 0 && count > 0)
    {
        //TODO: use a scratchpad here insead of calloc
        RAWINPUTDEVICELIST *rawInputDeviceList = (RAWINPUTDEVICELIST *)calloc(count, sizeof(RAWINPUTDEVICELIST));
        if(GetRawInputDeviceList(rawInputDeviceList, &count, sizeof(RAWINPUTDEVICELIST)) != -1 && count > 0)
        {
            for(u32 i = 0; i < count; i++)
            {
                RAWINPUTDEVICELIST *device = rawInputDeviceList + i;
                if(device->dwType == RIM_TYPEHID)
                {
                    RID_DEVICE_INFO rdi = {};
                    rdi.cbSize = sizeof(RID_DEVICE_INFO);
                    u32 size = sizeof(RID_DEVICE_INFO);
                    if(GetRawInputDeviceInfoA(device->hDevice, RIDI_DEVICEINFO, &rdi, &size) != -1)
                    {
                        if(MAKELONG(rdi.hid.dwVendorId, rdi.hid.dwProductId) == (long)guid->Data1)
                        {
                            char name[256];
                            memset(name, 0, sizeof(name));
                            size = sizeof(name);
                            if(GetRawInputDeviceInfoA(device->hDevice, RIDI_DEVICENAME, name, &size) != -1)
                            {
                                name[sizeof(name) - 1] = '\0';
                                if(strstr(name, "IG_"))
                                {
                                    result = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        free(rawInputDeviceList);
    }
    
    return result;
}

internal gamepad_mapping *GetMapping(char *guid)
{
    for(u32 i = 0; i < win32State.gamepadMappingCount; i++)
    {
        gamepad_mapping *mapping = win32State.gamepadMappings + i;
        if(StringsAreEqual(mapping->guid, guid))
        {
            return mapping;
        }
    }
    return NULL;
}

internal b32 ParseMapping(gamepad_mapping *mapping, char *mappingString)
{
    // mappingString looks like this:
    //
    // 030000008f0e00001200000000000000,Acme,platform:Windows,x:b2,a:b0,b:b1,y:b3,
    // back:b8,start:b9,dpleft:h0.8,dpdown:h0.4,dpright:h0.2,dpup:h0.1,leftshoulder:b4,
    // lefttrigger:b5,rightshoulder:b6,righttrigger:b7,leftstick:b10,rightstick:b11,
    // leftx:a0,lefty:a1,rightx:a3,righty:a2,
    
    char *c = mappingString;
    struct {char *name; gamepad_mapping_element *element;} fields[] =
    {
        { "platform", NULL },
        { "a", mapping->buttons + BUTTON_A },
        { "b", mapping->buttons + BUTTON_B },
        { "x", mapping->buttons + BUTTON_X },
        { "y", mapping->buttons + BUTTON_Y },
        { "back", mapping->buttons + BUTTON_BACK },
        { "start", mapping->buttons + BUTTON_START },
        { "guide", mapping->buttons + BUTTON_GUIDE },
        { "leftshoulder", mapping->buttons + BUTTON_LEFT_SHOULDER },
        { "rightshoulder", mapping->buttons + BUTTON_RIGHT_SHOULDER },
        { "leftstick", mapping->buttons + BUTTON_LEFT_STICK },
        { "rightstick", mapping->buttons + BUTTON_RIGHT_STICK },
        { "dpup", mapping->buttons + BUTTON_DPAD_UP },
        { "dpright", mapping->buttons + BUTTON_DPAD_RIGHT },
        { "dpdown", mapping->buttons + BUTTON_DPAD_DOWN },
        { "dpleft", mapping->buttons + BUTTON_DPAD_LEFT },
        { "lefttrigger", mapping->axes + AXIS_LEFT_TRIGGER },
        { "righttrigger", mapping->axes + AXIS_RIGHT_TRIGGER },
        { "leftx", mapping->axes + AXIS_LEFT_X },
        { "lefty", mapping->axes + AXIS_LEFT_Y },
        { "rightx", mapping->axes + AXIS_RIGHT_X },
        { "righty", mapping->axes + AXIS_RIGHT_Y },
    };

    memory_index length = StringLengthUntilOneOf(c, ",");
    if(length != 32 || c[length] != ',')
    {
        return false;
    }
    memcpy(mapping->guid, c, length);
    c += length + 1;

    length = StringLengthUntilOneOf(c, ",");
    if(length >= sizeof(mapping->name) || c[length] != ',')
    {
        return false;
    }
    memcpy(mapping->name, c, length);
    c += length + 1;

    while(*c)
    {
        for(u32 i = 0; i < ArrayCount(fields); i++)
        {
            length = StringLength(fields[i].name);
            if(!StringsAreEqual(length, c, fields[i].name) || c[length] != ':')
            {
                continue;
            }
            c += length + 1;

            if(fields[i].element)
            {
            
                if(*c == 'a')
                {
                    fields[i].element->type = GAMEPAD_AXIS;
                    fields[i].element->value = StringParseU8(c + 1, &c);
                }
                else if(*c == 'b')
                {
                    fields[i].element->type = GAMEPAD_BUTTON;
                    fields[i].element->value = StringParseU8(c + 1, &c);
                }
                else if(*c == 'h')
                {
                    fields[i].element->type = GAMEPAD_HAT;
                    u8 hat = StringParseU8(c + 1, &c);
                    u8 bit = StringParseU8(c + 1, &c);
                    fields[i].element->value = (hat << 4) | bit;
                }
                else break;
            }
            else
            {
                length = StringLength("Windows");
                if(!StringsAreEqual(length, c, "Windows"))
                {
                    return false;
                }
            }
        }

        c += StringLengthUntilOneOf(c, ",");
        c += StringLengthOfOnly(c, ",");
    }

    StringMakeLowercase(mapping->guid);

    // NOTE: this is just in case a guid in the mappings list
    // is not processed into the desired format i.e. ends with PIDVID
    if(StringsAreEqual(mapping->guid + 20, "504944564944"))
    {
        char original[33];
        memcpy(original, mapping->guid, StringLength(mapping->guid));
        sprintf_s(mapping->guid, "03000000%.4s0000%.4s000000000000", original, original + 4);
    }
    
    return true;
}

internal void UpdateMappings(char **mappings, u32 mappingCount)
{
    for(u32 l = 0; l < mappingCount; l++)
    {
        gamepad_mapping mapping = {};
        if(ParseMapping(&mapping, *(mappings + l)))
        {
            gamepad_mapping *m = GetMapping(mapping.guid);
            if(!m)
            {
                Assert(win32State.gamepadMappingCount < ArrayCount(win32State.gamepadMappings));
                m = win32State.gamepadMappings + win32State.gamepadMappingCount;                        
                win32State.gamepadMappingCount++;
            }
            memcpy(m->name, mapping.name, sizeof(mapping.name));
            memcpy(m->guid, mapping.guid, sizeof(mapping.guid));
            for(u32 i = 0; i < ArrayCount(mapping.axes); i++)
            {
                m->axes[i] = mapping.axes[i];
            }
            for(u32 i = 0; i < ArrayCount(mapping.buttons); i++)
            {
                m->buttons[i] = mapping.buttons[i];
            }
        }
    }

    for(u32 i = 0; i < win32State.gamepadCount; i++)
    {
        gamepad *g = win32State.gamepads + i;
        if(g->active)
        {
            g->mapping = GetMapping(g->guid);
        }
    }
}

internal i32 CompareDeviceObjects(const void *first, const void *second)
{
    const gamepad_object *o1 = (gamepad_object *)first;
    const gamepad_object *o2 = (gamepad_object *)second;

    if(o1->type != o2->type)
    {
        return o1->type - o2->type;
    }
    return o1->offset - o2->offset;
}

internal BOOL CALLBACK DIEnumDeviceObjectsCallback(const DIDEVICEOBJECTINSTANCE *doi, void *data)
{
    gamepad *g = (gamepad *)data;
    if(g->objectCount >= GAMEPAD_MAX_OBJECTS)
    {
        g->objectCount = GAMEPAD_MAX_OBJECTS;
        return DIENUM_STOP;
    }
    
    gamepad_object *o = g->objects + g->objectCount;

    if(DIDFT_GETTYPE(doi->dwType) & DIDFT_AXIS)
    {
        if(memcmp(&doi->guidType, &GUID_Slider, sizeof(GUID)) == 0)
        {
            o->offset = DIJOFS_SLIDER(g->sliderCount);
            g->sliderCount++;
        }
        else if(memcmp(&doi->guidType, &GUID_XAxis, sizeof(GUID)) == 0)
        {
            o->offset = DIJOFS_X;
        }
        else if(memcmp(&doi->guidType, &GUID_YAxis, sizeof(GUID)) == 0)
        {
            o->offset = DIJOFS_Y;
        }
        else if(memcmp(&doi->guidType, &GUID_ZAxis, sizeof(GUID)) == 0)
        {
            o->offset = DIJOFS_Z;
        }
        else if(memcmp(&doi->guidType, &GUID_RxAxis, sizeof(GUID)) == 0)
        {
            o->offset = DIJOFS_RX;
        }
        else if(memcmp(&doi->guidType, &GUID_RyAxis, sizeof(GUID)) == 0)
        {
            o->offset = DIJOFS_RY;
        }
        else if(memcmp(&doi->guidType, &GUID_RzAxis, sizeof(GUID)) == 0)
        {
            o->offset = DIJOFS_RZ;
        }
        else
        {
            return DIENUM_CONTINUE;
        }

        DIPROPRANGE range = {};
        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(range.diph);
        range.diph.dwObj = doi->dwType;
        range.diph.dwHow = DIPH_BYID;
        range.lMin = -32768;
        range.lMax = 32767;
        if(FAILED(g->device->SetProperty(DIPROP_RANGE, &range.diph)))
        {
            return DIENUM_CONTINUE;
        }
        
        o->type = GAMEPAD_AXIS;
        g->axisCount++;
    }
    else if(DIDFT_GETTYPE(doi->dwType) & DIDFT_BUTTON)
    {
        o->offset = DIJOFS_BUTTON(g->buttonCount);
        o->type = GAMEPAD_BUTTON;
        g->buttonCount++;
    }
    else if(DIDFT_GETTYPE(doi->dwType) & DIDFT_POV)
    {
        o->offset = DIJOFS_POV(g->povCount);
        o->type = GAMEPAD_HAT;
        g->povCount++;
    }

    g->objectCount++;
    return DIENUM_CONTINUE;
}

internal BOOL CALLBACK DIEnumDevicesCallback(const DIDEVICEINSTANCE *di, void *data)
{
    HWND window = (HWND)data;
    
    for(u32 i = 0; i < win32State.gamepadCount; i++)
    {
        gamepad *g = win32State.gamepads + i;
        if(g->active && (memcmp(&g->guidInstance, &di->guidInstance, sizeof(GUID)) == 0))
        {
            return DIENUM_CONTINUE;
        }
    }

    //TODO: Ignore supplemental devices ((dwDevType & 0xff) == DI8DEVTYPE_SUPPLEMENTAL)

    if(IsXInputGamepad(&di->guidProduct))
    {
        return DIENUM_CONTINUE;
    }

    Assert(win32State.gamepadCount < MAX_GAMEPAD_COUNT);
    gamepad *newGamepad = &win32State.gamepads[win32State.gamepadCount];
        
    HRESULT result = directInput->CreateDevice(di->guidInstance, &newGamepad->device, NULL);
    if(SUCCEEDED(result))
    {
        result = newGamepad->device->SetCooperativeLevel(window, DISCL_BACKGROUND|DISCL_EXCLUSIVE);
    }
    if(SUCCEEDED(result))
    {        
        result = newGamepad->device->SetDataFormat(&gamepadDataFormat);
    }
    if(SUCCEEDED(result))
    {
        DIDEVCAPS devCaps = {};
        devCaps.dwSize = sizeof(DIDEVCAPS);
        result = newGamepad->device->GetCapabilities(&devCaps);
    }
    if(SUCCEEDED(result))
    {
        DIPROPDWORD axisMode = {};
        axisMode.diph.dwSize = sizeof(DIPROPDWORD);
        axisMode.diph.dwHeaderSize = sizeof(axisMode.diph);
        axisMode.diph.dwHow = DIPH_DEVICE;
        axisMode.dwData = DIPROPAXISMODE_ABS;
        result = newGamepad->device->SetProperty(DIPROP_AXISMODE, &axisMode.diph);
    }
    if(SUCCEEDED(result))
    {
        DIPROPDWORD bufferSize = {};
        bufferSize.diph.dwSize = sizeof(DIPROPDWORD);
        bufferSize.diph.dwHeaderSize = sizeof(bufferSize.diph);
        bufferSize.diph.dwHow = DIPH_DEVICE;
        bufferSize.dwData = GAMEPAD_BUFFER_ITEMS;
        result = newGamepad->device->SetProperty(DIPROP_BUFFERSIZE, &bufferSize.diph);
        newGamepad->buffered = (result != DI_POLLEDDEVICE);
    }
    if(SUCCEEDED(result))
    {
        result = newGamepad->device->EnumObjects(DIEnumDeviceObjectsCallback, newGamepad, DIDFT_AXIS|DIDFT_BUTTON|DIDFT_POV);
    }
    if(SUCCEEDED(result))
    {                            
        qsort(newGamepad->objects, newGamepad->objectCount, sizeof(gamepad_object), CompareDeviceObjects);
        result = newGamepad->device->Acquire();
    }
    if(SUCCEEDED(result))
    {
        newGamepad->active = true;
        newGamepad->guidInstance = di->guidInstance;
        memcpy(newGamepad->name, di->tszProductName, ArrayCount(newGamepad->name));
        if(memcmp(&di->guidProduct.Data4[2], "PIDVID", 6) == 0)
        {
            sprintf_s(newGamepad->guid, "03000000%02x%02x0000%02x%02x000000000000",
                      (u8)(di->guidProduct.Data1 >> 0), (u8)(di->guidProduct.Data1 >> 8),
                      (u8)(di->guidProduct.Data1 >> 16), (u8)(di->guidProduct.Data1 >> 24));
        }
        else
        {
            sprintf_s(newGamepad->guid, "05000000%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x00",
                      newGamepad->name[0], newGamepad->name[1], newGamepad->name[2], newGamepad->name[3],
                      newGamepad->name[4], newGamepad->name[5], newGamepad->name[6], newGamepad->name[7],
                      newGamepad->name[8], newGamepad->name[9], newGamepad->name[10]);
        }
        newGamepad->mapping = GetMapping(newGamepad->guid);
        win32State.gamepadCount++;
        
        char buffer[512];
        sprintf_s(buffer, "%s connected\n", newGamepad->name);
        OutputDebugStringA(buffer);
    }
    else if(newGamepad->device)
    {
        newGamepad->device->Release();
        newGamepad->device = NULL;
    }

    //TODO: Query device paths for comparison

    return DIENUM_CONTINUE;
}

internal void DIDetectConnectedGamepads(HWND window)
{
    if(xInput)
    {
        for(u32 index = 0; index < XUSER_MAX_COUNT; index++)
        {
            u32 i;
            for(i = 0; i < win32State.gamepadCount; i++)
            {
                gamepad *g = win32State.gamepads + i;
                if(g->active && g->device == NULL && g->index == index)
                {
                    break;
                }
            }
            if(i < win32State.gamepadCount)
            {
                continue;
            }
            XINPUT_CAPABILITIES xic = {};
            if(XInputGetCapabilities(index, 0, &xic) != ERROR_SUCCESS)
            {
                continue;
            }

            Assert(win32State.gamepadCount < MAX_GAMEPAD_COUNT);
            gamepad *newGamepad = &win32State.gamepads[win32State.gamepadCount];
            newGamepad->index = index;
            newGamepad->active = true;
            sprintf_s(newGamepad->name, "XInput Gamepad %d", index);
            sprintf_s(newGamepad->guid, "78696e707574%02x000000000000000000", xic.SubType & 0xff);
            win32State.gamepadCount++;
        
            char buffer[512];
            sprintf_s(buffer, "%s connected\n", newGamepad->name);
            OutputDebugStringA(buffer);
        }
    }
    
    if(directInput)
    {
        directInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
                                 DIEnumDevicesCallback,
                                 (void *)window,
                                 DIEDFL_ALLDEVICES);
    }
}

internal void DIDetectDisconnectedGamepads(void)
{
    char buffer[512];
    
    for(u32 i = 0; i < win32State.gamepadCount; i++)
    {
        gamepad *g = win32State.gamepads + i;
        if(g->active)
        {
            if(g->device)
            {
                g->device->Poll();
                DIJOYSTATE state;
                HRESULT result = g->device->GetDeviceState(sizeof(DIJOYSTATE), &state);
                if(result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST)
                {
                    g->device->Unacquire();
                    g->device->Acquire();
                    g->device->Poll();
                    result = g->device->GetDeviceState(sizeof(DIJOYSTATE), &state);
                }
                if(SUCCEEDED(result))
                {
                    continue;
                }
                g->device->Release();
            }
            else
            {
                XINPUT_CAPABILITIES xic = {};
                if(XInputGetCapabilities(g->index, 0, &xic) == ERROR_SUCCESS)
                {
                    continue;
                }
            }
        }
        sprintf_s(buffer, "%s disconnected\n", g->name);
        OutputDebugStringA(buffer);

        memcpy(g, win32State.gamepads + win32State.gamepadCount - 1, sizeof(gamepad));
        win32State.gamepadCount--;
        i--;
    }
}

internal u8 TranslatePOV(DWORD value)
{
    u8 hatValues[] =
    {
        HAT_UP,
        HAT_UP|HAT_RIGHT,
        HAT_RIGHT,
        HAT_RIGHT|HAT_DOWN,
        HAT_DOWN,
        HAT_DOWN|HAT_LEFT,
        HAT_LEFT,
        HAT_LEFT|HAT_UP
    };

    if(LOWORD(value) == 0xFFFF)
    {
        return HAT_CENTERED;
    }
    value += 4500 / 2;
    value %= 36000;
    value /= 4500;
    if(value >= 8)
    {
        return HAT_CENTERED;
    }
    return hatValues[value];
}

internal void GetGamepadStateBuffered(gamepad *g)
{
    if(g->active && g->device)
    {
        DWORD itemCount = GAMEPAD_BUFFER_ITEMS;
        DIDEVICEOBJECTDATA buffer[GAMEPAD_BUFFER_ITEMS];
        g->device->Poll();
        HRESULT result = g->device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),
                                                  buffer,
                                                  &itemCount,
                                                  0);
        if(result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST)
        {
            itemCount = GAMEPAD_BUFFER_ITEMS;
            g->device->Unacquire();
            g->device->Acquire();
            g->device->Poll();
            result = g->device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),
                                              buffer,
                                              &itemCount,
                                              0);
        }
        
        if(result == DI_BUFFEROVERFLOW)
        {
            Assert(!"Input buffer overflow");
        }
        if(SUCCEEDED(result))
        {
            for(DWORD i = 0; i < itemCount; i++)
            {
                for(u32 o = 0; o < g->objectCount; o++)
                {
                    gamepad_object *object = g->objects + o;
                    if(buffer[i].dwOfs != object->offset)
                    {
                        continue;
                    }
                    switch(object->type)
                    {
                        case GAMEPAD_AXIS:
                        {
                            i16 value = (i16)buffer[i].dwData;
                        } break;

                        case GAMEPAD_BUTTON:
                        {
                            u8 value = (u8)buffer[i].dwData;
                        } break;

                        case GAMEPAD_HAT:
                        {
                            u8 value = TranslatePOV(buffer[i].dwData);
                        } break;
                    }
                }
            }
        }
    }
}

internal void GetGamepadStatePolled(gamepad *g)
{
    DIJOYSTATE state;
    g->device->Poll();
    HRESULT result = g->device->GetDeviceState(sizeof(DIJOYSTATE), &state);
    if(result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST)
    {
        g->device->Unacquire();
        g->device->Acquire();
        g->device->Poll();
        result = g->device->GetDeviceState(sizeof(DIJOYSTATE), &state);
    }

    if(SUCCEEDED(result))
    {
        for(u32 o = 0; o < g->objectCount; o++)
        {
            gamepad_object *object = g->objects + o;
            switch(object->type)
            {
                case GAMEPAD_AXIS:
                {
                    switch(object->offset)
                    {
                        case DIJOFS_X:
                        {
                            i16 value = (i16)state.lX;
                        } break;

                        case DIJOFS_Y:
                        {
                            i16 value = (i16)state.lY;
                        } break;

                        case DIJOFS_Z:
                        {
                            i16 value = (i16)state.lZ;
                        } break;

                        case DIJOFS_RX:
                        {
                            i16 value = (i16)state.lRx;
                        } break;

                        case DIJOFS_RY:
                        {
                            i16 value = (i16)state.lRy;
                        } break;

                        case DIJOFS_RZ:
                        {
                            i16 value = (i16)state.lRz;
                        } break;

                        case DIJOFS_SLIDER(0):
                        {
                            i16 value = (i16)state.rglSlider[0];
                        } break;
                        
                        case DIJOFS_SLIDER(1):
                        {
                            i16 value = (i16)state.rglSlider[1];
                        } break;
                    }
                } break;

                case GAMEPAD_BUTTON:
                {
                    u8 value = (u8)state.rgbButtons[object->offset - DIJOFS_BUTTON0];
                } break;

                case GAMEPAD_HAT:
                {
                    u8 value = TranslatePOV(state.rgdwPOV[object->offset - DIJOFS_POV(0)]);
                } break;
            }
        }
    }
}

internal void GetGamepadStateXInput(gamepad *g)
{
    static WORD buttons[] =
    {
        XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y,
        XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER,
        XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START,
        XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB
    };

    XINPUT_STATE state = {};
    HRESULT result = XInputGetState(g->index, &state);
    if(result == ERROR_DEVICE_NOT_CONNECTED)
    {
        //TODO: Send disconnect request and return neutral state
    }
    else if(result == ERROR_SUCCESS)
    {
        if((state.dwPacketNumber & state.dwPacketNumber) != g->packetNumber)
        {
            g->packetNumber = state.dwPacketNumber;
            
            i16 lx = (i16)state.Gamepad.sThumbLX;
            i16 ly = (i16)(-Max(-32767, state.Gamepad.sThumbLY));
            i16 lt = (i16)(((i32)state.Gamepad.bLeftTrigger * 65535 / 255) - 32768);
            i16 rx = (i16)state.Gamepad.sThumbRX;
            i16 ry = (i16)(-Max(-32767, state.Gamepad.sThumbRY));
            i16 rt = (i16)(((i32)state.Gamepad.bRightTrigger * 65535 / 255) - 32768);

            for(u8 button = 0; button < ArrayCount(buttons); button++)
            {
                b32 value = state.Gamepad.wButtons & buttons[button];
            }

            u8 hat = HAT_CENTERED;
            if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
            {
                hat |= HAT_UP;
            }
            if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
            {
                hat |= HAT_DOWN;
            }
            if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
            {
                hat |= HAT_LEFT;
            }
            if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
            {
                hat |= HAT_RIGHT;
            }
        }
    }
}

internal void GetGamepadState(gamepad *g)
{
    if(g->device)
    {
        if(g->buffered)
        {
            GetGamepadStateBuffered(g);
        }
        else
        {
            GetGamepadStatePolled(g);
        }
    }
    else
    {
        GetGamepadStateXInput(g);
    }
}

internal void DIInit(HWND window, HINSTANCE hInstance)
{
    HMODULE xInputLib = LoadLibraryA("xinput1_4.dll");
    if(!xInputLib)
    {
        xInputLib = LoadLibraryA("xinput1_3.dll");
    }
    if(!xInputLib)
    {
        xInputLib = LoadLibraryA("xinput9_1_0.dll");
    }
    if(xInputLib)
    {
        XInputGetState = (x_input_get_state *)GetProcAddress(xInputLib, "XInputGetState");
        XInputSetState = (x_input_set_state *)GetProcAddress(xInputLib, "XInputSetState");
        XInputGetCapabilities = (x_input_get_capabilities *)GetProcAddress(xInputLib, "XInputGetCapabilities");
        if(XInputGetState && XInputSetState && XInputGetCapabilities)
        {
            xInput = true;
        }
    }
    
    HMODULE dinputLib = LoadLibraryA("dinput8.dll");
    if(dinputLib)
    {
        direct_input_8_create *DirectInput8Create = (direct_input_8_create *)GetProcAddress(dinputLib, "DirectInput8Create");
        if(DirectInput8Create)
        {
            if(SUCCEEDED(DirectInput8Create(hInstance,
                                            DIRECTINPUT_VERSION,
                                            IID_IDirectInput8A,
                                            (void **)&directInput,
                                            NULL)))
            {
                DIDetectConnectedGamepads(window);
                UpdateMappings(defaultGamepadMappings, ArrayCount(defaultGamepadMappings));
            }
            else
            {
                //TODO: Could not create dinput object
            }
        }
        else
        {
            //TODO: Could not load create function
        }
    }
    else
    {
        //TODO: Could not load library
    }
}

internal void DIRelease()
{
    for(u32 i = 0; i < win32State.gamepadCount; i++)
    {
        gamepad *g = win32State.gamepads + i;
        if(g->device)
        {
            g->device->Unacquire();
            g->device->Release();
        }
    }
    IDirectInput8_Release(directInput);
    win32State.gamepadCount = 0;
}

