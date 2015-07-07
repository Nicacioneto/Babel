/*
 * Base class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "base.h"
#include "colony.h"
#include <core/font.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

Base::Base(int slot, const string& next)
    : Level("base", next), m_slot(slot)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_textures["scenario"] = env->resources_manager->get_texture(path + "colony_scenario.png");
    m_textures["right_bracket"] = env->resources_manager->get_texture(path + "right_bracket.png");
    m_textures["left_bracket"] = env->resources_manager->get_texture(path + "left_bracket.png");

    Colony *colony = new Colony(slot, this, "base");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Base::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["scenario"].get(), 275 * scale_w, 173 * scale_h);
    env->canvas->draw(m_textures["right_bracket"].get(), 275 * scale_w, 173 * scale_h);
    env->canvas->draw(m_textures["left_bracket"].get(), 28 * scale_w, 175 * scale_h);
}

bool
Base::on_message(Object *sender, MessageID id, Parameters)
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
    }
    else
    {
        if (button->id() == "barracks")
        {
            Environment *env = Environment::get_instance();
            shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
                std::to_string(m_slot) + "/colony.sav");
            settings->write<string>("Barracks", "prev", "base");
            settings->save("res/datas/slot" + std::to_string(m_slot) + "/colony.sav");
        }

        set_next(button->id());
    }
    
    finish();
    return true;
}

void
Base::create_buttons()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    const int x = 28 * scale_w;
    const int w = 190 * scale_w;
    const int h = 180/3 * scale_h;

    Button *button = new Button(this, "barracks", path + "barracks_button.png",
        x, 218 * scale_h, w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "facilities", path + "facilities_button.png",
        x, 322 * scale_h, w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "hospital", path + "hospital_button.png",
        x, 427 * scale_h, w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "workshop", path + "workshop_button.png",
        x, 531 * scale_h, w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "central", path + "central_button.png",
        x, 635 * scale_h, w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}
