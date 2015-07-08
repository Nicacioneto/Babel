/*
 * Class that represents the Tower
 *
 * Author: Tiamat
 * Date: 23/06/2015
 * License: LGPL. No copyright.
 */
#ifndef TOWER_H
#define TOWER_H

#include "base.h"
#include <core/level.h>
#include <core/listener.h>
#include <memory>
#include <map>

using std::map;

class Button;
class Texture;

class Tower : public Level, public Listener
{
public:
    Tower(int slot, const string& next = "");
    ~Tower();

private:
    int m_slot, m_actual_floor, m_unlocked_floors;
    shared_ptr<Settings> m_settings;
    pair<string, string> m_drone;

    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Button*> m_floors;

    void load_texture();
    void create_buttons();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    bool on_event(const KeyboardEvent& event);
};

#endif
