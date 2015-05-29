#ifndef BASE_H
#define BASE_H

#include "button.h"
#include <core/level.h>
#include <map>
#include <memory>

using std::map;

class Texture;

class Base : public Level
{
public:
    Base(const string& next = "");

private:
    shared_ptr<Texture> m_scenario;
    
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self(double x0 = 0, double y0 = 0);
    void create_buttons();
};

#endif
