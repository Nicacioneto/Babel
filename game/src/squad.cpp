/*
 * Squad class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "squad.h"
#include "character.h"
#include "colony.h"
#include <algorithm>
#include <core/font.h>
#include <core/keyboardevent.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Squad::Squad(int slot, const string& next)
    : Level("Squad", next), m_slot(slot), m_character(0), m_screen(SQUAD), m_settings(nullptr)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/tower/squad/";

    env->events_manager->register_listener(this);

    load_characters();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button = new Button(this, "confirm_choice", path + "confirm.png",
        470 * scale_w, 622 * scale_h, 25 * scale_w, 25 * scale_h);
    button->set_sprites(2);
    m_buttons[button->id()] = button;

    button = new Button(this, "reset_choice", path + "change.png",
        530 * scale_w, 622 * scale_h, 25 * scale_w, 25 * scale_h);
    button->set_sprites(2);
    m_buttons[button->id()] = button;

    button = new Button(this, "select_squad", path + "select_squad.png",
        300 * scale_w, 675 * scale_h, 100 * scale_w, 18 * scale_h);
    button->set_sprites(4);
    button->change_state(Button::ACTIVE);
    m_buttons[button->id()] = button;

    button = new Button(this, "select_drone", path + "select_drone.png",
        465 * scale_w, 675 * scale_h, 100 * scale_w, 18 * scale_h);
    button->set_sprites(4);
    m_buttons[button->id()] = button;

    button = new Button(this, "confirm", path + "select_confirm.png",
        645 * scale_w, 675 * scale_h, 60 * scale_w, 18 * scale_h);
    button->set_sprites(4);
    m_buttons[button->id()] = button;

    button = new Button(this, "back", path + "back.png",
        912 * scale_w, 55 * scale_h, 67 * scale_w, 26 * scale_h);
    button->set_sprites(1);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}

Squad::~Squad()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
Squad::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    Color color(70, 89, 79);

    string path = "res/images/colony/barracks/";
    shared_ptr<Texture> texture = env->resources_manager->get_texture(path + "bracket.png");
    env->canvas->draw(texture.get(), 30 * scale_w, 25 * scale_h);
    env->canvas->draw("Confirm Choice", 345 * scale_w, 630 * scale_h, color);
    env->canvas->draw("Reset Choice", 565 * scale_w, 630 * scale_h, color);

    switch (m_screen)
    {
        case SQUAD:
            draw_squad();
            break;
        case DRONE:
            draw_drone();
            break;
    }
}

bool
Squad::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "select_squad")
    {
        m_screen = SQUAD;
        m_buttons["select_drone"]->change_state(Button::IDLE);
        button->change_state(Button::ACTIVE);
    }
    else if (button->id() == "select_drone")
    {
        m_screen = DRONE;
        m_buttons["select_squad"]->change_state(Button::IDLE);
        button->change_state(Button::ACTIVE);
    }
    else if (button->id() == "confirm_choice")
    {
        if (m_screen == SQUAD)
        {
            confirm_squad();
        }
    }
    else if (button->id() == "reset_choice")
    {
        reset_choice();
    }
    else if (button->id() == "back")
    {
        set_next("tower");
        finish();
    }
    else
    {
        for (auto c : m_characters)
        {
            if (button->id() == c.first)
            {
                if (button->visible() and m_squad.size() < 4) // not selected
                {
                    m_squad.push_back(c.first);
                    button->set_visible(false);
                }
                else
                {
                    m_squad.erase(remove(m_squad.begin(), m_squad.end(), c.first), m_squad.end());
                    button->set_visible(true);
                }

                break;
            }
        }
    }

    return true;
}

void
Squad::reset_choice()
{
    for (auto c : m_characters)
    {
        m_buttons[c.first]->set_visible(true);
        m_squad.clear();
    }
}

void
Squad::confirm_squad()
{
    Environment *env = Environment::get_instance();
    string path = "res/datas/slot" + to_string(m_slot) + "/squad.sav";
    shared_ptr<Settings> settings = env->resources_manager->get_settings(path);

    int i = 1;
    for (auto id : m_squad)
    {
        settings->write<string>("Squad", "hero" + to_string(i++), id);
    }

    settings->save(path);
}

void
Squad::load_characters()
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/squad/";

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/characters.sav");
    auto sections = settings->sections();

    int x = 155, y = 175, w = 222, h = 123;
    int i = 0, j = 0;
    for (auto section : sections)
    {
        if (j > 2)
        {
            ++i; j = 0;
        }

        if (i > 2)
        {
            break;
        }

        if (section.first != "Default")
        {
            Character *character = new Character(m_slot, this, section.first, "albert.png",
                (x + 249*j) * scale_w, (y + 150*i) * scale_h,
                w * scale_w, h * scale_h, section.first);
            character->set_active(false);

            character->add_observer(this);
            add_child(character);
            character->set_active(false);
            m_characters[character->id()] = character;

            Button *button = new Button(this, section.first, (x + 249*j) * scale_w,
                (y + 150*i) * scale_h, w * scale_w, h * scale_h, Color(0, 0, 0, 128));

            m_buttons[button->id()] = button;

            ++j;
        }
    }
}

void
Squad::draw_squad()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    Color color(70, 89, 79);

    env->canvas->draw("Select Squad", 60 * scale_w, 50 * scale_h, color);
    env->canvas->draw("Click to add/remove", 60 * scale_w, 80 * scale_h, color);
    
}

void
Squad::draw_drone()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    Color color(70, 89, 79);

    env->canvas->draw("Select Drone", 60 * scale_w, 50 * scale_h, color);
    env->canvas->draw("Click to add/remove", 60 * scale_w, 80 * scale_h, color);
}

bool
Squad::on_event(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED
        and event.key() == KeyboardEvent::ESCAPE)
    {
        set_next("base");
        finish();

        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);

        return true;
    }

    return false;
}
