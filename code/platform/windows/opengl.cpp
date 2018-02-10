#include "opengl.h"

internal void SetPixelFormat(HDC dc, b32 sRGBCapable)
{
    i32 pixelFormatIndex = 0;
    GLuint extendedPick = 0;

    if(wglChoosePixelFormatARB)
    {
        i32 attribs[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, sRGBCapable ? GL_TRUE : GL_FALSE,
            0
        };
        
        wglChoosePixelFormatARB(dc, attribs, {0}, 1, &pixelFormatIndex, &extendedPick);
    }

    if(!extendedPick)
    {
        PIXELFORMATDESCRIPTOR desiredPixelformat = {};
        desiredPixelformat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        desiredPixelformat.nVersion = 1;
        desiredPixelformat.iPixelType = PFD_TYPE_RGBA;
        desiredPixelformat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
        desiredPixelformat.cColorBits = 32;
        desiredPixelformat.cAlphaBits = 8;
        desiredPixelformat.iLayerType = PFD_MAIN_PLANE;

        pixelFormatIndex = ChoosePixelFormat(dc, &desiredPixelformat);
    }
    
    PIXELFORMATDESCRIPTOR pixelFormat;
    DescribePixelFormat(dc, pixelFormatIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormat);
    SetPixelFormat(dc, pixelFormatIndex, &pixelFormat);
}

internal wgl_extensions LoadWGLExtensions()
{
    wgl_extensions wglExtensions = {};
    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = DefWindowProc;
    windowClass.hInstance = GetModuleHandle(0);
    windowClass.lpszClassName = "WGLLoader";
    if(RegisterClassEx(&windowClass))
    {
        HWND window = CreateWindowEx(0,
                                     windowClass.lpszClassName,
                                     "SinusWGL",
                                     0,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     0,
                                     0,
                                     windowClass.hInstance,
                                     0);

        if(window)
        {
            HDC dc = GetDC(window);
            SetPixelFormat(dc, FALSE);
            HGLRC openGLRC = wglCreateContext(dc);
            if(wglMakeCurrent(dc, openGLRC))
            {
                wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb *)wglGetProcAddress("wglChoosePixelFormatARB");
                wglCreateContextAttribsARB = (wgl_create_context_attribs_arb *)wglGetProcAddress("wglCreateContextAttribsARB");
                wglSwapIntervalEXT = (wgl_swap_interval_ext *)wglGetProcAddress("wglSwapIntervalEXT");
                wglGetExtensionsStringEXT = (wgl_get_extensions_string_ext *)wglGetProcAddress("wglGetExtensionsStringEXT");
                
                if(wglGetExtensionsStringEXT)
                {
                    char *extensionsString = (char *)wglGetExtensionsStringEXT();
                    if(extensionsString)
                    {
                        char *start = extensionsString;
                        while(*start)
                        {
                            while(IsWhitespace(*start)) {start++;}
                            char *end = start;
                            while(!IsWhitespace(*end) && *end) {end++;}
                            memory_index length = end - start;
            
                            if(StringsAreEqual(length, start, "WGL_EXT_framebuffer_sRGB")) {wglExtensions.WGL_framebuffer_sRGB = true;}                    

                            start = end;
                        }
                    }
                }

                wglMakeCurrent(NULL, NULL);
            }

            wglDeleteContext(openGLRC);
            ReleaseDC(window, dc);
            DestroyWindow(window);
        }
    }
    return wglExtensions;
}

internal gl_extensions LoadGLExtensions(i32 majorVersion)
{
    glGetStringi = (gl_get_stringi *)wglGetProcAddress("glGetStringi");
    glAttachShader = (gl_attach_shader *)wglGetProcAddress("glAttachShader");
    glBindBuffer = (gl_bind_buffer *)wglGetProcAddress("glBindBuffer");
    glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");
    glBufferData = (gl_buffer_data *)wglGetProcAddress("glBufferData");
    glCompileShader = (gl_compile_shader *)wglGetProcAddress("glCompileShader");
    glCreateProgram = (gl_create_program *)wglGetProcAddress("glCreateProgram");
    glCreateShader = (gl_create_shader *)wglGetProcAddress("glCreateShader");
    glDeleteBuffers = (gl_delete_buffers *)wglGetProcAddress("glDeleteBuffers");
    glDeleteProgram = (gl_delete_program *)wglGetProcAddress("glDeleteProgram");
    glDeleteShader = (gl_delete_shader *)wglGetProcAddress("glDeleteShader");
    glDeleteVertexArrays = (gl_delete_vertex_arrays *)wglGetProcAddress("glDeleteVertexArrays");
    glDetachShader = (gl_detach_shader *)wglGetProcAddress("glDetachShader");
    glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)wglGetProcAddress("glEnableVertexAttribArray");
    glGenBuffers = (gl_gen_buffers *)wglGetProcAddress("glGenBuffers");
    glGenVertexArrays = (gl_gen_vertex_arrays *)wglGetProcAddress("glGenVertexArrays");
    glGetAttribLocation = (gl_get_attrib_location *)wglGetProcAddress("glGetAttribLocation");
    glGetProgramInfoLog = (gl_get_program_info_log *)wglGetProcAddress("glGetProgramInfoLog");
    glGetProgramiv = (gl_get_program_iv *)wglGetProcAddress("glGetProgramiv");
    glGetShaderInfoLog = (gl_get_shader_info_log *)wglGetProcAddress("glGetShaderInfoLog");
    glGetShaderiv = (gl_get_shader_iv *)wglGetProcAddress("glGetShaderiv");
    glLinkProgram = (gl_link_program *)wglGetProcAddress("glLinkProgram");
    glShaderSource = (gl_shader_source *)wglGetProcAddress("glShaderSource");
    glUseProgram = (gl_use_program *)wglGetProcAddress("glUseProgram");
    glVertexAttribPointer = (gl_vertex_attrib_pointer *)wglGetProcAddress("glVertexAttribPointer");
    glBindAttribLocation = (gl_bind_attrib_location *)wglGetProcAddress("glBindAttribLocation");
    glGetUniformLocation = (gl_get_uniform_location *)wglGetProcAddress("glGetUniformLocation");
    glUniformMatrix4fv = (gl_uniform_matrix_4fv *)wglGetProcAddress("glUniformMatrix4fv");
    glActiveTexture = (gl_active_texture *)wglGetProcAddress("glActiveTexture");
    glUniform1i = (gl_uniform_1i *)wglGetProcAddress("glUniform1i");
    glDisableVertexAttribArray = (gl_disable_vertex_attrib_array *)wglGetProcAddress("glDisableVertexAttribArray");
    glUniform3fv = (gl_uniform_3fv *)wglGetProcAddress("glUniform3fv");
    glUniform4fv = (gl_uniform_4fv *)wglGetProcAddress("glUniform4fv");

    gl_extensions glExtensions = {};
    if(majorVersion >= 3)
    {
        i32 extensionsCount = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsCount);
        for(i32 i = 0; i < extensionsCount; i++)
        {
            char *extension = (char *)glGetStringi(GL_EXTENSIONS, i);

            if(StringsAreEqual(extension, "GL_EXT_texture_sRGB")) {glExtensions.GL_texture_sRGB = true;}
            if(StringsAreEqual(extension, "GL_EXT_framebuffer_sRGB")) {glExtensions.GL_framebuffer_sRGB = true;}
            if(StringsAreEqual(extension, "GL_ARB_framebuffer_sRGB")) {glExtensions.GL_framebuffer_sRGB = true;}
        }
    }
    else
    {
        char *extensionsString = (char *)glGetString(GL_EXTENSIONS);
        if(extensionsString)
        {
            char *start = extensionsString;
            while(*start)
            {
                while(IsWhitespace(*start)) {start++;}
                char *end = start;
                while(!IsWhitespace(*end) && *end) {end++;}
                memory_index length = end - start;
            
                if(StringsAreEqual(length, start, "GL_EXT_texture_sRGB")) {glExtensions.GL_texture_sRGB = true;}
                if(StringsAreEqual(length, start, "GL_EXT_framebuffer_sRGB")) {glExtensions.GL_framebuffer_sRGB = true;}                    
                if(StringsAreEqual(length, start, "GL_ARB_framebuffer_sRGB")) {glExtensions.GL_framebuffer_sRGB = true;}                    

                start = end;
            }
        }
    }
    return glExtensions;
}

inline char *OpenGLGetVersion(i32 *major, i32 *minor)
{
    char *version = (char *)glGetString(GL_VERSION);
    if(version && (major || minor))
    {
        u32 dotIndex = 0;
        while(version[dotIndex] && (version[dotIndex] != '.')) {dotIndex++;}
        if(dotIndex != 0)
        {
            if(major)
            {
                *major = version[dotIndex - 1] - '0';
                if(*major < 1) *major = 1;
            }
            if(minor)
            {
                *minor = version[dotIndex + 1] - '0';
                if(*major == 1 && (*minor < 0 || *minor > 5)) *minor = 0;
            }
        }
        return version;
    }
    return 0;
}

internal gl_info GetGLInfo(void)
{
    gl_info info = {};
    info.vendor = (char *)glGetString(GL_VENDOR);
    info.renderer = (char *)glGetString(GL_RENDERER);
    info.version = OpenGLGetVersion(&info.majorVersion, &info.minorVersion);
    info.shadingLanguageVersion = info.majorVersion >= 2
        ? (char *)glGetString(GL_SHADING_LANGUAGE_VERSION)
        : "(none)";
    
    return info;
}

internal HGLRC OpenGLInit(HWND window)
{
    HDC dc = GetDC(window);

    wgl_extensions wglExtensions = LoadWGLExtensions();
    
    SetPixelFormat(dc, wglExtensions.WGL_framebuffer_sRGB);
    
    HGLRC openGLRC = wglCreateContext(dc);
    if(wglMakeCurrent(dc, openGLRC))
    {
        if(wglCreateContextAttribsARB)
        {
            i32 major = 1;
            i32 minor = 0;
            OpenGLGetVersion(&major, &minor);
            int attribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, major,
                WGL_CONTEXT_MINOR_VERSION_ARB, minor,
                WGL_CONTEXT_FLAGS_ARB, 0, //WGL_CONTEXT_DEBUG_BIT_ARB,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,//WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0
            };

            HGLRC modernGLRC = wglCreateContextAttribsARB(dc, 0, attribs);
            if(wglMakeCurrent(dc, modernGLRC))
            {
                wglDeleteContext(openGLRC);
                openGLRC = modernGLRC;
            }
        }
 
        gl_info info = GetGLInfo();
        
        gl_extensions glExtensions = LoadGLExtensions(info.majorVersion);
  
        char buffer[128];
        sprintf_s(buffer, "OpenGL context: %s\nShading language: %s\n", info.version, info.shadingLanguageVersion);
        OutputDebugStringA(buffer);
       
        GLuint textureFormat = GL_RGBA8;
        if(glExtensions.GL_texture_sRGB
           && glExtensions.GL_framebuffer_sRGB
           && wglExtensions.WGL_framebuffer_sRGB)
        {
            textureFormat = GL_SRGB8_ALPHA8;
            glEnable(GL_FRAMEBUFFER_SRGB);
        }
           
        if(wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(1);
        }
    }
    else
    {
        //TODO: Diagnostic
        return 0;
    }

    ReleaseDC(window, dc);
    return openGLRC;
}

internal void OpenGLRelease(HGLRC openGLRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(openGLRC);
}

