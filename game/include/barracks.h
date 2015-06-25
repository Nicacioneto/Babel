/*
 * Class that represents the Barracks
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef BARRACKS_H
#define BARRACKS_H

#include "button.h"
#include <core/level.h>
#include <core/listener.h>
#include <memory>
#include <map>

using std::map;

class Texture;
class Character;
class KeyboardEvent;

class Barracks : public Level, public Listener
{
public:
    Barracks(int slot, const string& next = "base");
    ~Barracks();

private:
    typedef enum { INSPECT, EQUIP } Screen;
    int m_slot, m_character;
    Screen m_screen;

    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, shared_ptr<Texture>> m_textures;

    void draw_self();
    void inspect_screen();
    void equip_screen();
    void draw_character();
    void draw_attributes();
    void draw_skills();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void load_characters();
    Character * current_char() const;
    void update_char_attributes(Character *c, string class_);
    bool on_event(const KeyboardEvent& event);
};

#endif
