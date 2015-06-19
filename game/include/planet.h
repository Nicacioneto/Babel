#ifndef PLANET_H
#define PLANET_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class Planet : public Level
{
public:
    Planet(const string& next = "");

private:
    typedef enum { IDLE, POPUP } State;
    shared_ptr<Texture> m_texture, m_popup;
    map<ObjectID, Button*> m_buttons;
    State m_state;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void enable_popup(bool popup);
    void load_buttons();
};

#endif
