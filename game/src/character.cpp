#include "character.h"
#include <core/environment.h>
#include <core/texture.h>
#include <core/mousebuttonevent.h>
#include <core/rect.h>
#include <core/settings.h>
#include <cmath>

Character::Character(Object *parent, ObjectID id, const string& texture,
    double x, double y, double w, double h, const string& name)
    : Object(parent, id, x, y, w, h), m_texture(nullptr), m_settings(nullptr),
        m_name(name), m_attacks_quantity(1)
{
    Environment *env = Environment::get_instance();
    env->events_manager->register_listener(this);

    string path = "res/images/characters/";
    m_texture = env->resources_manager->get_texture(path + texture);

    if (not w and not h)
    {
        set_dimensions(m_texture->w(), m_texture->h());
    }

    m_settings = env->resources_manager->get_settings("res/datas/characters.sav");
    init();
}

Character::~Character()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
Character::init()
{
    m_military = m_settings->read<int>(m_name, "military", 0);
    m_psionic = m_settings->read<int>(m_name, "psionic", 0);
    m_tech = m_settings->read<int>(m_name, "tech", 0);
    m_levelup_m = m_settings->read<int>(m_name, "levelup_m", 0);
    m_levelup_p = m_settings->read<int>(m_name, "levelup_p", 0);
    m_levelup_t = m_settings->read<int>(m_name, "levelup_t", 0);
    m_shield = m_settings->read<int>(m_name, "shield", 0);
    m_max_shield = m_settings->read<int>(m_name, "max_shield", 0);
    m_life = m_settings->read<int>(m_name, "life", 0);
    m_max_life = m_settings->read<int>(m_name, "max_life", 0);
    m_mp = m_settings->read<int>(m_name, "mp", 0);
    m_max_mp = m_settings->read<int>(m_name, "max_mp", 0);
    m_might = m_settings->read<int>(m_name, "might", 0);
    m_mind = m_settings->read<int>(m_name, "mind", 0);
    m_resilience = m_settings->read<int>(m_name, "resilience", 0);
    m_willpower = m_settings->read<int>(m_name, "willpower", 0);
    m_agility = m_settings->read<int>(m_name, "agility", 0);
    m_perception = m_settings->read<int>(m_name, "perception", 0);
    m_might_attack = m_settings->read<int>(m_name, "might_attack", 0);
    m_mind_attack = m_settings->read<int>(m_name, "mind_attack", 0);
    m_cooldown = m_settings->read<int>(m_name, "cooldown", 0);
    m_defense = m_settings->read<int>(m_name, "defense", 0);
    m_might_armor = m_settings->read<int>(m_name, "might_armor", 0);
    m_mind_armor = m_settings->read<int>(m_name, "mind_armor", 0);
    m_critical = m_settings->read<int>(m_name, "critical", 0);
}

void
Character::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->draw(m_texture.get(), x(), y(), w(), h());
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

    if (attacker->might_attack() > m_defense)
    {
        damage = attacker->might_attack() - m_defense;
    }
    else
    {
        damage = attacker->might_attack() * 0.1;
    }

    // TODO Balance attacks

    int d = round(damage);
    m_life -= d;

    return d;
}

int
Character::attacks_quantity() const
{
    return m_attacks_quantity;
}

int
Character::military() const
{
    return m_military;
}

int
Character::psionic() const
{
    return m_psionic;
}

int
Character::tech() const
{
    return m_tech;
}

int
Character::might() const
{
    return m_might;
}

int
Character::levelup_m() const
{
    return m_levelup_m;
}

int
Character::levelup_p() const
{
    return m_levelup_p;
}

int
Character::levelup_t() const
{
    return m_levelup_t;
}

int
Character::shield() const
{
    return m_shield;
}

int
Character::max_shield() const
{
    return m_max_shield;
}

int
Character::life() const
{
    return m_life;
}

int
Character::max_life() const
{
    return m_max_life;
}

int
Character::mp() const
{
    return m_mp;
}

int
Character::max_mp() const
{
    return m_max_mp;
}

int
Character::mind() const
{
    return m_mind;
}

int
Character::resilience() const
{
    return m_resilience;
}

int
Character::willpower() const
{
    return m_willpower;
}

int
Character::agility() const
{
    return m_agility;
}

int
Character::perception() const
{
    return m_perception;
}

int
Character::might_attack() const
{
    return m_might_attack;
}

int
Character::mind_attack() const
{
    return m_mind_attack;
}

int
Character::cooldown() const
{
    return m_cooldown;
}

int
Character::defense() const
{
    return m_defense;
}

int
Character::might_armor() const
{
    return m_might_armor;
}

int
Character::mind_armor() const
{
    return m_mind_armor;
}

int
Character::critical() const
{
    return m_critical;
}

void
Character::set_attacks_quantity(int attacks_quantity)
{
    m_attacks_quantity = attacks_quantity;
}

void
Character::set_military(int military)
{
    m_military = military;
    write<int>("military", military);
}

void
Character::set_psionic(int psionic)
{
    m_psionic = psionic;
    write<int>("psionic", psionic);
}

void
Character::set_tech(int tech)
{
    m_tech = tech;
    write<int>("tech", tech);
}

void
Character::set_levelup_m(int levelup_m)
{
    m_levelup_m = levelup_m;
    write<int>("levelup_m", levelup_m);
}

void
Character::set_levelup_p(int levelup_p)
{
    m_levelup_p = levelup_p;
    write<int>("levelup_p", levelup_p);
}

void
Character::set_levelup_t(int levelup_t)
{
    m_levelup_t = levelup_t;
    write<int>("levelup_t", levelup_t);
}

void
Character::set_shield(int shield)
{
    m_shield = shield;
    write<int>("shield", shield);
}

void
Character::set_max_shield(int max_shield)
{
    m_max_shield = max_shield;
    write<int>("max_shield", max_shield);
}

void
Character::set_life(int life)
{
    m_life = life;
    write<int>("life", life);
}

void
Character::set_max_life(int max_life)
{
    m_max_life = max_life;
    write<int>("max_life", max_life);
}

void
Character::set_mp(int mp)
{
    m_mp = mp;
    write<int>("mp", mp);
}

void
Character::set_max_mp(int max_mp)
{
    m_max_mp = max_mp;
    write<int>("max_mp", max_mp);
}

void
Character::set_might(int might)
{
    m_might = might;
    write<int>("might", might);
}

void
Character::set_mind(int mind)
{
    m_mind = mind;
    write<int>("mind", mind);
}

void
Character::set_resilience(int resilience)
{
    m_resilience = resilience;
    write<int>("resilience", resilience);
}

void
Character::set_willpower(int willpower)
{
    m_willpower = willpower;
    write<int>("willpower", willpower);
}

void
Character::set_agility(int agility)
{
    m_agility = agility;
    write<int>("agility", agility);
}

void
Character::set_perception(int perception)
{
    m_perception = perception;
    write<int>("perception", perception);
}

void
Character::set_might_attack(int might_attack)
{
    m_might_attack = might_attack;
    write<int>("might_attack", might_attack);
}

void
Character::set_mind_attack(int mind_attack)
{
    m_mind_attack = mind_attack;
    write<int>("mind_attack", mind_attack);
}

void
Character::set_cooldown(int cooldown)
{
    m_cooldown = cooldown;
    write<int>("cooldown", cooldown);
}

void
Character::set_defense(int defense)
{
    m_defense = defense;
    write<int>("defense", defense);
}

void
Character::set_might_armor(int might_armor)
{
    m_might_armor = might_armor;
    write<int>("might_armor", might_armor);
}

void
Character::set_mind_armor(int mind_armor)
{
    m_mind_armor = mind_armor;
    write<int>("mind_armor", mind_armor);
}

void
Character::set_critical(int critical)
{
    m_critical = critical;
    write<int>("critical", critical);
}

template<typename T> void
Character::write(const string& attr, const T& value)
{
    if (m_name != "Default")
    {
        m_settings->write<int>(m_name, attr, value);
    }
}
