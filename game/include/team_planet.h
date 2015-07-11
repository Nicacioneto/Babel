/*
 * Class that represents the Team plantet
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
class Settings;
class Text;
class Texture;

class TeamPlanet : public Object
{
public:
    TeamPlanet(int slot, Object *parent = nullptr);

    void confirm();
    void reset();
    void change_buttons(bool state);
    unsigned int size();
    void update_available();
    vector<ObjectID> m_team;
    
private:
    int m_slot;
    shared_ptr<Settings> m_settings;

    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Character*> m_characters;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void load_characters();
    void load_team();
};

#endif
