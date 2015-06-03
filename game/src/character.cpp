#include "character.h"
#include <core/environment.h>
#include <core/texture.h>

Character::Character(Object *parent, ObjectID id, const string& texture,
    double x, double y, double w, double h)
    : Object(parent, id, x, y, w, h), m_texture(nullptr), m_lvl(1), m_life(100),
    m_attack(10), m_defense(5)
{
    Environment *env = Environment::get_instance();
    m_texture = env->resources_manager->get_texture(texture);
}

int
Character::lvl()
{
    return m_lvl;
}

double
Character::life()
{
    return m_life;
}

double
Character::attack()
{
    return m_attack;
}

double
Character::defense()
{
    return m_defense;
}
