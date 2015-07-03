/*
 * Drone class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "character.h"
#include "drone.h"
#include <algorithm>
#include <core/color.h>
#include <core/font.h>
#include <core/settings.h>
#include <core/texture.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Drone::Drone(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_settings(nullptr), m_font(nullptr),
        m_background(nullptr), m_drone("")
{
    Environment *env = Environment::get_instance();
    string path = "res/datas/slot" + to_string(m_slot) + "/squad.sav";
    m_settings = env->resources_manager->get_settings(path);

    m_background = env->resources_manager->get_texture("res/images/tower/squad/background.png");

    create_buttons();
    load_drones();
}

void
Drone::draw_self()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Select Drone", 60 * scale_w, 50 * scale_h, Color(70, 89, 79));
    
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(24);
    
    env->canvas->draw(m_background.get(), 277 * scale_w, 174 * scale_h);
    env->canvas->draw("DRONE", 360 * scale_w, 210 * scale_h, Color(170, 215, 190));
}

bool
Drone::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->state() != Button::ACTIVE)
    {
        m_drone = button->id();
    }
    else
    {
        m_drone = "";
    }
    
    change_buttons(m_drone);

    return false;
}

void
Drone::load_drones()
{
    m_drone = m_settings->read<string>("Drone", "name", "");

    change_buttons(m_drone);
    
    if (m_drone != "")
    {
        m_buttons[m_drone]->change_state(Button::ACTIVE);
    }
}

void
Drone::confirm()
{
    m_settings->write<string>("Drone", "name", m_drone);
    m_settings->save("res/datas/slot" + to_string(m_slot) + "/squad.sav");
}

void
Drone::create_buttons()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    string path = "res/images/tower/squad/";

    Button *button = new Button(this, "hornet", path + "hornet.png",
        360 * scale_w, 330 * scale_h, 305 * scale_w, 40 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "hound", path + "hound.png",
        360 * scale_w, 400 * scale_h, 305 * scale_w, 40 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "hedgehog", path + "hedgehog.png",
        360 * scale_w, 470 * scale_h, 305 * scale_w, 40 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "hawk", path + "hawk.png",
        360 * scale_w, 540 * scale_h, 305 * scale_w, 40 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}

void
Drone::change_buttons(bool state)
{
    for (auto b : m_buttons)
    {
        b.second->set_visible(state);
        b.second->set_active(state);
    }

    load_drones();
}

void
Drone::change_buttons(string id)
{
    for (auto b : m_buttons)
    {
        if (b.first == id)
        {
            b.second->change_state(Button::ACTIVE);
        }
        else
        {
            b.second->change_state(Button::IDLE);
        }
    }
}
