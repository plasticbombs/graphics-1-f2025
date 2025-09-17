#include "Window.h"
#include <glad/glad.h>
#include <cstddef>

struct vec2
{
    float x;    // x is 4 bytes
    float y;    // y is 4 bytes
};

struct vec3
{
    float x;
    float y;
    float z;
};

struct Vertex
{
    vec2 pos;   // offset of 0
    vec3 col;   // offset of 8 (4 bytes for pos.x + 4 bytes for pos.y = 8)
};

// Assignment 1 object 1 -- white triangle (change these vertex colours from red to white)!!!
static const Vertex vertices_white[3] =
{
    { { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
    { {  0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
    { {   0.f,  0.6f }, { 1.0f, 0.0f, 0.0f } }
};
  
// Assignment 1 object 2 -- rainbow triangle (done for you)
static const Vertex vertices_rainbow[3] =
{
    { { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
    { {  0.6f, -0.4f }, { 0.0f, 1.0f, 0.0f } },
    { {   0.f,  0.6f }, { 0.0f, 0.0f, 1.0f } }
};

static const char* vertex_shader_text =
"#version 330\n"
"layout (location = 0) in vec2 vPos;\n"
"layout (location = 1) in vec3 vCol;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"    fragColor = vec4(color, 1.0);\n"
"}\n";

int main()
{
    CreateWindow(800, 800, "Graphics 1");

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLuint vertex_buffer_rainbow;
    glGenBuffers(1, &vertex_buffer_rainbow);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rainbow), vertices_rainbow, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    // Can only have 1 vertex buffer bound at a time, so must unbind in order to prevent overwriting it

    GLuint vertex_buffer_white;
    glGenBuffers(1, &vertex_buffer_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_white), vertices_white, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    GLuint vertex_array_rainbow;
    glGenVertexArrays(1, &vertex_array_rainbow);
    glBindVertexArray(vertex_array_rainbow);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_rainbow);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

    glBindVertexArray(GL_NONE);

    GLuint vertex_array_white;
    glGenVertexArrays(1, &vertex_array_white);
    glBindVertexArray(vertex_array_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

    glBindVertexArray(GL_NONE);

    int object_index = 0;

    /* Loop until the user closes the window */
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        // Colors are represented as fractions between 0.0 and 1.0, so convert using a colour-picker tool accordingly!
        float r = 239.0f / 255.0f;
        float g = 136.0f / 255.0f;
        float b = 190.0f / 255.0f;
        float a = 1.0f;

        /* Render here */
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        if (IsKeyPressed(KEY_SPACE))
        {
            ++object_index %= 5;
        }

        switch (object_index)
        {
        case 0:
            glUseProgram(program);
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 1:
            glUseProgram(program);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 2:
            glUseProgram(program);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 3:
            glUseProgram(program);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 4:
            glUseProgram(program);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        }

        // Called at end of the frame to swap buffers and update input
        Loop();
    }

    DestroyWindow();
    return 0;
}
