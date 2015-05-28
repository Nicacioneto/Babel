#ifndef PLANET_H
#define PLANET_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class Planet : public Level
{
public:
    Planet(const string& next = "",
        const string& texture = "res/images/planet/planet.png");

private:
    shared_ptr<Texture> m_texture;
    Button *m_misson1, *m_misson2, *m_misson3;
    Button *m_colony;

    void draw_self(double x0 = 0, double y0 = 0);
    bool on_message(Object *sender, MessageID id, Parameters p);
};

#endif
