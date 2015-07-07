/*
 * Class that represents the Workshop
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef WORKSHOP_H
#define WORKSHOP_H

#include "button.h"
#include "base.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;

class Workshop : public Level
{
public:
    Workshop(int slot, const string& next = "");

private:
    typedef enum { CHAT, DRONE, VEHICLE, ARSENAL } Screen;
    
    int m_slot;
    Screen m_screen;
    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
    void change_buttons();
    void change_to_chat();
    void change_to_drone();
    void change_to_vehicle();
    void change_to_arsenal();
};

#endif
