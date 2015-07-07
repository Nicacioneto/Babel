/*
 * Class that represents the Base
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
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
    Base(int slot, const string& next = "");

private:
    int m_slot;

    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
};

#endif
