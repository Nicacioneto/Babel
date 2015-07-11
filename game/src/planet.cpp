/*
 * Planet class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "colony.h"
#include "planet.h"
#include "timer.h"
#include <core/font.h>
#include <core/keyboardevent.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Planet::Planet(int slot, const string& next)
    : Level("planet", next), m_slot(slot), m_state(IDLE), m_last(0), m_settings(nullptr)
{
    Environment *env = Environment::get_instance();
    env->events_manager->register_listener(this);

    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/planet.sav");

    Colony *colony = new Colony(slot, this, "planet");
    colony->add_observer(this);
    add_child(colony);

    load_textures();
    create_buttons();
}

Planet::~Planet()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
Planet::load_textures()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/planet/";
    m_textures["planet"] = env->resources_manager->get_texture(path + "planet.png");
    m_textures["popup"] = env->resources_manager->get_texture(path + "popup.png");
    m_textures["place"] = env->resources_manager->get_texture(path + "place.png");
    m_textures["bracket"] = env->resources_manager->get_texture(path + "bracket.png");
    m_textures["Tundra"] = env->resources_manager->get_texture(path + "Tundra.png");
    m_textures["Lake"] = env->resources_manager->get_texture(path + "Lake.png");

    path = "res/images/colony/";
    m_textures["right_bracket"] = env->resources_manager->get_texture(path + "right_bracket.png");
    m_textures["left_bracket"] = env->resources_manager->get_texture(path + "left_bracket.png");
}

void
Planet::create_buttons()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22);
    string path = "res/images/planet/";

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button = new Button(this, "tundra", path + "spot.png", 400 * scale_w, 380 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "jungle", path + "spot.png", 400 * scale_w, 490 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "sea", path + "spot.png", 470 * scale_w, 585 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "dunes", path + "spot.png", 490 * scale_w, 460 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "taiga", path + "spot.png", 505 * scale_w, 312 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "lake", path + "spot.png", 540 * scale_w, 338 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "swamp", path + "spot.png", 715 * scale_w, 377 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "cavern", path + "spot.png", 740 * scale_w, 270 * scale_h,
        25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    int x = 700 * scale_w;
    int y = 212 * scale_h;

    button = new Button(this, "x", path + "x.png", x, y, 30 * scale_w, 25 * scale_h);
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    const int w = 140 * scale_w;
    const int h = 60 * scale_h;

    x = (env->canvas->w() - w) / 2;
    y = (env->canvas->h() + m_textures["popup"]->h()) / 2 - (h + 10 * scale_h);

    button = new Button(this, "send_party", path + "send_party.png", 450 * scale_w, 662 * scale_h,
        120 * scale_w, 35 * scale_h);
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
Planet::update_self(unsigned long elapsed)
{
    m_last = elapsed;
}

void
Planet::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(26);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["planet"].get(), 28 * scale_w, 175 * scale_h);
    env->canvas->draw(m_textures["bracket"].get(), 28 * scale_w, 175 * scale_h);

    Color color(170, 215, 190);

    if (m_state == POPUP)
    {
        Rect rect { 0, 0, (double)env->canvas->w(), (double)env->canvas->h() };

        env->canvas->set_blend_mode(Canvas::BLEND);
        env->canvas->fill(rect, Color(0, 0, 0, 128));
        env->canvas->set_blend_mode(Canvas::NONE);

        env->canvas->draw(m_textures["popup"].get(), 317 * scale_w, 230 * scale_h);
        env->canvas->draw(m_textures[m_place].get(), 280 * scale_w, 278 * scale_h);
        // env->canvas->draw(m_textures["place"].get(), 280 * scale_w, 278 * scale_h);
        env->canvas->draw(m_place, 345 * scale_w, 240 * scale_h, color);

        font->set_size(16);
        env->canvas->draw("This region is rich with Class A minerals and",
            350 * scale_w, 510 * scale_h, color);
        env->canvas->draw("other heavy metals, capable of boosting our",
            350 * scale_w, (510 + 17) * scale_h, color);
        env->canvas->draw("economy even further",
            350 * scale_w, (510 + 17*2) * scale_h, color);

        env->canvas->draw("Low", 412 * scale_w, 625 * scale_h, color);
        env->canvas->draw("None", 490 * scale_w, 625 * scale_h, color);
        env->canvas->draw("High", 573 * scale_w, 625 * scale_h, color);

        string time = m_settings->read<string>(m_place, "time", "0:00");
        if (time.front() == '0')
        {
            time = time.substr(1);
        }
        env->canvas->draw(time, 600 * scale_w, 250 * scale_h, color);
    }
}

bool
Planet::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        Colony *colony = dynamic_cast<Colony *>(sender);
        if (not colony)
        {
            return false;
        }

        set_next(id);
        finish();
        return true;
    }

    if (button->id() == "tundra")
    {
        m_place = "Tundra";
        enable_popup(true);
    }
    else if (button->id() == "jungle")
    {
        m_place = "Jungle";
        enable_popup(true);
    }
    else if (button->id() == "sea")
    {
        m_place = "Sea";
        enable_popup(true);
    }
    else if (button->id() == "dunes")
    {
        m_place = "Dunes";
        enable_popup(true);
    }
    else if (button->id() == "taiga")
    {
        m_place = "Taiga";
        enable_popup(true);
    }
    else if (button->id() == "lake")
    {
        m_place = "Lake";
        enable_popup(true);
    }
    else if (button->id() == "swamp")
    {
        m_place = "Swamp";
        enable_popup(true);
    }
    else if (button->id() == "cavern")
    {
        m_place = "Cavern";
        enable_popup(true);
    }
    else if (button->id() == "x")
    {
        enable_popup(false);
    }
    else if (button->id() == "send_party")
    {
        enable_popup(false);
        set_next("squad_planet");
        finish();
    }
    m_settings->write<string>("Mission", "current", m_place);
    m_settings->save("res/datas/slot" + to_string(m_slot) + "/planet.sav");
    return true;
}

void
Planet::enable_popup(bool popup)
{
    for (auto button : m_buttons)
    {
        if (button.first == "x" || button.first == "send_party")
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

bool
Planet::on_event(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED and
        event.key() == KeyboardEvent::ESCAPE)
    {
        set_next("base");
        finish();

        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);

        return true;
    }

    return false;
}
