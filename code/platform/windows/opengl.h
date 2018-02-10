#if !defined(OPENGL_H)

#define WGL_DRAW_TO_WINDOW_ARB                       0x2001
#define WGL_ACCELERATION_ARB                         0x2003
#define WGL_SWAP_METHOD_ARB                          0x2007
#define WGL_SUPPORT_OPENGL_ARB                       0x2010
#define WGL_DOUBLE_BUFFER_ARB                        0x2011
#define WGL_PIXEL_TYPE_ARB                           0x2013
#define WGL_COLOR_BITS_ARB                           0x2014
#define WGL_ALPHA_BITS_ARB                           0x201B
#define WGL_DEPTH_BITS_ARB                           0x2022
#define WGL_STENCIL_BITS_ARB                         0x2023
#define WGL_SWAP_EXCHANGE_ARB                        0x2028
#define WGL_TYPE_RGBA_ARB                            0x202B
#define WGL_FULL_ACCELERATION_ARB                    0x2027
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB             0x20A9
#define WGL_CONTEXT_MAJOR_VERSION_ARB                0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB                0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB                  0x2093
#define WGL_CONTEXT_FLAGS_ARB                        0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB                    0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB       0x0002
#define WGL_CONTEXT_PROFILE_MASK_ARB                 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB             0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB    0x00000002

#define GL_NUM_EXTENSIONS                            0x821D
#define GL_ARRAY_BUFFER                              0x8892
#define GL_STATIC_DRAW                               0x88E4
#define GL_FRAGMENT_SHADER                           0x8B30
#define GL_VERTEX_SHADER                             0x8B31
#define GL_COMPILE_STATUS                            0x8B81
#define GL_LINK_STATUS                               0x8B82
#define GL_INFO_LOG_LENGTH                           0x8B84
#define GL_TEXTURE0                                  0x84C0
#define GL_BGRA                                      0x80E1
#define GL_ELEMENT_ARRAY_BUFFER                      0x8893
#define GL_SHADING_LANGUAGE_VERSION                  0x8B8C
#define GL_FRAMEBUFFER_SRGB                          0x8DB9
#define GL_SRGB8_ALPHA8                              0x8C43
#define GL_MAJOR_VERSION                             0x821B
#define GL_MINOR_VERSION                             0x821C

typedef HGLRC WINAPI wgl_create_context_attribs_arb(HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL WINAPI wgl_choose_pixel_format_arb(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef const char * WINAPI wgl_get_extensions_string_ext(void);
typedef BOOL WINAPI wgl_swap_interval_ext(int interval);

typedef const GLubyte *gl_get_stringi(GLenum name, GLuint index);
typedef void APIENTRY gl_attach_shader(GLuint program, GLuint shader);
typedef void APIENTRY gl_bind_buffer(GLenum target, GLuint buffer);
typedef void APIENTRY gl_bind_vertex_array(GLuint array);
typedef void APIENTRY gl_buffer_data(GLenum target, ptrdiff_t size, const GLvoid *data, GLenum usage);
typedef void APIENTRY gl_compile_shader(GLuint shader);
typedef GLuint APIENTRY gl_create_program(void);
typedef GLuint APIENTRY gl_create_shader(GLenum type);
typedef void APIENTRY gl_delete_buffers(GLsizei n, const GLuint *buffers);
typedef void APIENTRY gl_delete_program(GLuint program);
typedef void APIENTRY gl_delete_shader(GLuint shader);
typedef void APIENTRY gl_delete_vertex_arrays(GLsizei n, const GLuint *arrays);
typedef void APIENTRY gl_detach_shader(GLuint program, GLuint shader);
typedef void APIENTRY gl_enable_vertex_attrib_array(GLuint index);
typedef void APIENTRY gl_gen_buffers(GLsizei n, GLuint *buffers);
typedef void APIENTRY gl_gen_vertex_arrays(GLsizei n, GLuint *arrays);
typedef GLint APIENTRY gl_get_attrib_location(GLuint program, const char *name);
typedef void APIENTRY gl_get_program_info_log(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
typedef void APIENTRY gl_get_program_iv(GLuint program, GLenum pname, GLint *params);
typedef void APIENTRY gl_get_shader_info_log(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog);
typedef void APIENTRY gl_get_shader_iv(GLuint shader, GLenum pname, GLint *params);
typedef void APIENTRY gl_link_program(GLuint program);
typedef void APIENTRY gl_shader_source(GLuint shader, GLsizei count, const char* *string, const GLint *length);
typedef void APIENTRY gl_use_program(GLuint program);
typedef void APIENTRY gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void APIENTRY gl_bind_attrib_location(GLuint program, GLuint index, const char *name);
typedef GLint APIENTRY gl_get_uniform_location(GLuint program, const char *name);
typedef void APIENTRY gl_uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY gl_active_texture(GLenum texture);
typedef void APIENTRY gl_uniform_1i(GLint location, GLint v0);
typedef void APIENTRY gl_disable_vertex_attrib_array(GLuint index);
typedef void APIENTRY gl_uniform_3fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY gl_uniform_4fv(GLint location, GLsizei count, const GLfloat *value);

global wgl_create_context_attribs_arb *wglCreateContextAttribsARB;
global wgl_choose_pixel_format_arb *wglChoosePixelFormatARB;
global wgl_get_extensions_string_ext *wglGetExtensionsStringEXT;
global wgl_swap_interval_ext *wglSwapIntervalEXT;

global gl_get_stringi *glGetStringi;
global gl_attach_shader *glAttachShader;
global gl_bind_buffer *glBindBuffer;
global gl_bind_vertex_array *glBindVertexArray;
global gl_buffer_data *glBufferData;
global gl_compile_shader *glCompileShader;
global gl_create_program *glCreateProgram;
global gl_create_shader *glCreateShader;
global gl_delete_buffers *glDeleteBuffers;
global gl_delete_program *glDeleteProgram;
global gl_delete_shader *glDeleteShader;
global gl_delete_vertex_arrays *glDeleteVertexArrays;
global gl_detach_shader *glDetachShader;
global gl_enable_vertex_attrib_array *glEnableVertexAttribArray;
global gl_gen_buffers *glGenBuffers;
global gl_gen_vertex_arrays *glGenVertexArrays;
global gl_get_attrib_location *glGetAttribLocation;
global gl_get_program_info_log *glGetProgramInfoLog;
global gl_get_program_iv *glGetProgramiv;
global gl_get_shader_info_log *glGetShaderInfoLog;
global gl_get_shader_iv *glGetShaderiv;
global gl_link_program *glLinkProgram;
global gl_shader_source *glShaderSource;
global gl_use_program *glUseProgram;
global gl_vertex_attrib_pointer *glVertexAttribPointer;
global gl_bind_attrib_location *glBindAttribLocation;
global gl_get_uniform_location *glGetUniformLocation;
global gl_uniform_matrix_4fv *glUniformMatrix4fv;
global gl_active_texture *glActiveTexture;
global gl_uniform_1i *glUniform1i;
global gl_disable_vertex_attrib_array *glDisableVertexAttribArray;
global gl_uniform_3fv *glUniform3fv;
global gl_uniform_4fv *glUniform4fv;

struct gl_extensions
{
    b32 GL_texture_sRGB;
    b32 GL_framebuffer_sRGB;
};

struct wgl_extensions
{
    b32 WGL_framebuffer_sRGB;
};

struct gl_info
{
    char *vendor;
    char *renderer;
    char *shadingLanguageVersion;
    char *version;
    i32 majorVersion;
    i32 minorVersion;
};

#define OPENGL_H
#endif
