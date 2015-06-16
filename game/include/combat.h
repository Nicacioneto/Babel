#ifndef COMBAT_H
#define COMBAT_H

#include "button.h"
#include <core/level.h>
#include <map>
#include <iostream>

using std::multimap;
using std::pair;

class Texture;
class Character;

class Combat : public Level
{
public:
    typedef enum { ENEMY_ATTACK, CHARACTER_ATTACK, SHOW_DAMAGE } State;
    Combat(const string& next = "", const string& image = "res/images/combat/arena.png");

private:
    shared_ptr<Texture> m_texture;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, Character*> m_enemies;

    string m_attacker;
    multimap<int, string> m_attackers;

    State m_state;
    unsigned long m_last;
    int m_damage;
    pair<double, double> m_receiver;

    void update_self(unsigned long elapsed);
    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters Parameters);
    void load_characters();
    void load_enemies();
    void enemy_attack(Character* enemy);
    void update_attackers(Character* character);
    void receiver(double x, double y);
};

#endif
