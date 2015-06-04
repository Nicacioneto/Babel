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
        double x = 0, double y = 0, double w = 100, double h = 100);
    ~Character();

    void receive_damage(double damage);
    int lvl();
    void set_lvl(int lvl);
    double life();
    void set_life(double life);
    double attack();
    void set_attack(double attack);
    double defense();
    void set_defense(double defense);

private:
    shared_ptr<Texture> m_character;

    int m_lvl;
    double m_life, m_attack, m_defense;

    void draw_self();

    bool on_event(const MouseButtonEvent& event);
};

#endif
