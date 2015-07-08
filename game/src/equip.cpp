/*
 * Equipe class implementation
 *
 * Author: Tiamat
 * Date: 07/07/2015
 * License: LGPL. No copyright.
 */
#include "barracks.h"
#include "character.h"
#include "equip.h"
#include <algorithm>
#include <core/font.h>
#include <core/keyboardevent.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Equip::Equip(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_equip(WEAPON), m_equipment("Katana XM-11"), m_weapon(RIFLE)
{
    parent->add_observer(this);

    load_textures();
    create_buttons();
    load_equipments();
}

void
Equip::load_textures()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/barracks/equip/";
    m_textures["rifle_katana"] = env->resources_manager->get_texture(path + "Rifle_Katana.png");
    m_textures["bracket_equip"] = env->resources_manager->get_texture(path + "Bracket.png");
    m_textures["status"] = env->resources_manager->get_texture(path + "status.png");
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
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    button->change_state(Button::ACTIVE);
    m_weapons[button->id()] = button;

    button = new Button(this, "shotgun", path + "weapon_shotgun.png", 201 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "pistol", path + "weapon_pistol.png", 254 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "sniper", path + "weapon_sniper.png", 307 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "melee", path + "weapon_melee.png", 413 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "hand", path + "weapon_hand.png", 518 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "nano", path + "weapon_nano.png", 571 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "ui", path + "weapon_ui.png", 677 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "psiblade", path + "weapon_psiblade.png", 730 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "psiamp", path + "weapon_psiamp.png", 783 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
    button->set_sprites(3);
    m_weapons[button->id()] = button;

    button = new Button(this, "psiwhip", path + "weapon_psiwhip.png", 836 * scale_w, y, w, h);
    button->set_active(false);
    button->set_visible(false);
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
}

void
Equip::load_equipments()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    string path = "res/datas/slot" + to_string(m_slot) + "/";
    shared_ptr<Settings> settings = env->resources_manager->get_settings(path + "weapons.sav");
    auto sections = settings->sections();
    sections.size();

    Color color(170, 215, 190);
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    int x = 680 * scale_w, y = 435;

    for (auto weapon : sections)
    {
        Button *button = new Button(this, weapon.first, x, y * scale_h);
        button->set_sprites(1);
        button->set_text(weapon.first, color);
        button->set_dimensions(button->text()->w() * scale_w, button->text()->h() * scale_h);
        m_equipments[WEAPON][button->id()] = button;
        button->add_observer(this);
        add_child(button);

        y += 35;
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

    string text = "Avaible ";
    switch (m_equip)
    {
        case RIFLE:
            text += "Rifles";
            break;
        case ARMOR:
            text += "Armor";
            break;
        case SHIELD:
            text += "Shields";
            break;
    }

    env->canvas->draw(text, 733 * scale_w, 402 * scale_h, Color(84, 107, 95));

    for (auto b : m_weapons)
    {
        b.second->set_active(m_equip == WEAPON);
        b.second->set_visible(m_equip == WEAPON);
    }

    for (auto b : m_armor)
    {
        b.second->set_active(m_equip == ARMOR);
        b.second->set_visible(m_equip == ARMOR);
    }

    for (auto b : m_shield)
    {
        b.second->set_active(m_equip == SHIELD);
        b.second->set_visible(m_equip == SHIELD);
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

    if (m_equip == WEAPON)
    {
        Color color(84, 107, 95);

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

        Rect clip = Rect(0, 0, 298, 30);
        env->canvas->draw(m_textures["status"].get(), clip, 145 * scale_w, 432 * scale_h,
            298 * scale_w, 30 * scale_h);

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
        env->canvas->draw(get_equipment("m"), (315+15) * scale_w, 483 * scale_h,
            Color(183, 157, 39));
        env->canvas->draw(get_equipment("p"), (358+15) * scale_w, 483 * scale_h,
            Color(143, 61, 130));
        env->canvas->draw(get_equipment("t"), (402+15) * scale_w, 483 * scale_h,
            Color(60, 145, 145));

        auto button = m_equipments[m_equip][m_equipment];
        env->canvas->draw("->", button->x() - 20, button->y(), color);

        font->set_size(24);
        env->canvas->draw(m_equipment, 145 * scale_w, 477 * scale_h, color);
        env->canvas->draw(m_textures["rifle_katana"].get(), 145 * scale_w, 512 * scale_h);

        font->set_size(14);

        int x = 150, y = 640;
        for (auto icon : m_attributes)
        {
            string value = get_equipment(icon.first);
            if (value.substr(1) == "0")
            {
                continue;
            }

            env->canvas->draw(icon.second.get(), x * scale_w, y * scale_h);
            env->canvas->draw(value, x * scale_w, (y-25) * scale_h, color);
            x += 50;
        }
    }
}

string
Equip::get_equipment(string attr)
{
    string file;
    switch (m_equip)
    {
        case WEAPON:
            file = "/weapons.sav";
            break;
        case ARMOR:
            file = "/armor.sav";
            break;
        case SHIELD:
            file = "/shields.sav";
            break;
    }

    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + file);
    string value = settings->read<string>(m_equipment, attr, "+0");
    return value;
}

bool
Equip::on_message(Object *sender, MessageID id, Parameters)
{
    Barracks *barracks = dynamic_cast<Barracks *>(sender);

    if (barracks)
    {
        if (id == "weapon")
        {
            m_equip = WEAPON;
            m_weapons["rifle"]->change_state(Button::ACTIVE);
        }
        else if (id == "armor")
        {
            m_equip = ARMOR;
            m_armor["light"]->change_state(Button::ACTIVE);
        }
        else if (id == "shield")
        {
            m_equip = SHIELD;
            m_shield["shield"]->change_state(Button::ACTIVE);
        }
        else
        {
            set_visible(id.find("enable") != string::npos);
        }

        return true;
    }

    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    bool ok = 1;
    for (auto b : m_equipments[WEAPON])
    {
        if (button->id() == b.first)
        {
            m_equipment = b.first;
            ok = 0;
        }
    }

    for (auto b : m_equipments[ARMOR])
    {
        if (button->id() == b.first)
        {
            m_equipment = b.first;
            ok = 0;
        }
    }

    for (auto b : m_equipments[SHIELD])
    {
        if (button->id() == b.first)
        {
            m_equipment = b.first;
            ok = 0;
        }
    }

    if (ok)
    {
        change_buttons();
        button->change_state(Button::ACTIVE);
    }

    return true;
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
