#ifndef COMBAT_H
#define COMBAT_H

#include "button.h"
#include <core/level.h>
#include <map>

using std::multimap;

class Texture;
class Character;

class Combat : public Level
{
public:
    typedef enum { ENEMY_ATTACK, CHARACTER_ATTACK } State;
    Combat(const string& next = "", const string& image = "res/images/combat/arena.png");

private:
    shared_ptr<Texture> m_texture;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, Character*> m_enemies;

    string m_attacker;
    multimap<int, string> m_attackers;

    State m_state;

    void update_self(unsigned long);
    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters Parameters);
    void load_characters();
    void load_enemies();
    void enemy_attack(Character* enemy);
    void update_attackers(Character* character);
};

#endif
