#ifndef GLRENDERER_H
#define GLRENDERER_H

#define GL_NUM_EXTENSIONS                            0x821D
#define GL_ARRAY_BUFFER                              0x8892
#define GL_STATIC_DRAW                               0x88E4
#define GL_DYNAMIC_DRAW                              0x88E8
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
#define GL_READ_ONLY                                 0x88B8
#define GL_WRITE_ONLY                                0x88B9

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
typedef void APIENTRY gl_validate_program (GLuint program);
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
typedef void *APIENTRY gl_map_buffer (GLenum target, GLenum access);
typedef GLboolean APIENTRY gl_unmap_buffer (GLenum target);

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
global gl_validate_program *glValidateProgram;
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
global gl_map_buffer *glMapBuffer;
global gl_unmap_buffer *glUnmapBuffer;

struct gl_extensions
{
    b32 GL_texture_sRGB;
    b32 GL_framebuffer_sRGB;
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

#define RENDERER_MAX_SPRITES 6000
#define RENDERER_BUFFER_SIZE 4 * sizeof(vertex_data) * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

struct vertex_data
{
    vec3 position;
    vec4 color;
};

struct renderer
{
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    u32 indexCount;
    vertex_data *buffer;
};

global char basicShaderVertexSource[] = 
"#version 330 core\n"
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec4 color;\n"
"out data\n"
"{\n"
    "vec4 position;\n"
    "vec4 color;\n"
"} vs_out;\n"
"void main()\n"
"{\n"
    "gl_Position = position;\n"
    "vs_out.position = position;\n"
    "vs_out.color = color;\n"
"};\n";

global char basicShaderFragmentSource[] =
"#version 330 core\n"
"layout(location = 0) out vec4 color;\n"
"in data\n"
"{\n"
    "vec4 position;\n"
    "vec4 color;\n"
"} fs_in;\n"
"void main()\n"
"{\n"
    "color = fs_in.color;\n"
"};\n";

internal u32 CompileShader(u32 type, const char *source)
{
    u32 id = glCreateShader(type);
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);

    i32 compileResult;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE)
    {
        i32 length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        OutputDebugStringA(message);
        OutputDebugStringA("\n");
        glDeleteShader(id);
        id = 0;
    }
    return id;
}

internal u32 CreateShader(const char *vertexSource, const char *fragmentSource)
{
    u32 program = glCreateProgram();
    u32 vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
    u32 fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if(!vs || !fs)
    {
        program = 0;
    }
    else
    {
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        i32 linkResult;
        glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
        if(linkResult == GL_FALSE)
        {
            i32 length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            char *message = (char *)alloca(length * sizeof(char));
            glGetProgramInfoLog(program, length, &length, message);
            OutputDebugStringA(message);
            OutputDebugStringA("\n");
            program = 0;
        }
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

internal void RendererInit(renderer *renderer)
{
    renderer->indexCount = 0;
    GLuint indices[RENDERER_INDICES_SIZE];
    for(GLuint i = 0, offset = 0; i < RENDERER_INDICES_SIZE; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }

    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (const void *)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (const void *)(offsetof(vertex_data, color)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &renderer->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICES_SIZE * sizeof(GLuint), &indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
}

internal void RendererBegin(renderer *renderer)
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    renderer->buffer = (vertex_data *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

internal void RendererEnd()
{
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

internal void RendererSubmit(renderer *renderer, vec3 position, vec2 size, vec4 color)
{
    renderer->buffer->position = position;
    renderer->buffer->color = color;
    renderer->buffer++;

    renderer->buffer->position = {position.x, position.y + size.y, position.z};
    renderer->buffer->color = color;
    renderer->buffer++;

    renderer->buffer->position = {position.x + size.x, position.y + size.y, position.z};
    renderer->buffer->color = color;
    renderer->buffer++;

    renderer->buffer->position = {position.x + size.x, position.y, position.z};
    renderer->buffer->color = color;
    renderer->buffer++;

    renderer->indexCount += 6;
}

internal void RendererFlush(renderer *renderer)
{
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
    glDrawElements(GL_TRIANGLES, renderer->indexCount, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    renderer->indexCount = 0;
}

internal void RendererClearColor(vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

internal void RendererSetViewport(u32 x, u32 y, i32 width, i32 height)
{
    glViewport(x, y, width, height);
}

#endif /* GLRENDERER_H */
