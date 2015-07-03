/*
 * Class that represents the Team
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef TEAM_H
#define TEAM_H

#include <core/object.h>
#include <map>
#include <vector>

using std::map;
using std::shared_ptr;
using std::vector;

class Button;
class Character;
class Font;
class Settings;
class Texture;

class Team : public Object
{
public:
    Team(int slot, Object *parent = nullptr);

    void confirm();
    void reset();
    void change_buttons(bool state);
    
private:
    int m_slot;
    shared_ptr<Settings> m_settings;
    shared_ptr<Font> m_font;

    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Character*> m_characters;
    vector<ObjectID> m_squad;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void load_characters();
    void load_squad();
    void create_buttons();
};

#endif
