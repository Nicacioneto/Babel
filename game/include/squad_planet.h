/*
 * Class that represents the SquadPlanet
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef SQUAD_PLANET_H
#define SQUAD_PLANET_H

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
class TeamPlanet;
class Texture;

class SquadPlanet : public Level, public Listener
{
public:
    typedef enum { TEAM, DRONE } Screen;

    SquadPlanet(int slot, Screen screen = TEAM, const string& next = "base");
    ~SquadPlanet();

private:
    int m_slot;
    Screen m_screen;

    shared_ptr<Settings> m_settings;
    map<ObjectID, Button*> m_buttons;
    shared_ptr<Texture> m_bracket;
    shared_ptr<Texture> m_level;
    shared_ptr<Texture> m_time;
    TeamPlanet *m_team;
    Drone *m_drone;
    string m_level_m;
    string m_level_p;
    string m_level_t;
    string m_timer;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_drone();
    bool on_event(const KeyboardEvent& event);
    void change_screen();
    void confirm_state();
    void start_mission();
    void calculate_level();
    void update_squad();
};

#endif
