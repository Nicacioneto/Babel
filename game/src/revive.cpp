/*
 * Revive class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "colony.h"
#include "hospital.h"
#include "revive.h"
#include <core/color.h>
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>
#include <core/texture.h>

using std::to_string;

#define W 1024.0
#define H 768.0

Revive::Revive(int slot, Colony *colony, Object *parent)
    : Object(parent), m_slot(slot), m_colony(colony), m_settings(nullptr),
        m_page(1), m_max_pages(0), m_font(nullptr)
{
    parent->add_observer(this);

    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";

    m_textures["matter_energy"] = env->resources_manager->get_texture(path +
        "icons/matter_energy.png");
    m_textures["health"] = env->resources_manager->get_texture(path + "/icons/health.png");

    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/characters.sav");

    m_max_pages = 2;

    create_buttons();

    m_font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(m_font);
}

void
Revive::draw_self()
{
    Environment *env = Environment::get_instance();
    Color color(170, 215, 190);
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    m_font->set_size(18);

    env->canvas->draw("Name", 360 * scale_w, 186* scale_h, color);
    env->canvas->draw("M", 510 * scale_w, 186 * scale_h, color);
    env->canvas->draw("P", 543 * scale_w, 186 * scale_h, color);
    env->canvas->draw("T", 573 * scale_w, 186 * scale_h, color);
    env->canvas->draw(m_textures["matter_energy"].get(), 706 * scale_w, 188 * scale_h);
    env->canvas->draw("HP", 833 * scale_w, 186 * scale_h, color);

    draw_character(scale_w, scale_h, color);
}

void
Revive::draw_character(double scale_w, double scale_h, Color color)
{
    Environment *env = Environment::get_instance();

    int y = 236, i = 0;

    for (auto section : m_settings->sections())
    {
        if (++i <= (m_page - 1) * BIG_LIST or i > BIG_LIST * m_page)
        {
            change_button_state(m_buttons[section.first], false);
            continue;
        }

        change_button_state(m_buttons[section.first], true, y);

        env->canvas->draw(m_textures["health"].get(), Rect(0, 25, 50, 50/2),
            310 * scale_w, y * scale_h, 50 * scale_w, 25 * scale_h);

        env->canvas->draw(section.first, 360 * scale_w, y * scale_h, color);
        env->canvas->draw(section.second["military"], 510 * scale_w, y * scale_h, color);
        env->canvas->draw(section.second["psionic"], 540 * scale_w, y * scale_h, color);
        env->canvas->draw(section.second["tech"], 570 * scale_w, y * scale_h, color);

        int life = m_settings->read<int>(section.first, "life", 0);
        int max_life = m_settings->read<int>(section.first, "max_life", 0);

        int m = m_settings->read<int>(section.first, "military", 0);
        int p = m_settings->read<int>(section.first, "psionic", 0);
        int t = m_settings->read<int>(section.first, "tech", 0);

        int level = m + t + p;

        int cost = (level * 100 * (max_life - life)) / max_life;

        env->canvas->draw(to_string(cost), 706 * scale_w, y * scale_h, color);

        if (life == 0)
        {
            color = Color(159, 6, 6);
        }
        env->canvas->draw(to_string(life) + "/" + to_string(max_life),
            810 * scale_w, y * scale_h, color);

        y += 64;
    }
}


bool
Revive::on_message(Object *sender, MessageID id, Parameters p)
{
    Hospital *hospital = nullptr;
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        hospital = dynamic_cast<Hospital *>(sender);
        if (not hospital)
        {
            return false;
        }
    }

    if (hospital)
    {
        set_visible(id == "revive");
        change_buttons(id != "chat", visible());
        
        if (visible())
        {
            notify("max_pages", to_string(m_max_pages));
            
            if (p.size())
            {
                m_page = atoi(p.c_str());
            }
            else
            {
                m_page = 1;
            }
        }
    }
    else if (button)
    {
        revive_character(button->id());
    }

    return false;
}

void
Revive::create_buttons()
{
    Environment *env = Environment::get_instance();
    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/characters.sav");

    auto sections = m_settings->sections();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    int y = 236;

    for (auto s : sections)
    {
        Button *button = new Button(this, s.first, "res/images/colony/big_list.png",
            310 * scale_w, (y+5) * scale_h, 602 * scale_w, 25 * scale_h);

        m_buttons[button->id()] = button;

        button->add_observer(this);
        add_child(button);

        y += 64;
    }
}

void
Revive::change_buttons(bool visible, bool active)
{
    for (auto button : m_buttons)
    {
        button.second->set_active(active);
        button.second->set_visible(visible);
    }
}

void
Revive::change_button_state(Button *button, bool state, int y)
{
    button->set_visible(state);
    button->set_active(state);

    if (state)
    {
        Environment *env = Environment::get_instance();
        button->set_y((y+5) * env->canvas->h() / H);
    }
}

void
Revive::revive_character(const ObjectID id)
{
    int life = m_settings->read<int>(id, "life", 0);
    int max_life = m_settings->read<int>(id, "max_life", 0);

    int m = m_settings->read<int>(id, "military", 0);
    int p = m_settings->read<int>(id, "psionic", 0);
    int t = m_settings->read<int>(id, "tech", 0);

    int level = m + t + p;

    int cost = (level * 100 * (max_life - life)) / max_life;

    int matter = m_colony->matter() - cost;

    if (matter >= 0)
    {
        m_colony->set_matter(matter);

        m_settings->write<int>(id, "life", max_life);
        m_settings->save("res/datas/slot" + to_string(m_slot) + "/characters.sav");
    }
}
