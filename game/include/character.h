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
    Character(Object *parent = nullptr, ObjectID id = "", const string& character = "",
        double x = 0, double y = 0, double w = 0, double h = 0);
    ~Character();

    int receive_damage(Character *character);

    int lvl();
    int cooldown();
    int attacks_quantity();
    double life();
    double mind_points();
    double attack();
    double defense();
    double armor();

    void set_lvl(int lvl);
    void set_cooldown(int cooldown);
    void set_attacks_quantity(int attacks_quantity);
    void set_life(double life);
    void set_mind_points(double mind_points);
    void set_attack(double attack);
    void set_defense(double defense);
    void set_armor(double armor);

private:
    shared_ptr<Texture> m_character;
    shared_ptr<Settings> m_settings;
    
    int m_lvl;
    int m_cooldown;
    int m_attacks_quantity;
    double m_life;
    double m_attack;
    double m_defense;
    double m_mind_points;
    double m_armor;

    void draw_self();
    bool on_event(const MouseButtonEvent& event);
};

#endif
