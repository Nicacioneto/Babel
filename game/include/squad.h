/*
 * Class that represents the Squad
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef SQUAD_H
#define SQUAD_H

#include "button.h"
#include <core/level.h>
#include <core/listener.h>
#include <memory>
#include <map>

using std::map;

class Texture;
class Character;
class KeyboardEvent;

class Squad : public Level, public Listener
{
public:
    Squad(int slot, const string& next = "base");
    ~Squad();

private:
    int m_slot, m_character;

    shared_ptr<Settings> m_settings;
    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, shared_ptr<Texture>> m_textures;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void load_characters();
    Character * current_char() const;
    bool on_event(const KeyboardEvent& event);
};

#endif
