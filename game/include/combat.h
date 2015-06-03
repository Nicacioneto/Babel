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
    map<ObjectID, Character*> m_characters;

    void draw_self();
    void load_characters();
};

#endif
