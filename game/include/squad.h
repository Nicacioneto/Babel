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
#include <map>
#include <vector>

using std::map;
using std::vector;

class Character;
class Drone;
class KeyboardEvent;
class Team;
class Texture;

class Squad : public Level, public Listener
{
public:
    typedef enum { TEAM, DRONE } Screen;

    Squad(int slot, Screen screen = TEAM, const string& next = "base");
    ~Squad();

private:
    int m_slot;
    Screen m_screen;

    shared_ptr<Settings> m_settings;
    map<ObjectID, Button*> m_buttons;
    shared_ptr<Texture> m_bracket;
    Team *m_team;
    Drone *m_drone;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_drone();
    bool on_event(const KeyboardEvent& event);
    void change_screen();
    void confirm_state();
};

#endif
