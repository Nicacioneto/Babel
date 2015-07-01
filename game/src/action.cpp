/*
 * Action class implementation
 *
 * Author: Tiamat
 * Date: 01/07/2015
 * License: LGPL. No copyright.
 */
#include "button.h"
#include "action.h"
#include <core/texture.h>

using std::to_string;

#define W 1024.0
#define H 768.0

Action::Action(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_mpt(NONE)
{
    parent->add_observer(this);

    Environment *env = Environment::get_instance();
    string path = "res/images/combat/";

    m_textures["bracket"] = env->resources_manager->get_texture(path + "actions.png");

    m_textures["military"] = env->resources_manager->get_texture(path + "military.png");
    m_textures["psionic"] = env->resources_manager->get_texture(path + "psionic.png");
    m_textures["tech"] = env->resources_manager->get_texture(path + "tech.png");
    m_textures["bracket-m"] = env->resources_manager->get_texture(path + "bracket-m.png");
    m_textures["bracket-p"] = env->resources_manager->get_texture(path + "bracket-p.png");
    m_textures["bracket-t"] = env->resources_manager->get_texture(path + "bracket-t.png");

    create_buttons();
}

void
Action::draw_self()
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["bracket"].get(), 278 * scale_w, 620 * scale_h);

    switch (m_mpt)
    {
        case MILITARY:
            env->canvas->draw(m_textures["bracket-m"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["military"].get(), 548 * scale_w, 532 * scale_h);
            break;
        case PSIONIC:
            env->canvas->draw(m_textures["bracket-p"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["psionic"].get(), 548 * scale_w, 532 * scale_h);
            break;
        case TECH:
            env->canvas->draw(m_textures["bracket-t"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["tech"].get(), 548 * scale_w, 532 * scale_h);
            break;
        default:
            break;
    }
}

bool
Action::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (m_mpt == NONE)
    {
        m_mpt = MILITARY;
    }

    m_buttons["left_arrow"]->set_visible(true);
    m_buttons["left_arrow"]->set_active(true);

    m_buttons["right_arrow"]->set_visible(true);
    m_buttons["right_arrow"]->set_active(true);


    if (button->id() == "left_arrow")
    {
        int pos = m_mpt - 1;
        pos = pos < 0 ? 2 : pos;
        m_mpt = static_cast<State>(pos);
    }
    else if (button->id() == "right_arrow")
    {
        int pos = (m_mpt + 1) % 3;
        m_mpt = static_cast<State>(pos);
    }

    change_buttons();

    button->change_state(Button::ACTIVE);

    return true;
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

    path = "res/images/colony/";

    button = new Button(this, "left_arrow", path + "left_arrow.png",
        498 * scale_w, 600 * scale_h, 20 * scale_w, 20 * scale_h);
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);

    m_buttons[button->id()] = button;

    button = new Button(this, "right_arrow", path + "right_arrow.png",
        760 * scale_w, 600 * scale_h, 20 * scale_w, 20 * scale_h);
    button->set_sprites(1);
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
Action::change_buttons()
{
    for (auto b : m_buttons)
    {
        if (b.first != "left_arrow" and
            b.first != "right_arrow")
        {
            b.second->change_state(Button::IDLE);
        }
    }
}