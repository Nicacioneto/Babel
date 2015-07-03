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
#include <memory>
#include <map>

using std::map;

class Button;
class Texture;

class Tower : public Level
{
public:
    Tower(int slot, const string& next = "");

private:
    int m_slot, m_actual_floor, m_unlocked_floors;
    shared_ptr<Settings> m_settings;

    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Button*> m_floors;

    void load_texture();
    void create_buttons();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
};

#endif
