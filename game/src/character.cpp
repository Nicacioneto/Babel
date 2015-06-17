#include "character.h"
#include <core/environment.h>
#include <core/texture.h>
#include <core/mousebuttonevent.h>
#include <core/rect.h>
#include <cmath>

Character::Character(Object *parent, ObjectID id, const string& character,
    double x, double y, double w, double h)
    : Object(parent, id, x, y, w, h), m_character(nullptr), m_settings(nullptr), m_lvl(1), m_cooldown(1),
        m_attacks_quantity(1), m_life(100), m_attack(10), m_defense(5), m_mind_points(1), m_armor(1)
{

    Environment *env = Environment::get_instance();
    env->events_manager->register_listener(this);

    string path = "res/images/characters/";
    m_character = env->resources_manager->get_texture(path + character);
    m_settings = env->resources_manager->get_settings("res/datas/characters.sav");

    if (!w and !h)
    {
        set_dimensions(m_character->w(), m_character->h());
    }
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
    env->canvas->draw(m_character.get(), x(), y(), w(), h());
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

int
Character::receive_damage(Character *attacker)
{
    double damage = 0;

    if (attacker->attack() > m_defense)
    {
        damage = attacker->attack() - m_defense;
    }
    else
    {
        damage = attacker->attack() * 0.1;
    }

    //TO DO Balance attacks

    int d = round(damage);
    m_life -= d;

    return d;
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

int
Character::cooldown()
{
    return m_cooldown;
}

void
Character::set_cooldown(int cooldown)
{
    m_cooldown = cooldown;
}

double
Character::mind_points()
{
    return m_mind_points;
}

void
Character::set_mind_points(double mind_points)
{
    m_mind_points = mind_points;
}

double
Character::armor()
{
    return m_armor;
}

void
Character::set_armor(double armor)
{
    m_armor = armor;
}

int
Character::attacks_quantity()
{
    return m_attacks_quantity;
}
void
Character::set_attacks_quantity(int attacks_quantity)
{
    m_attacks_quantity = attacks_quantity;
}
