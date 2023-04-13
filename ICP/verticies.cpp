#include "verticies.h"
#define M_PI       3.14159265358979323846   // pi

void make_triangle(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
    std::vector<vertex> vertices = {
        { { 0.0f, 0.5f, 0.0f}, {1.0f,0.0f,0.0f} }, //vrchol
        { { -0.5f, 0.0f, 0.5f}, {0.0f,1.0f,0.0f} }, //1.spodek
        { { -0.5f, 0.0f, -0.5f}, {0.0f,0.0f,1.0f} }, //2.vrchol
        { { 0.5f, 0.0f, -0.5f}, {1.0f,0.0f,1.0f} },
        { { 0.5f, 0.0f, 0.5f}, {1.0f,1.0f,0.0f} }
        };
    //std::vector<GLuint> indices = { 0,1,2,3,0,2 };
    //std::vector<GLuint> indices = { 0,3,2,3,1,3,4,3,0 };
    std::vector<GLuint> indices = { 0,2,1,  0,1,4,  0,4,3, 0,3,2};
    *indices_ptr = indices;
    *vertex_ptr = vertices;
}

void make_triangle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr){
    std::vector<vertex> vertices = {
        { {-0.5f,-0.5f,0.0f}, {1.0f,0.0f,0.0f} },
        { { 0.5f,-0.5f,0.0f}, {0.0f,1.0f,0.0f} },
        { { 0.0f, 0.5f,0.0f}, {0.0f,0.0f,1.0f} } };
    std::vector<GLuint> indices = { 0,1,2 };
    *indices_ptr = indices;
    *vertex_ptr = vertices;
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

void make_circle(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
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
    *vertex_ptr = vertices;
}

void make_circle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
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
    *vertex_ptr = vertices;
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

void make_checker(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr, size_t number_of_col, size_t number_of_rows) {
    std::vector<vertex> vertices = {};
    float start_x = -1.0f;
    float end_x = 1.0f;
    float start_y = 1.0f;
    float end_y = -1.0f;
    float size_rows = -(start_x - end_x) / number_of_rows;
    float size_col = -(-start_y + end_y) / number_of_col;
    glm::vec3 color;
    vertex vert;
    std::vector<GLuint> indices = {};
    for (size_t col = 0; col < number_of_col; col++) {
        for (size_t rows = 0; rows < number_of_rows; rows++) {
            if (rows % 2 == col % 2) {
                color = { 1.0f,1.0f,1.0f };
            }
            else {
                color = { 0.5f, 0.5f, 0.5f };
            }
            vert = { {start_x + size_rows * rows,start_y - size_col * col,0.0f} ,color };
            vertices.push_back(vert);
            indices.push_back(0 + rows * 4 + col * 4 * number_of_rows);
            vert = { {start_x + size_rows * rows,start_y - size_col - size_col * col,0.0f} ,color };
            vertices.push_back(vert);
            indices.push_back(1 + rows * 4 + col * 4 * number_of_rows);
            vert = { {start_x + size_rows + size_rows * rows,start_y - size_col * col,0.0f} ,color };
            vertices.push_back(vert);
            indices.push_back(2 + rows * 4 + col * 4 * number_of_rows);
            vert = { {start_x + size_rows + size_rows * rows,start_y - size_col - size_col * col,0.0f} ,color };
            vertices.push_back(vert);
            indices.push_back(3 + rows * 4 + col * 4 * number_of_rows);
        }
        indices.push_back(4 * number_of_rows - 1 + col * 4 * number_of_rows);
        indices.push_back(1 + col * 4 * number_of_rows);
    }
    *indices_ptr = indices;
    *vertex_ptr = vertices;
}

void make_checker(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr, size_t number_of_col, size_t number_of_rows){
    std::vector<vertex> vertices = {};
    float start_x = -1.0f;
    float end_x = 1.0f;
    float start_y = 1.0f;
    float end_y = -1.0f;
    float size_rows = -(start_x - end_x)/number_of_rows;
    float size_col = -(-start_y + end_y)/number_of_col;
    glm::vec3 color;
    vertex vert;
    std::vector<GLuint> indices = {};
    for (size_t col = 0; col < number_of_col; col++){
        for (size_t rows = 0; rows < number_of_rows; rows++) {
            if (rows % 2 == col % 2) {
                color = { 1.0f,1.0f,1.0f };
            }
            else {
                color = { 0.5f, 0.5f, 0.5f };
            }
            vert = {{start_x + size_rows*rows,start_y - size_col * col,0.0f} ,color};
            vertices.push_back(vert);
            indices.push_back(0 + rows * 4 + col*4* number_of_rows);
            vert = { {start_x + size_rows * rows,start_y -size_col-size_col*col,0.0f} ,color };
            vertices.push_back(vert);
            indices.push_back(1 + rows * 4 + col * 4* number_of_rows);
            vert = { {start_x + size_rows + size_rows * rows,start_y -size_col * col,0.0f} ,color };
            vertices.push_back(vert);
            indices.push_back(2 + rows * 4 + col * 4* number_of_rows);
            vert = { {start_x + size_rows + size_rows * rows,start_y -size_col - size_col * col,0.0f} ,color };
            vertices.push_back(vert);
            indices.push_back(3 + rows * 4 + col * 4* number_of_rows);
        }
        indices.push_back(4* number_of_rows -1+col * 4*number_of_rows);
        indices.push_back(1 + col * 4*number_of_rows);
    }
    *indices_ptr = indices;
    *vertex_ptr = vertices;
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