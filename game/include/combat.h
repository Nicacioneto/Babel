#ifndef COMBAT_H
#define COMBAT_H

#include "button.h"
#include <core/level.h>

class Texture;
class Character;

class Combat : public Level
{
public:
    Combat(const string& next = "combat", const string& image = "res/images/combat/arena.png");

private:
    shared_ptr<Texture> m_texture;
    int m_attacker;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, Character*> m_enemies;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters Parameters);
    void load_characters();
    void load_enemies();
};

#endif
