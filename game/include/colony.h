#ifndef COLONY_H
#define COLONY_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class Colony : public Level
{
public:
    Colony(const string& next = "");
    
private:
    shared_ptr<Texture> m_colony;

    void draw_self();
};

#endif
