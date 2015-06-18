#ifndef CHARACTER_H
#define CHARACTER_H

#include "button.h"
#include <core/object.h>
#include <core/listener.h>

class Texture;
class MouseButtonEvent;

class Character : public Object, public Listener
{
public:
    Character(Object *parent = nullptr, ObjectID id = "Default", const string& texture = "",
        double x = 0, double y = 0, double w = 0, double h = 0);
    ~Character();

    int receive_damage(Character *character);

    int attacks_quantity();
    int levelup();
    int military();
    int psionic();
    int tech();
    int might();
    int mind();
    int perception();
    int agility();
    int might_attack();
    int mind_attack();
    int cooldown();
    int defense();
    int might_armor();
    int mind_armor();
    int critical();
    int life();
    int shield();
    int mp();
    int max_life();
    int max_shield();
    int max_mp();

    void set_attacks_quantity(int attacks_quantity);
    void set_levelup(int levelup);
    void set_military(int military);
    void set_psionic(int psionic);
    void set_tech(int tech);
    void set_might(int might);
    void set_mind(int mind);
    void set_perception(int perception);
    void set_agility(int agility);
    void set_might_attack(int might_attack);
    void set_mind_attack(int mind_attack);
    void set_cooldown(int cooldown);
    void set_defense(int defense);
    void set_might_armor(int might_armor);
    void set_mind_armor(int mind_armor);
    void set_critical(int critical);
    void set_life(int life);
    void set_shield(int shield);
    void set_mp(int mp);
    void set_max_life(int max_life);
    void set_max_shield(int max_shield);
    void set_max_mp(int max_mp);

private:
    shared_ptr<Texture> m_texture;
    shared_ptr<Settings> m_settings;

    string m_name;
    int m_attacks_quantity;

    int m_levelup;
    int m_military;
    int m_psionic;
    int m_tech;
    int m_might;
    int m_mind;
    int m_perception;
    int m_agility;
    int m_might_attack;
    int m_mind_attack;
    int m_cooldown;
    int m_defense;
    int m_might_armor;
    int m_mind_armor;
    int m_critical;
    int m_life;
    int m_shield;
    int m_mp;
    int m_max_life;
    int m_max_shield;
    int m_max_mp;

    void init();
    void draw_self();
    bool on_event(const MouseButtonEvent& event);

    template<typename T>
    void write(const string& name, const T& value);
};

#endif
