/*
 * Class that represents the Tower
 *
 * Author: Tiamat
 * Date: 23/06/2015
 * License: LGPL. No copyright.
 */
#ifndef TOWER_H
#define TOWER_H

#include "button.h"
#include "base.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;

class Tower : public Level
{
public:
    Tower(int slot, const string& next = "");

private:
    int m_slot;
    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
};

#endif
