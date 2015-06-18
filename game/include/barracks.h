#ifndef BARRACKS_H
#define BARRACKS_H

#include "button.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;
class Character;

class Barracks : public Level
{
public:
    Barracks(const string& next = "base");

private:
    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Character*> m_characters;
    shared_ptr<Settings> m_settings;
    shared_ptr<Texture> m_big_card;
    shared_ptr<Texture> m_bracket;
    shared_ptr<Texture> m_bracket_m;
    shared_ptr<Texture> m_bracket_p;
    shared_ptr<Texture> m_bracket_t;
    shared_ptr<Texture> m_attributes;
    shared_ptr<Texture> m_stats;
    shared_ptr<Texture> m_levelup;
    shared_ptr<Texture> m_military;
    shared_ptr<Texture> m_psionic;
    shared_ptr<Texture> m_tech;
    shared_ptr<Texture> m_equip;
    shared_ptr<Texture> m_rifle;
    shared_ptr<Texture> m_armor;
    shared_ptr<Texture> m_shield;
    shared_ptr<Texture> m_equip_change;

    int m_character;

    void draw_self();
    void draw_character();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void load_characters();
};

#endif
