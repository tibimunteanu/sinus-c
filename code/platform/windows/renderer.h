#ifndef RENDERER_H
#define RENDERER_H

#define RENDERER_MAX_SPRITES 6000
#define RENDERER_BUFFER_SIZE 4 * sizeof(vertex_data) * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

struct vertex_data
{
    v3 position;
    v4 color;
};

struct renderer
{
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    u32 indexCount;
    vertex_data *buffer;
};

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

internal void RendererSubmit(renderer *renderer, v3 position, v2 size, v4 color)
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

internal void RendererClearColor(v4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

internal void RendererSetViewport(u32 x, u32 y, i32 width, i32 height)
{
    glViewport(x, y, width, height);
}

#endif /* RENDERER_H */
