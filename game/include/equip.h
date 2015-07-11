/*
 * Class that represents the Equip Hero
 *
 * Author: Tiamat
 * Date: 07/07/2015
 * License: LGPL. No copyright.
 */
#ifndef EQUIP_H
#define EQUIP_H

#include "textbox.h"
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
    typedef string Type;
    typedef string EquipmentClass;

    int m_slot, m_matter_cost, m_energy_cost;
    EquipmentClass m_class;
    ObjectID m_equipment;
    Button *m_status;
    Type m_type;
    Barracks *m_barracks;
    TextBox *m_equipment_text;
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
    void load_equipments(const string& type);
    void draw_self();
    void draw_equipments();
    void create_textbox();
    string get_equipment(const string& equipment_id, const string& attr) const;
    bool on_message(Object *sender, MessageID id, Parameters p);
    void deactivate_equipments();
    void buy_equipment(ObjectID equipment);
    void change_buttons();
    void change_buttons(bool visible);
};

#endif
