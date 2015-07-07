/*
 * Class that represents the Equip Hero
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef EQUIP_H
#define EQUIP_H

#include "button.h"
#include "character.h"
#include <core/level.h>
#include <core/listener.h>
#include <memory>
#include <map>

using std::map;

class Texture;
class Character;
class KeyboardEvent;

class Equip : public Object
{
public:
    Equip(int slot, Object *parent = nullptr);

private:
    typedef enum { WEAPON, ARMOR, SHIELD } Equipment;
    typedef enum { RIFLE, SHOTGUN, PISTOL, SNIPER, MELEE, HAND, NANO,
        UI, PSIBLADE, PSIAMP, PSIWHIP } Weapon;

    int m_slot, m_character;
    Equipment m_equip;
    Weapon m_weapon;

    map<ObjectID, Button *> m_buttons;
    map<ObjectID, Button *> m_weapons;
    map<ObjectID, Button *> m_armor;
    map<ObjectID, Button *> m_shield;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, shared_ptr<Texture>> m_textures;

    void load_textures();
    void create_buttons();
    void draw_self();
    void draw_equipments();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void change_buttons();
};

#endif
