/*
 * Class that represents the Equip Hero
 *
 * Author: Tiamat
 * Date: 07/07/2015
 * License: LGPL. No copyright.
 */
#ifndef EQUIP_H
#define EQUIP_H

#include <memory>
#include <map>

class Button;

using std::map;

class Equip : public Object
{
public:
    Equip(int slot, Object *parent = nullptr);
    void set_character(Character *character);

private:
    typedef enum { WEAPON, ARMOR, SHIELD } Equipment;
    typedef enum { RIFLE, SHOTGUN, PISTOL, SNIPER, MELEE, HAND, NANO,
        UI, PSIBLADE, PSIAMP, PSIWHIP } Weapon;

    int m_slot;
    Equipment m_equip;
    ObjectID m_equipment;
    Weapon m_weapon;
    Character *m_character;

    map<ObjectID, Button *> m_buttons;
    map<ObjectID, Button *> m_weapons;
    map<ObjectID, Button *> m_armor;
    map<ObjectID, Button *> m_shield;
    map<ObjectID, Button *> m_rifle;
    map<Equipment, map<ObjectID, Button *>> m_equipments;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, shared_ptr<Texture>> m_attributes;

    void load_textures();
    void create_buttons();
    void load_equipments();
    void draw_self();
    void draw_equipments();
    string get_equipment(string attr);
    bool on_message(Object *sender, MessageID id, Parameters p);
    void change_buttons();
};

#endif
