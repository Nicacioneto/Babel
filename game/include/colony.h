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
    shared_ptr<Texture> m_colony_screen;
    shared_ptr<Texture> m_colony;
    shared_ptr<Texture> m_center_bracket;
    shared_ptr<Texture> m_tower;
    shared_ptr<Texture> m_planet;

    void draw_self();
};

#endif
