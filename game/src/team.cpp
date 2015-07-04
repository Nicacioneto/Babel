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
#include <iostream>
using namespace std;

#define W 1024.0
#define H 768.0

using std::to_string;

Team::Team(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_settings(nullptr), m_bracket(nullptr)
{
    Environment *env = Environment::get_instance();
    string path = "res/datas/slot" + to_string(m_slot) + "/squad.sav";
    m_settings = env->resources_manager->get_settings(path);

    m_bracket = env->resources_manager->get_texture("res/images/characters/card_small.png");

    load_characters();
    load_team();
    load_texts();
}

void
Team::draw_self()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Select Squad", 60 * scale_w, 50 * scale_h, Color(70, 89, 79));

    int x = 155, y = 175;
    int i = 0, j = 0;
    for (auto c : m_characters)
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

        env->canvas->draw(m_bracket.get(), (x + 249*j) * scale_w, (y + 150*i) * scale_h);
        env->canvas->draw(c.first, (x + 133 + 249*j) * scale_w,
            (y + 3 + 150*i) * scale_h, Color(170, 215, 190));

        draw_attributes(x, y, i, j, c.first);
        
        ++j;
    }

}

void
Team::draw_attributes(int x, int y, int i, int j, string id)
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Rect box_shield((x + 172 + 249*j) * scale_w, (y + 32 + 150*i) * scale_h,
        40 * scale_w, 21 * scale_h);
    double w_shield = m_texts[id + "_shield"]->w() + m_texts[id + "_max_shield"]->w();
    double x_shield = (box_shield.w() - w_shield)/2 + box_shield.x();
    double y_shield = (box_shield.h() - m_texts[id + "_shield"]->h())/2 + box_shield.y();
    double x_max_shield = x_shield + m_texts[id + "_shield"]->w();
    double y_max_shield = y_shield + m_texts[id + "_shield"]->h() -
        m_texts[id + "_max_shield"]->h();

    m_texts[id + "_shield"]->set_position(x_shield, y_shield);
    m_texts[id + "_max_shield"]->set_position(x_max_shield, y_max_shield);
    
    m_texts[id + "_shield"]->draw();
    m_texts[id + "_max_shield"]->draw();
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
        if (button->id() == c.first)
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
            Character *character = new Character(m_slot, this, section.first, "Albert_small.png",
                (x + 249*j) * scale_w, (y + 150*i) * scale_h,
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
            m_buttons[h.second]->set_visible(false);
            m_team.push_back(h.second);
        }
    }
}

void
Team::load_texts()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);

    for (auto c : m_characters)
    {
        string shield = to_string(c.second->shield());
        string max_shield = to_string(c.second->max_shield());

        font->set_size(11);
        m_texts[c.first + "_shield"] = new Text(this, shield + "/", Color(170, 215, 190));

        font->set_size(8);
        m_texts[c.first + "_max_shield"] = new Text(this, max_shield, Color(170, 215, 190));
    }
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
