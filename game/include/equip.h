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
    Equip(int slot, Object *parent = nullptr, Character *character = nullptr);
    void set_character(Character *character);

private:
    typedef string State;
    typedef string EquipmentClass;

    int m_slot, m_matter_cost, m_energy_cost;
    EquipmentClass m_class;
    ObjectID m_equipment;
    Character *m_character;
    Button *m_status;
    State m_state;

    map<ObjectID, Button *> m_weapons;
    map<ObjectID, Button *> m_armor;
    map<ObjectID, Button *> m_shield;
    map<ObjectID, Button *> m_rifle;
    map<ObjectID, Character*> m_characters;
    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, shared_ptr<Texture>> m_attributes;
    map<EquipmentClass, map<ObjectID, Button *>> m_equipments;

    void load_textures();
    void create_buttons();
    void load_equipments(string type);
    void draw_self();
    void draw_equipments();
    string get_equipment(string equipment_id, string attr);
    bool on_message(Object *sender, MessageID id, Parameters p);
    void change_buttons();
};

#endif
