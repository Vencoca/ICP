#include "arena.h"
extern s_globals globals;

arena::arena(int size) {
    this->size = size;
    generateObstacles();
}

obstacle::obstacle(int posx, int posy) :posx(posx), posy(posy){};

void arena::generateObstacles() {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, this->size); // define the range
    for (int n = 0; n < size*2; ++n) {
        obstacles.push_back(obstacle(distr(gen), distr(gen)));
    }
}

void arena::drawArena(glm::mat4 v_m) {
    globals.mesh["floor"].draw_repeat(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f), size);
    for (int i = 0; i < (size+1) * 2; i = i + 2) {
        globals.mesh["box"].translate_s(glm::vec3(-2.0f+i, 1.001f, -2.0f));
        globals.mesh["box"].draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
        globals.mesh["box"].translate_s(glm::vec3(-2.0f, 1.001f, -2.0f + i));
        globals.mesh["box"].draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
        globals.mesh["box"].translate_s(glm::vec3(size*2, 1.001f, -2.0f + i));
        globals.mesh["box"].draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
        globals.mesh["box"].translate_s(glm::vec3(-2.0f + i, 1.001f, size * 2));
        globals.mesh["box"].draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
    }
    globals.mesh["box"].translate_s(glm::vec3(size*2, 1.001f, size * 2));
    globals.mesh["box"].draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));

    drawObstacles(v_m);
}

void arena::drawInArena(glm::mat4 v_m, int posx, int posy, std::string mesh_name) {
    if (posx >= 0 && posx < size && posy >= 0 && posy < size) {
        if (auto search = globals.mesh.find(mesh_name); search != globals.mesh.end()) {
            globals.mesh[mesh_name].translate_s(glm::vec3(posx * 2, 1.001f, posy * 2));
            globals.mesh["box"].draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
        }
    }

}

void arena::drawObstacles(glm::mat4 v_m) {
    for (auto const& o : obstacles)
    {
        drawInArena(v_m,o.posx, o.posy, "box");
    }
}

bool arena::checkColisions(glm::vec3 position) {
    float magic_const = 0.1;
    if (position.x < -1+magic_const || position.x > size*2-1-magic_const || position.z < -1 + magic_const || position.z > size*2-1 - magic_const) {
        return true;
    }
    for (auto const& o : obstacles)
    {
        if (position.x + magic_const > (o.posx * 2) - 1 && position.z + magic_const > (o.posy * 2) - 1 && position.z - magic_const < (o.posy * 2) + 1 && position.x - magic_const < (o.posx * 2) + 1) {
            return true;
        }
    }
    return false;
}

void prepare_meshes() {
    std::vector<GLuint> indices_mesh;
    std::vector<vertex> vertex_mesh;
    make_triangle(&indices_mesh, &vertex_mesh);
    mesh mesh_triangle = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    globals.mesh["triangle"] = mesh_triangle;

    mesh mesh_floor = mesh();
    createMesh("resources/floor.obj", globals.shader["texture"], mesh_floor, { 1.0f,1.0f,1.0f });
    mesh_floor.add_texture_id("resources/floor.jpg");
    mesh_floor.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0.0);
    globals.mesh["floor"] = mesh_floor;

    mesh mesh_bunny = mesh();
    createMesh("resources/bunny_tri_vnt.obj", globals.shader["mesh"], mesh_bunny, { 1.0f,0.0f,0.0f });
    globals.mesh["bunny"] = mesh_bunny;

    mesh mesh_teapot = mesh();
    createMesh("resources/teapot_tri_vnt.obj", globals.shader["light"], mesh_teapot, { 0.0f,1.0f,0.0f });
    mesh_teapot.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 30.0);
    globals.mesh["teapot"] = mesh_teapot;

    mesh mesh_box = mesh();
    createMesh("resources/box.obj", globals.shader["texture"], mesh_box, { 0.0f, 0.0f, 0.0f });
    mesh_box.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 30.0);
    mesh_box.add_texture_id("resources/box.png");
    globals.mesh["box"] = mesh_box;
}