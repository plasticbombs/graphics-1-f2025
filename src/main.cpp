//Please work :pray:
#include "Window.h"
#include "Shader.h"
#include "raymath.h"
#include <imgui/imgui.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
using namespace std;

// 4 lines in 1 square -- each line has 2 vertices, therefore 8 array elements because 4 lines * 2 vertices per line = 8
// Hint: if 1 square is 8 vertices, and Assignment 2 requires you to render 8 squares, then 8 squares * 8 vertices per square = 64 vertices;
// (Consider reserving 64 vertices worth of space if you'd like to fit all your positions in a single vertex array)

static const int line_vertex_count = 80;
Vector2 line_vertex_positions[line_vertex_count]
{
    { -0.99f,  -0.99f },   // bottom-left
    {  0.99f,  -0.99f },   // bottom-right

    {  0.99f, -0.99f },   // bottom-right
    {  0.99f,  0.99f },   // top-right

    {   0.99f,  0.99f },   // top-right
    {  -0.99f,  0.99f },   // top-left

    { -0.99f,   0.99f },   // top-left
    { -0.99f,  -0.99f }    // bottom-left
};

int main()
{
    // How to form the vertices for the 2nd square:
    Vector2 line_vertex_positions2[8];

    line_vertex_positions2[0] = Vector2Lerp(line_vertex_positions[0], line_vertex_positions[1], 0.5f);
    line_vertex_positions2[1] = Vector2Lerp(line_vertex_positions[2], line_vertex_positions[3], 0.5f);

    line_vertex_positions2[2] = Vector2Lerp(line_vertex_positions[2], line_vertex_positions[3], 0.5f);
    line_vertex_positions2[3] = Vector2Lerp(line_vertex_positions[4], line_vertex_positions[5], 0.5f);

    line_vertex_positions2[4] = Vector2Lerp(line_vertex_positions[4], line_vertex_positions[5], 0.5f);
    line_vertex_positions2[5] = Vector2Lerp(line_vertex_positions[6], line_vertex_positions[7], 0.5f);
    
    line_vertex_positions2[6] = Vector2Lerp(line_vertex_positions[6], line_vertex_positions[7], 0.5f);
    line_vertex_positions2[7] = Vector2Lerp(line_vertex_positions[0], line_vertex_positions[1], 0.5f);
    // (For full marks, you need to automate this with loops or recursion for 8 iterations [meaning 8 squares])

    Vector2 previousLoop[8];

    int iterator = 1; //Iterator to check if the loop is on it's second iteration
    int currentLoop = 0;
    int maxLoops = 10;
    Vector2 savedPosition = {};
    Vector2 startPosition = {};
    int loopPosition = 0; //Acttual loop position
    int prevPosition = 0;

    int actual = 2;

    while (currentLoop < maxLoops) //Behold, the grand while loop housing two for loops
    {
        //Run a for loop to save the current square
        for (int saveIterator = 0; saveIterator < 8; saveIterator++)
        {
            previousLoop[saveIterator] = line_vertex_positions[loopPosition];
            loopPosition++;
        }

        prevPosition = loopPosition; //Save the position of the start

        //Continue from loop position to create the next square
        for (int prevIterator = 0; prevIterator < 8; prevIterator++)
        {
            //If the loop is at the beginning, create the vertices and then save it as the starting point
            if (prevIterator == 0)
            {
                line_vertex_positions[loopPosition] = Vector2Lerp(previousLoop[prevIterator], previousLoop[prevIterator + 1], 0.5);
                startPosition = line_vertex_positions[loopPosition];
            }
            //If the loop is a multiple of 2 and the loop is not at its end, create the vertices and then save it for the next iteration
            else if (iterator == 2 && prevIterator != 7)
            {
                line_vertex_positions[loopPosition] = Vector2Lerp(previousLoop[actual], previousLoop[actual + 1], 0.5);
                savedPosition = line_vertex_positions[loopPosition];
                iterator = 0;
                actual += 2;
            }
            //If the loop is at its end, create a vertices to the starting point
            else if (prevIterator == 7)
            {
                line_vertex_positions[loopPosition] = startPosition;
                iterator = 0;
            }
            //If all else clears, create a vertices from the saved position
            else
            {
                line_vertex_positions[loopPosition] = savedPosition;
            }
            
            loopPosition++;
            iterator++;
        }
        
        loopPosition = prevPosition; //Go back to the previous position to copy the newly created square
        actual = 2;
        currentLoop++;
    }
    for (int i = 0; i < line_vertex_count; i++)
    {
        cout << line_vertex_positions[i].x << "," << line_vertex_positions[i].y << endl;
    }

    CreateWindow(800, 800, "Graphics 1");
    
    // Hint: The a1_triangle shaders handle vertex position AND vertex colour.
    // Vertex colour is needed in order to receive full marks on this assignment!
    GLuint a2_lines_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a2_lines.vert");
    GLuint a2_lines_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a2_lines.frag");
    GLuint a2_lines_shader = CreateProgram(a2_lines_vert, a2_lines_frag);

    GLuint vbo_line_positions;
    glGenBuffers(1, &vbo_line_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_line_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertex_positions), line_vertex_positions, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertex_positions2), line_vertex_positions2, GL_STATIC_DRAW);
    // Comment/uncomment to see 1st square vs 2nd square.
    // Your job is to automate the generation of squares so all 8 squares are rendered at once!

    GLuint vao_line;
    glGenVertexArrays(1, &vao_line);
    glBindVertexArray(vao_line);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_line_positions);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

    glBindVertexArray(GL_NONE);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    GLint u_color = glGetUniformLocation(a2_lines_shader, "u_color");
    GLint u_mvp = glGetUniformLocation(a2_lines_shader, "u_mvp");

    while (!WindowShouldClose())
    {
        float t = Time();

        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        // Note: change MatrixOrtho's left/right/bottom/top values to modify the extents of your screen!
        Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
        Matrix world = MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(a2_lines_shader);
        glBindVertexArray(vao_line);
        glLineWidth(1.0f);

        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));

        int ee = line_vertex_count / 8;

        for (int i = 0; i < ee; i++)
        {
            // Generate different colors for each square
            float hue = (float)i / ee;  // This spreads the colors evenly
            glUniform3f(u_color, 
                0.5f + 0.5f * sinf(hue * 2.0f * PI), 
                0.5f + 0.5f * sinf((hue + 0.333f) * 2.0f * PI),
                0.5f + 0.5f * sinf((hue + 0.667f) * 2.0f * PI));
            glDrawArrays(GL_LINES, i * 8, 8);  // Draw only 8 vertices (one square) at a time
        }
        //glUniform3f(u_color, 1.0f, 0.0f, 0.0f);
        //glDrawArrays(GL_LINES, 0, line_vertex_count);

        BeginGui();
        //ImGui::ShowDemoWindow(nullptr);
        EndGui();

        Loop();
    }

    glDeleteVertexArrays(1, &vao_line);
    glDeleteBuffers(1, &vbo_line_positions);
    glDeleteProgram(a2_lines_shader);
    glDeleteShader(a2_lines_frag);
    glDeleteShader(a2_lines_vert);

    DestroyWindow();
    return 0;
}
