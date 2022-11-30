#include "verticies.h"
#define M_PI       3.14159265358979323846   // pi

void make_triangle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr){
    std::vector<vertex> vertices = {
        { {-0.5f,-0.5f,0.0f}, {1.0f,0.0f,0.0f} },
        { { 0.5f,-0.5f,0.0f}, {0.0f,1.0f,0.0f} },
        { { 0.0f, 0.5f,0.0f}, {0.0f,0.0f,1.0f} } };
    std::vector<GLuint> indices = { 0,1,2 };
    *indices_ptr = indices;
    //GL names for Array and Buffers Objects
    GLuint VAO;
    {
        GLuint VBO, EBO;
        // Generate the VAO and VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind VAO (set as the current)
        glBindVertexArray(VAO);
        // Bind the VBO, set type as GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Fill-in data into the VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
        // Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // Fill-in data into the EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        // Set Vertex Attribute to explain OpenGL how to interpret the VBO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, position)));
        // Enable the Vertex Attribute 0 = position
        glEnableVertexAttribArray(0);
        // Set end enable Vertex Attribute 1 = Texture Coordinates
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, color)));
        glEnableVertexAttribArray(1);
        // Bind VBO and VAO to 0 to prevent unintended modification
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    *VAO_ptr = VAO;
}

void make_circle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr) {
    std::vector<vertex> vertices = {
    { {0.0f,0.0f,0.0f}, {1.0f,0.0f,0.0f} },
    };
    std::vector<GLuint> indices = { 0 };
    int n = 30;
    float r = 0.6f;
    for (size_t i = 1; i <= n; i++)
    {
        float x = cos(2 * M_PI / n * i) * r;
        float y = sin(2 * M_PI / n * i) * r;
        vertex vert = { {x,y,0.0f}, {1.0f,0.0f,0.0f} };
        vertices.push_back(vert);
        indices.push_back(i);
    };
    float x = cos(2 * M_PI / n * 1) * r;
    float y = sin(2 * M_PI / n * 1) * r;
    vertex vert = { {x,y,0.0f}, {1.0f,0.0f,0.0f} };
    vertices.push_back(vert);
    indices.push_back(n + 1);
    *indices_ptr = indices;
    GLuint VAO;
    {
        GLuint VBO, EBO;
        // Generate the VAO and VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind VAO (set as the current)
        glBindVertexArray(VAO);
        // Bind the VBO, set type as GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Fill-in data into the VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
        // Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // Fill-in data into the EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        // Set Vertex Attribute to explain OpenGL how to interpret the VBO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, position)));
        // Enable the Vertex Attribute 0 = position
        glEnableVertexAttribArray(0);
        // Set end enable Vertex Attribute 1 = Texture Coordinates
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, color)));
        glEnableVertexAttribArray(1);
        // Bind VBO and VAO to 0 to prevent unintended modification
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    *VAO_ptr = VAO;
}