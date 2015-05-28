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
    shared_ptr<Texture> m_right_bracket;
    shared_ptr<Texture> m_colony;
    shared_ptr<Texture> m_tower_img;
    shared_ptr<Texture> m_planet_img;
    shared_ptr<Texture> m_left_bracket;
    shared_ptr<Texture> m_resources;

    Button *m_center_bracket;
    Button *m_tower;
    Button *m_planet;
    Button *m_barracks;
    Button *m_research;
    Button *m_hospital;
    Button *m_workshop;
    Button *m_central;

    void draw_self(double x0 = 0, double y0 = 0);
    bool on_message(Object *sender, MessageID id, Parameters p);
    void update_children();
};

#endif
