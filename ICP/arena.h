#pragma once

#include "mesh.h"
#include <list>
#include <random>
#include "globals.h"
#include "verticies.h"
#include "OBJloader.h"

void prepare_meshes();

class obstacle {
public:
    int posx;
    int posy;

    obstacle();
    obstacle(int posx, int posy);
};

class arena {
public:
    int size;
    std::list<obstacle> obstacles;
    obstacle prize;
    int score;

    arena(int size);

    void drawArena(glm::mat4 v_m);
    bool checkColisions(glm::vec3 position);
    void generateObstacles();
private:
    void drawInArena(glm::mat4 v_m, int posx, int posy, std::string mesh_name, int scale, bool rotate);
    void drawObstacles(glm::mat4 v_m);
    void generatePrize();
};

