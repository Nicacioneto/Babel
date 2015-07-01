/*
 * Action class implementation
 *
 * Author: Tiamat
 * Date: 01/07/2015
 * License: LGPL. No copyright.
 */
#include "button.h"
#include "action.h"
// #include <core/color.h>
// #include <core/font.h>
// #include <core/rect.h>
// #include <core/settings.h>
#include <core/texture.h>

using std::to_string;

#define W 1024.0
#define H 768.0

Action::Action(int slot, Object *parent)
    : Object(parent), m_slot(slot)
{
    parent->add_observer(this);

    Environment *env = Environment::get_instance();
    string path = "res/images/combat/";

    m_textures["bracket"] = env->resources_manager->get_texture(path + "actions.png");

    // m_textures["health"] = env->resources_manager->get_texture(path + "/icons/health.png");

    create_buttons();

    // m_font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    // env->canvas->set_font(m_font);
}

void
Action::draw_self()
{
    Environment *env = Environment::get_instance();
    // Color color(170, 215, 190);
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["bracket"].get(), 278 * scale_w, 620 * scale_h);

    // m_font->set_size(18);
}

bool
Action::on_message(Object *sender, MessageID id, Parameters p)
{
    return false;
}

void
Action::create_buttons()
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    string path = "res/images/combat/icon_";


    int x = 304, y = 630;

    Button *button = new Button(this, "attack", path + "attack.png",
        x * scale_w, y * scale_h, 46 * scale_w, 126 / 3 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "defense", path + "defense.png",
        x * scale_w, (y + 60) * scale_h, 46 * scale_w, 126 / 3 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    x += 71;
    button = new Button(this, "skill", path + "skill.png",
        375 * scale_w, y * scale_h, 46 * scale_w, 126 / 3 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "rest", path + "rest.png",
        x * scale_w, (y + 60) * scale_h, 46 * scale_w, 126 / 3 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    x += 71;
    button = new Button(this, "item", path + "item.png",
        x * scale_w, y * scale_h, 46 * scale_w, 126 / 3 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "run", path + "run.png",
        x * scale_w, (y + 60) * scale_h, 46 * scale_w, 126 / 3 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}
