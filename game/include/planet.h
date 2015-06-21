#ifndef PLANET_H
#define PLANET_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class Planet : public Level
{
public:
    Planet(int slot, const string& next = "");

private:
    typedef enum { IDLE, POPUP } State;

    int m_slot;
    string m_text;
    State m_state;
    shared_ptr<Texture> m_texture, m_popup;
    map<ObjectID, Button*> m_buttons;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void enable_popup(bool popup);
    void load_buttons();
    void start_mission();
};

#endif
