#ifndef COLONY_H
#define COLONY_H

#include "button.h"
#include <core/level.h>
#include <memory>
#include <vector>

using std::vector;

class Texture;

class Colony : public Level
{
public:
    Colony(const string& next = "");

private:
    shared_ptr<Texture> m_colony_cenario;
    shared_ptr<Texture> m_right_bracket;
    shared_ptr<Texture> m_colony;
    shared_ptr<Texture> m_tower_img;
    shared_ptr<Texture> m_planet_img;
    shared_ptr<Texture> m_left_bracket;
    shared_ptr<Texture> m_resources;

    Button *m_center_bracket;
    Button *m_tower;
    Button *m_planet;
    vector<Button *> m_buttons;

    void draw_self(double x0 = 0, double y0 = 0);
    bool on_message(Object *sender, MessageID id, Parameters p);
    void create_buttons();
    void change_to_colony();
    void change_to_hospital();
    void change_to_central();
};

#endif
