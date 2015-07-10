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

class Barracks;
class Button;
class Settings;

using std::map;

class Equip : public Object
{
public:
    Equip(int slot, Object *parent = nullptr);

private:
    typedef string State;
    typedef string EquipmentClass;

    int m_slot, m_matter_cost, m_energy_cost;
    EquipmentClass m_class;
    ObjectID m_equipment;
    Button *m_status;
    State m_state;
    Barracks *m_barracks;
    shared_ptr<Settings> m_settings;

    map<ObjectID, Button *> m_weapons;
    map<ObjectID, Button *> m_armor;
    map<ObjectID, Button *> m_shield;
    map<ObjectID, Button *> m_buttons;
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
    void deactivate_equipments();
    void buy_equipment(ObjectID equipment);
    void change_buttons();
    void change_buttons(bool visible);
};

#endif
