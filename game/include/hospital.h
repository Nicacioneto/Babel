#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "button.h"
#include "base.h"
#include <memory>
#include <map>

using std::map;

class Texture;

class Hospital : public Level
{
public:
    Hospital(const string& next = "");

private:
    shared_ptr<Texture> m_scenario;
    shared_ptr<Texture> m_right_bracket;
    shared_ptr<Texture> m_colony;
    shared_ptr<Texture> m_tower_img;
    shared_ptr<Texture> m_planet_img;
    shared_ptr<Texture> m_left_bracket;
    shared_ptr<Texture> m_resources;

    Button *m_center_bracket;
    Button *m_tower;
    Button *m_planet;
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self(double x0 = 0, double y0 = 0);
    void create_buttons();
};

#endif
