/*
 * Research class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "button.h"
#include "colony.h"
#include "hospital.h"
#include "research.h"
#include "timer.h"
#include <core/color.h>
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>
#include <core/texture.h>

using std::to_string;

#define W 1024.0
#define H 768.0

Research::Research(int slot, Colony *colony, Object *parent)
    : Object(parent), m_slot(slot), m_colony(colony), m_settings(nullptr),
        m_page(1), m_max_pages(0), m_font(nullptr)
{
    parent->add_observer(this);

    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";

    m_textures["data"] = env->resources_manager->get_texture(path +
        "icons/data.png");

    m_textures["health"] = env->resources_manager->get_texture(path + "/icons/health.png");

    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/items.sav");

    calculate_max_page();

    create_buttons();

    m_font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(m_font);
}

void
Research::draw_self()
{
    Environment *env = Environment::get_instance();
    Color color(170, 215, 190);
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    m_font->set_size(18);

    env->canvas->draw("Name", 360 * scale_w, 188 * scale_h, color);
    env->canvas->draw("Time", 855 * scale_w, 186 * scale_h, color);
    env->canvas->draw(m_textures["data"].get(), 690 * scale_w, 188 * scale_h);

    draw_items(scale_w, scale_h, color);
}

void
Research::draw_items(double scale_w, double scale_h, Color color)
{
    Environment *env = Environment::get_instance();

    int y = 236, i = 0;
    bool research = false;

    for (auto section : m_settings->sections())
    {
        research = m_settings->read<int>(section.first, "research_data", 0) > 0;
        if (not research or (++i <= (m_page - 1) * BIG_LIST or i > BIG_LIST * m_page))
        {
            change_button_state(m_buttons[section.first], false);
            continue;
        }

        change_button_state(m_buttons[section.first], true, y);

        string time = m_settings->read<string>(section.first, "time", "");

        env->canvas->draw(section.first, 360 * scale_w, y * scale_h, color);
        env->canvas->draw(section.second["research_data"],
            690 * scale_w, y * scale_h, color);
        env->canvas->draw(time, 855 * scale_w, y * scale_h, color);

        env->canvas->draw(m_textures["health"].get(), Rect(0, 25, 50, 50/2),
            310 * scale_w, y * scale_h, 50 * scale_w, 25 * scale_h);

        y += 64;
    }
}

bool
Research::on_message(Object *sender, MessageID id, Parameters p)
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
        set_visible(id == "research");
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
        research_item(button->id());
        notify("max_pages", to_string(m_max_pages));
    }

    return false;
}

void
Research::create_buttons()
{
    Environment *env = Environment::get_instance();
    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/items.sav");

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
Research::change_buttons(bool visible, bool active)
{
    for (auto button : m_buttons)
    {
        button.second->set_active(active);
        button.second->set_visible(visible);
    }
}

void
Research::change_button_state(Button *button, bool state, int y)
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
Research::research_item(const ObjectID id)
{
    int data = m_colony->data() - m_settings->read<int>(id, "research_data", 0);

    if (data >= 0)
    {
        m_colony->set_data(data);

        m_settings->write<int>(id, "research_data", 0);

        m_settings->save("res/datas/slot" + to_string(m_slot) + "/items.sav");
        
        string time = m_settings->read<string>(id, "time", "00:00");
        
        unsigned long min = atol(time.substr(0, 2).c_str()) * 60;
        unsigned long seg = atol(time.substr(3).c_str());
        
        start_time(id, min + seg, "hospital", 0, 0, "items");
        calculate_max_page();
    }
}

void
Research::calculate_max_page()
{
    int count = 0;
    for (auto section : m_settings->sections())
    {
        if (atoi(section.second["research_data"].c_str()) > 0)
        {
            count++;
        }
    }
    m_max_pages = (count / BIG_LIST) + (count % BIG_LIST != 0);
    
    if (m_max_pages == 1)
    {
        m_page = 1;
    }
}
