#include "character.h"
#include <core/environment.h>
#include <core/texture.h>
#include <core/mousebuttonevent.h>
#include <core/rect.h>

Character::Character(Object *parent, ObjectID id, const string& character,
    double x, double y, double w, double h)
    : Object(parent, id, x, y, w, h), m_character(nullptr), m_lvl(1), m_life(100),
        m_attack(10), m_defense(5)
{

    Environment *env = Environment::get_instance();
    env->events_manager->register_listener(this);

    string path = "res/images/characters/";
    m_character = env->resources_manager->get_texture(path + character);
}

Character::~Character()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
Character::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->draw(m_character.get(), x(), y());
}

bool
Character::on_event(const MouseButtonEvent& event)
{
    Environment *env = Environment::get_instance();

    if (event.state() == MouseButtonEvent::PRESSED and
        event.button() == MouseButtonEvent::LEFT and
        bounding_box().contains(event.x(), event.y()))
    {
        notify(id(), "");

        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);

        return true;
    }

    return false;
}

void
Character::receive_damage(double damage)
{
    damage = (damage > m_defense ? damage - m_defense : 0);
    m_life -= damage;
}

int
Character::lvl()
{
    return m_lvl;
}

void
Character::set_lvl(int lvl)
{
    m_lvl = lvl;
}

double
Character::life()
{
    return m_life;
}

void
Character::set_life(double life)
{
    m_life = life;
}

double
Character::attack()
{
    return m_attack;
}

void
Character::set_attack(double attack)
{
    m_attack = attack;
}

double
Character::defense()
{
    return m_defense;
}

void
Character::set_defense(double defense)
{
    m_defense = defense;
}
