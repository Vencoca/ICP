#pragma once

#include "mesh.h"
#include <list>
#include "globals.h"

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

    void drawArena();
};