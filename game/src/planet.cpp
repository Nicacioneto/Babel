/*
 * Planet class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "planet.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/settings.h>
#include "timer.h"

#define W 1024.0
#define H 768.0

using std::to_string;

Planet::Planet(int slot, const string& next)
    : Level("planet", next), m_slot(slot), m_state(IDLE), m_last(0)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/planet/";

    m_texture = env->resources_manager->get_texture(path + "planet.png");
    m_popup = env->resources_manager->get_texture(path + "popup.png");

    load_buttons();
}

void
Planet::update_self(unsigned long elapsed)
{
    m_last = elapsed;
}

void
Planet::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_texture.get());

    if (m_state == POPUP)
    {
        int x = (env->canvas->w() - m_popup->w())/2;
        int y = (env->canvas->h() - m_popup->h())/2;

        Rect rect { 0, 0, (double)env->canvas->w(), (double)env->canvas->h() };
        
        env->canvas->set_blend_mode(Canvas::BLEND);
        env->canvas->fill(rect, Color(0, 0, 0, 128));
        env->canvas->set_blend_mode(Canvas::NONE);
        
        env->canvas->draw(m_popup.get(), x, y);

        env->canvas->draw("Start misson in " + m_text, x + 10 * env->canvas->w() / W,
            y + m_popup->h()/2);
    }
}

bool
Planet::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "colony")
    {
        set_next("base");
        finish();
    }
    else if (button->id() == "trunda")
    {
        m_text = "Trunda";
        enable_popup(true);
    }
    else if (button->id() == "jungle")
    {
        m_text = "Jungle";
        enable_popup(true);
    }
    else if (button->id() == "sea")
    {
        m_text = "Sea";
        enable_popup(true);
    }
    else if (button->id() == "dunes")
    {
        m_text = "Dunes";
        enable_popup(true);
    }
    else if (button->id() == "taiga")
    {
        m_text = "Taiga";
        enable_popup(true);
    }
    else if (button->id() == "lake")
    {
        m_text = "Lake";
        enable_popup(true);
    }
    else if (button->id() == "swamp")
    {
        m_text = "Swamp";
        enable_popup(true);
    }
    else if (button->id() == "x")
    {
        enable_popup(false);
    }
    else if (button->id() == "confirm")
    {
        start_mission();
        enable_popup(false);
    }

    return true;
}

void
Planet::enable_popup(bool popup)
{
    for (auto button : m_buttons)
    {
        if (button.first == "x" || button.first == "confirm")
        {
            button.second->set_active(popup);
            button.second->set_visible(popup);
        }
        else
        {
            button.second->set_active(not popup);
            button.second->set_visible(not popup);
        }
    }

    m_state = popup ? POPUP : IDLE;
}

void
Planet::load_buttons()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22);
    string path = "res/images/planet/";

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button = new Button(this, "trunda", 0, 0, 300 * scale_w, 290 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "jungle", 390 * scale_w, 0,
        360 * scale_w, 235 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "sea", 755 * scale_w, 0,
        265 * scale_w, 235 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "dunes", 630 * scale_w, 270 * scale_h,
        394 * scale_w, 185 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "taiga", 0 * scale_w, 410 * scale_h,
        310 * scale_w, 358 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "lake", 300 * scale_w, 500 * scale_h,
        300 * scale_w, 185 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "swamp", 600 * scale_w, 485 * scale_h,
        424 * scale_w, 283 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "colony", 405 * scale_w, 410 * scale_h,
        110 * scale_w, 80 * scale_h);
    m_buttons[button->id()] = button;

    int x = (W / 2 - 25) * scale_w + m_popup->w() / 2;
    int y = (H / 2 + 12) * scale_h - m_popup->h() / 2;

    button = new Button(this, "x", path + "x.png", x, y, 13 * scale_w, 18 * scale_h);
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    const int w = 140 * scale_w;
    const int h = 60 * scale_h;

    x = (env->canvas->w() - w) / 2;
    y = (env->canvas->h() + m_popup->h()) / 2 - (h + 10 * scale_h);

    button = new Button(this, "confirm", path + "button.png", x, y, w, h);
    button->set_text("Confirm");
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}

void
Planet::start_mission()
{
    start_time(m_text, 30, "workshop");
}
