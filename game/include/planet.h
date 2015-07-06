/*
 * Class that represents the Planet
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
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
    typedef string PlaceID;

    int m_slot;
    PlaceID m_place;
    State m_state;
    unsigned long m_last;
    shared_ptr<Settings> m_settings;

    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button*> m_buttons;

    void update_self(unsigned long elapsed);
    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void enable_popup(bool popup);
    void create_buttons();
    void start_mission();
};

#endif
