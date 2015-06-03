#ifndef CHARACTER_H
#define CHARACTER_H

#include "button.h"
#include <core/object.h>

class Texture;

class Character : public Object
{
public:
    Character(Object *parent = nullptr, ObjectID id = "", const string& texture = "",
        double x = 0, double y = 0, double w = 100, double h = 100);

    int lvl();
    double life();
    double attack();
    double defense();

private:
    shared_ptr<Texture> m_texture;

    int m_lvl;
    double m_life, m_attack, m_defense;
};

#endif
