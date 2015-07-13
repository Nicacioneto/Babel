/*
 * Class that represents the Combat
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef COMBAT_H
#define COMBAT_H

#include "button.h"
#include "character.h"
#include <core/level.h>
#include <core/listener.h>
#include <map>

using std::multimap;

class Action;
class Character;
class Text;
class Texture;

class Combat : public Level, public Listener
{
public:
    Combat(int slot, const string& next = "");
    ~Combat();

private:
    typedef enum { ENEMY_ATTACK, CHARACTER_ATTACK, SHOW_DAMAGE, FINISHED_COMBAT, EXECUTE } State;

    shared_ptr<Texture> m_texture;
    shared_ptr<Texture> m_actions;
    shared_ptr<Texture> m_result;
    shared_ptr<Texture> m_attacker_icon;
    shared_ptr<Texture> m_turn;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, Character*> m_enemies;
    multimap<int, string> m_attackers;
    pair<string, string> current_action;

    int m_slot;
    string m_attacker;
    State m_state;
    Character *m_enemy_turn;
    Action *m_action;
    Text *m_text;
    unsigned long m_last;
    int m_data;

    void update_self(unsigned long elapsed);
    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void load_team();
    void clear_team(string hero);
    void load_enemies();
    void enemy_attack(Character* enemy);
    void update_attackers(Character* character);
    void set_text(const string& str, const Color& color);
    bool on_event(const KeyboardEvent& event);
    void set_attacker_position(Character *character);
    void set_initial_position();
    void character_message(MessageID id);
    void action_message(MessageID id, Parameters p);
};

#endif
