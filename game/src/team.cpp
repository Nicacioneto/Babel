/*
 * Team class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "character.h"
#include "team.h"
#include <algorithm>
#include <core/color.h>
#include <core/font.h>
#include <core/settings.h>
#include <core/text.h>
#include <core/texture.h>
#include <core/rect.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Team::Team(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_settings(nullptr)
{
    Environment *env = Environment::get_instance();
    string path = "res/datas/slot" + to_string(m_slot) + "/squad.sav";
    m_settings = env->resources_manager->get_settings(path);

    load_characters();
    load_team();
}

void
Team::draw_self()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Select Squad", 60 * scale_w, 50 * scale_h, Color(70, 89, 79));
}

bool
Team::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    for (auto c : m_characters)
    {
        if (button->id() == c.first and c.second->available())
        {
            if (button->visible() and m_team.size() < 4) // not selected
            {
                m_team.push_back(c.first);
                button->set_visible(false);
            }
            else
            {
                m_team.erase(remove(m_team.begin(), m_team.end(), c.first), m_team.end());
                button->set_visible(true);
            }

            break;
        }
    }

    return true;
}

void
Team::load_characters()
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/squad/";

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/characters.sav");

    int x = 155, y = 175, w = 222, h = 123;
    int i = 0, j = 0;
    for (auto section : settings->sections())
    {
        if (j > 2)
        {
            ++i;
            j = 0;
        }

        if (i > 2)
        {
            break;
        }

        if (section.first != "Default")
        {
            Character *character = new Character(m_slot, this, section.first,
                section.first + "_small.png", (x + 249*j) * scale_w, (y + 150*i) * scale_h,
                w * scale_w, h * scale_h);
            character->set_active(false);

            character->add_observer(this);
            add_child(character);
            m_characters[character->id()] = character;

            Button *button = new Button(this, section.first, (x + 249*j) * scale_w,
                (y + 150*i) * scale_h, w * scale_w, h * scale_h, Color(0, 0, 0, 128));

            button->add_observer(this);
            add_child(button);
            m_buttons[button->id()] = button;

            ++j;
        }
    }
}

void
Team::load_team()
{
    m_team.clear();
    
    auto heroes = m_settings->sections()["Squad"];

    for (auto h : heroes)
    {
        if (h.second != "")
        {
            if (m_characters[h.second]->life() == 0)
            {
                m_settings->write<string>("Squad", h.first, "");
            }
            else
            {
                m_buttons[h.second]->set_visible(false);
                m_team.push_back(h.second);
            }
        }
    }

    m_settings->save("res/datas/slot" + to_string(m_slot) + "/squad.sav");
}

void
Team::confirm()
{
    int i = 1;
    for (auto id : m_team)
    {
        m_settings->write<string>("Squad", "hero" + to_string(i++), id);
    }

    for (int j = i; j <= 4; ++j)
    {
        m_settings->write<string>("Squad", "hero" + to_string(j), "");
    }

    m_settings->save("res/datas/slot" + to_string(m_slot) + "/squad.sav");
}

void
Team::reset()
{
    for (auto c : m_characters)
    {
        m_buttons[c.first]->set_visible(true);
    }

    load_team();
}

void
Team::change_buttons(bool state)
{
    for (auto b : m_buttons)
    {
        b.second->set_visible(state);
        b.second->set_active(state);
    }

    load_team();
}

unsigned int
Team::size()
{
    return m_team.size();
}
