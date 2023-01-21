#include "shaders.h"
extern s_globals globals;

void make_shader(std::string vertex_shader, std::string fragment_shader, GLuint* shader) {
    GLuint VS_h, FS_h, prog_h;
    VS_h = glCreateShader(GL_VERTEX_SHADER);
    FS_h = glCreateShader(GL_FRAGMENT_SHADER);
    std::string VSsrc = textFileRead(vertex_shader);
    const char* VS_string = VSsrc.c_str();
    std::string FSsrc = textFileRead(fragment_shader);
    const char* FS_string = FSsrc.c_str();
    glShaderSource(VS_h, 1, &VS_string, NULL);
    glShaderSource(FS_h, 1, &FS_string, NULL);
    glCompileShader(VS_h);
    getShaderInfoLog(VS_h);
    glCompileShader(FS_h);
    getShaderInfoLog(FS_h);
    prog_h = glCreateProgram();
    glAttachShader(prog_h, VS_h);
    glAttachShader(prog_h, FS_h);
    glLinkProgram(prog_h);
    getProgramInfoLog(prog_h);
    *shader = prog_h;
    //glUseProgram(prog_h);
}

void make_shaders() {
    GLuint shader_mesh;
    make_shader("resources/basic.vert", "resources/basic.frag", &shader_mesh);
    GLuint shader_light;
    make_shader("resources/light2.vert", "resources/light.frag", &shader_light);
    GLuint shader_texture;
    make_shader("resources/texture.vert", "resources/texture.frag", &shader_texture);
    globals.shader["mesh"] = shader_mesh;
    globals.shader["light"] = shader_light;
    globals.shader["texture"] = shader_texture;
}

std::string textFileRead(const std::string fn) {
    std::ifstream file;
    std::stringstream ss;
    file.open(fn);
    if (file.is_open()) {
        ss << file.rdbuf();
    }
    else {
        std::cerr << "Error opening file: " << fn << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::move(ss.str());
}
std::string getShaderInfoLog(const GLuint obj) {
    int infologLength = 0;
    std::string s;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0) {
        std::vector<char> v(infologLength);
        glGetShaderInfoLog(obj, infologLength, NULL,
            v.data());
        s.assign(begin(v), end(v));
    }
    return s;
}
std::string getProgramInfoLog(const GLuint obj) {
    int infologLength = 0;
    std::string s;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0) {
        std::vector<char> v(infologLength);
        glGetProgramInfoLog(obj, infologLength, NULL,
            v.data());
        s.assign(begin(v), end(v));
    }
    return s;
}