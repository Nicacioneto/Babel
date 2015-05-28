#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "button.h"
#include "base.h"
#include <core/level.h>
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
    
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self(double x0 = 0, double y0 = 0);
    void create_buttons();
    void change_buttons();
};

#endif
