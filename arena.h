#pragma once

#include "mesh.h"
#include <list>
#include "globals.h"
#include "verticies.h"
#include "OBJloader.h"

void prepare_meshes();

class obstacle {
public:
    int posx;
    int posy;
};

class arena {
public:
    int size;
    std::list<obstacle> obstacles;

    arena(int size);

    void drawArena(glm::mat4 v_m);
};

