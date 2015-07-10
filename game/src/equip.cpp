/*
 * Equip class implementation
 *
 * Author: Tiamat
 * Date: 07/07/2015
 * License: LGPL. No copyright.
 */
#include "barracks.h"
#include "character.h"
#include "colony.h"
#include "equip.h"
#include <algorithm>
#include <core/font.h>
#include <core/keyboardevent.h>
#include <core/line.h>
#include <core/point.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Equip::Equip(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_matter_cost(0), m_energy_cost(0),
        m_class("weapon"), m_state("rifle"), m_barracks(nullptr)
{
    parent->add_observer(this);

    m_barracks = dynamic_cast<Barracks *>(parent);

    load_textures();
    create_buttons();
}

void
Equip::load_textures()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/barracks/equip/";
    m_textures["rifle_katana"] = env->resources_manager->get_texture(path + "Rifle_Katana.png");
    m_textures["bracket_equip"] = env->resources_manager->get_texture(path + "Bracket.png");
    m_textures["status"] = env->resources_manager->get_texture(path + "status.png");
    m_textures["resources"] = env->resources_manager->get_texture(path + "resources.png");
    m_textures["bracket_m"] = env->resources_manager->get_texture(path + "bracket_m.png");
    m_textures["bracket_p"] = env->resources_manager->get_texture(path + "bracket_p.png");
    m_textures["bracket_t"] = env->resources_manager->get_texture(path + "bracket_t.png");
    m_attributes["agility"] = env->resources_manager->get_texture(path + "icon_agility.png");
    m_attributes["critical"] = env->resources_manager->get_texture(path + "icon_critical.png");
    m_attributes["hit_chance"] = env->resources_manager->get_texture(path + "icon_hitchance.png");
    m_attributes["might"] = env->resources_manager->get_texture(path + "icon_might.png");
    m_attributes["mind"] = env->resources_manager->get_texture(path + "icon_mind.png");
    m_attributes["perception"] = env->resources_manager->get_texture(path + "icon_perception.png");
    m_attributes["resilience"] = env->resources_manager->get_texture(path + "icon_resilience.png");
    m_attributes["speed"] = env->resources_manager->get_texture(path + "icon_speed.png");
    m_attributes["willpower"] = env->resources_manager->get_texture(path + "icon_willpower.png");
}

void
Equip::create_buttons()
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    string path = "res/images/colony/barracks/equip/";

    int y = 334 * scale_h;
    int w = 35 * scale_w;
    int h = 35 * scale_h;

    Button *button = new Button(this, "rifle", path + "weapon_rifle.png", 148 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "shotgun", path + "weapon_shotgun.png", 201 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "pistol", path + "weapon_pistol.png", 254 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "sniper", path + "weapon_sniper.png", 307 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "melee", path + "weapon_melee.png", 413 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "hand", path + "weapon_hand.png", 518 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "nano", path + "weapon_nano.png", 571 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "ui", path + "weapon_ui.png", 677 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "psiblade", path + "weapon_psiblade.png", 730 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "psiamp", path + "weapon_psiamp.png", 783 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "psiwhip", path + "weapon_psiwhip.png", 836 * scale_w, y, w, h);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    for (auto b : m_weapons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    button = new Button(this, "light", path + "armor_light.png", 148 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_armor[button->id()] = button;

    button = new Button(this, "medium", path + "armor_medium.png", 201 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_armor[button->id()] = button;

    button = new Button(this, "heavy", path + "armor_heavy.png", 254 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_armor[button->id()] = button;

    for (auto b : m_armor)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    button = new Button(this, "shield", path + "shield_shield.png", 148 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_shield[button->id()] = button;

    button = new Button(this, "barrier", path + "shield_barrier.png", 201 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_shield[button->id()] = button;

    for (auto b : m_shield)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    load_equipments("weapon");
    load_equipments("armor");
    load_equipments("shield");

    m_class = "weapon";

    m_status = new Button(this, "status", path + "status.png", 145 * scale_w, 432 * scale_h,
        298 * scale_w, 30 * scale_h);
    m_status->set_sprites(4);
    m_status->set_active(false);
    m_status->set_visible(false);
    m_status->add_observer(this);
    add_child(m_status);
}

void
Equip::load_equipments(string type)
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    Color color(170, 215, 190);
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int x = 518 * scale_w, y = 480;
    string path = "res/datas/slot" + to_string(m_slot) + "/";
    shared_ptr<Settings> settings = env->resources_manager->get_settings(path + type + ".sav");

    for (auto it : settings->sections())
    {
        int num = it.first.front() - '1';
        Button *button = new Button(this, it.first, x, (y + 30 * num) * scale_h);
        button->set_sprites(1);
        button->set_text(it.first, color);
        button->set_dimensions(button->text()->w() * scale_w, button->text()->h() * scale_h);
        button->set_active(m_state == get_equipment(it.first, "type"));
        button->set_visible(m_state == get_equipment(it.first, "type"));
        m_equipments[type][button->id()] = button;
        button->add_observer(this);
        add_child(button);
    }
}

void
Equip::draw_self()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Color color(170, 215, 190);

    env->canvas->draw(m_textures["bracket_equip"].get(), 112 * scale_w, 322 * scale_h);
    env->canvas->draw("Equip Hero", 52 * scale_w, 52 * scale_h, Color(84, 107, 95));
    env->canvas->draw(m_textures["resources"].get(), 518 * scale_w, 432 * scale_h);
    env->canvas->draw(to_string(Colony(m_slot).matter()), 550 * scale_w, 437 * scale_h, color);
    env->canvas->draw(to_string(Colony(m_slot).energy()), 615 * scale_w, 437 * scale_h, color);
    env->canvas->draw("Avaible " + m_state + "s", 700 * scale_w, 436 * scale_h, Color(84, 107, 95));

    for (auto b : m_weapons)
    {
        b.second->set_active(m_class == "weapon");
        b.second->set_visible(m_class == "weapon");
    }

    for (auto b : m_armor)
    {
        b.second->set_active(m_class == "armor");
        b.second->set_visible(m_class == "armor");
    }

    for (auto b : m_shield)
    {
        b.second->set_active(m_class == "shield");
        b.second->set_visible(m_class == "shield");
    }

    draw_equipments();
}

void
Equip::draw_equipments()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    Color color(84, 107, 95);

    if (not m_equipment.empty())
    {
        // shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        //     to_string(m_slot) + "/equipments.sav");
        // auto sections = settings->sections();
        // string text = sections["Rifle"]["text"];

        // replace(text.begin(), text.end(), ';', '\n');
        // env->canvas->draw(text, 145 * scale_w, 384 * scale_h, color);

        env->canvas->draw("Assault Rifles have high fire rate, sending",
            145 * scale_w, 384 * scale_h, color);
        env->canvas->draw("wave after wave of raining death.",
            145 * scale_w, (384+17) * scale_h, color);

    // printf("%s\n", m_character->id().c_str());

        // Rect clip = Rect(0, yi * 30, 298, 30);
        // env->canvas->draw(m_textures["status"].get(), clip, 145 * scale_w, 432 * scale_h,
        //     298 * scale_w, 30 * scale_h);

        color = Color(170, 215, 190);
        env->canvas->draw("The Katana Assault Rifle is a", 211 * scale_w, 512 * scale_h, color);
        env->canvas->draw("state-of-the art weapon, being", 211 * scale_w,
            (512+17) * scale_h, color);
        env->canvas->draw("able to shoot a 10mm", 211 * scale_w, (512+17*2) * scale_h, color);
        env->canvas->draw("steel-core bullet at 800 rpm.", 211 * scale_w,
            (512+17*3) * scale_h, color);

        env->canvas->draw(m_textures["bracket_m"].get(), 313 * scale_w, 483 * scale_h);
        env->canvas->draw(m_textures["bracket_p"].get(), 358 * scale_w, 483 * scale_h);
        env->canvas->draw(m_textures["bracket_t"].get(), 402 * scale_w, 483 * scale_h);
        env->canvas->draw(get_equipment(m_equipment, "m"), (315+15) * scale_w, 483 * scale_h,
            Color(183, 157, 39));
        env->canvas->draw(get_equipment(m_equipment, "p"), (358+15) * scale_w, 483 * scale_h,
            Color(143, 61, 130));
        env->canvas->draw(get_equipment(m_equipment, "t"), (402+15) * scale_w, 483 * scale_h,
            Color(60, 145, 145));

        font->set_size(24);
        env->canvas->draw(m_equipment.substr(3), 145 * scale_w, 477 * scale_h, color);
        env->canvas->draw(m_textures["rifle_katana"].get(), 145 * scale_w, 512 * scale_h);

        font->set_size(14);

        int x = 150, y = 640;
        for (auto icon : m_attributes)
        {
            string value = get_equipment(m_equipment, icon.first);
            if (value.substr(1) == "0")
            {
                continue;
            }

            env->canvas->draw(icon.second.get(), x * scale_w, y * scale_h);
            env->canvas->draw(value, x * scale_w, (y-25) * scale_h, color);
            x += 50;
        }
        auto button = m_equipments[m_class][m_equipment];
        Point a(button->x(), button->y() + 20);
        Point b(button->x() + m_textures["resources"].get()->w() * scale_w, button->y() + 20);
        env->canvas->draw(Line(a, b), color);

        font->set_size(16);
        if (m_status->state() != Button::ACTIVE)
        {
            Character *c = m_barracks->current_char();
            if (atoi(get_equipment(m_equipment, "m").c_str()) > c->military() or
                atoi(get_equipment(m_equipment, "p").c_str()) > c->psionic() or
                atoi(get_equipment(m_equipment, "t").c_str()) > c->tech() or
                atoi(get_equipment(m_equipment, "matter").c_str()) > Colony(m_slot).matter() or
                atoi(get_equipment(m_equipment, "energy").c_str()) > Colony(m_slot).energy())
            {
                color = Color(154, 6, 6);
            }

            m_matter_cost = atoi(get_equipment(m_equipment, "matter").c_str());
            m_energy_cost = atoi(get_equipment(m_equipment, "energy").c_str());
            env->canvas->draw(to_string(m_matter_cost), 310 * scale_w, 436 * scale_h, color);
            env->canvas->draw(to_string(m_energy_cost), 380 * scale_w, 436 * scale_h, color);
        }
    }
}

string
Equip::get_equipment(string equipment_id, string attr)
{
    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/" + m_class + ".sav");
    return settings->read<string>(equipment_id, attr, "+0");
}

bool
Equip::on_message(Object *sender, MessageID id, Parameters)
{
    Barracks *barracks = dynamic_cast<Barracks *>(sender);

    if (barracks)
    {
        deactivate_equipments();

        if (id == "weapon")
        {
            m_class = "weapon";
            m_equipment.clear();
        }
        else if (id == "armor")
        {
            m_class = "armor";
            m_equipment.clear();
        }
        else if (id == "shield")
        {
            m_class = "shield";
            m_equipment.clear();
        }
        else
        {
            set_visible(id.find("enable") != string::npos);
        }

        m_status->set_active(false);
        m_status->set_visible(false);

        return true;
    }

    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    deactivate_equipments();

    for (auto b : m_weapons)
    {
        if (button->id() == b.first)
        {
            m_state = button->id();
            m_equipment.clear();
            break;
        }
    }

    for (auto b : m_armor)
    {
        if (button->id() == b.first)
        {
            m_state = button->id();
            m_equipment.clear();
            break;
        }
    }

    for (auto b : m_shield)
    {
        if (button->id() == b.first)
        {
            m_state = button->id();
            m_equipment.clear();
            break;
        }
    }

    m_status->set_active(not m_equipment.empty());
    m_status->set_visible(not m_equipment.empty());

    bool ok = 1;
    for (auto b : m_equipments[m_class])
    {
        if (button->id() == b.first)
        {
            ok = 0;
            m_equipment = b.first;

            Environment *env = Environment::get_instance();
            shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
                to_string(m_slot) + "/" + m_class + ".sav");
            
            Character *c = m_barracks->current_char();            
            int equiped = settings->read<int>(m_equipment, c->id(), 0);

            if (equiped)
            {
                m_status->change_state(Button::ACTIVE);
            }
            else if (atoi(get_equipment(b.first, "m").c_str()) > c->military() or
                atoi(get_equipment(b.first, "p").c_str()) > c->psionic() or
                atoi(get_equipment(b.first, "t").c_str()) > c->tech() or
                atoi(get_equipment(b.first, "matter").c_str()) > Colony(m_slot).matter() or
                atoi(get_equipment(b.first, "energy").c_str()) > Colony(m_slot).energy())
            {
                m_status->change_state(Button::INACTIVE);
            }
            else
            {
                m_status->change_state(Button::IDLE);
            }
        }

        b.second->set_active(get_equipment(b.first, "type") == m_state);
        b.second->set_visible(get_equipment(b.first, "type") == m_state);
    }

    if (button->id() == "status")
    {
        if (button->state() != Button::ACTIVE)
        {
            buy_equipment(m_equipment);
        }

        ok = 0;
        button->change_state(Button::ACTIVE);
    }

    if (ok)
    {
        change_buttons();
        button->change_state(Button::ACTIVE);
    }

    return true;
}

void
Equip::deactivate_equipments()
{
    for (auto b : m_equipments["weapon"])
    {
        b.second->set_active(false);
        b.second->set_visible(false);
    }

    for (auto b : m_equipments["armor"])
    {
        b.second->set_active(false);
        b.second->set_visible(false);
    }

    for (auto b : m_equipments["shield"])
    {
        b.second->set_active(false);
        b.second->set_visible(false);
    }
}

void
Equip::buy_equipment(ObjectID equipment)
{
    Colony(m_slot).set_matter(Colony(m_slot).matter() - m_matter_cost);
    Colony(m_slot).set_energy(Colony(m_slot).energy() - m_energy_cost);

    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/" + m_class + ".sav");

    settings->write<int>(equipment, m_barracks->current_char()->id(), 1);
    settings->save("res/datas/slot" + to_string(m_slot) + "/" + m_class + ".sav");
}

void
Equip::change_buttons()
{
    for (auto b : m_weapons)
    {
        b.second->change_state(Button::IDLE);
    }

    for (auto b : m_armor)
    {
        b.second->change_state(Button::IDLE);
    }

    for (auto b : m_shield)
    {
        b.second->change_state(Button::IDLE);
    }
}
