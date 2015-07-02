/*
 * Action class implementation
 *
 * Author: Tiamat
 * Date: 01/07/2015
 * License: LGPL. No copyright.
 */
#include "button.h"
#include "action.h"
#include <core/rect.h>
#include <core/texture.h>

using std::to_string;

#define W 1024.0
#define H 768.0

Action::Action(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_mpt(MILITARY)
{
    parent->add_observer(this);

    Environment *env = Environment::get_instance();
    string path = "res/images/combat/";

    m_textures["bracket"] = env->resources_manager->get_texture(path + "actions.png");

    m_textures["military"] = env->resources_manager->get_texture(path + "military.png");
    m_textures["psionic"] = env->resources_manager->get_texture(path + "psionic.png");
    m_textures["tech"] = env->resources_manager->get_texture(path + "tech.png");
    m_textures["bracket_m"] = env->resources_manager->get_texture(path +
        "bracket_m.png");
    m_textures["bracket_p"] = env->resources_manager->get_texture(path +
        "bracket_p.png");
    m_textures["bracket_t"] = env->resources_manager->get_texture(path + "bracket_t.png");
    m_textures["action_shelf"] = env->resources_manager->get_texture(path + "action_shelf.png");
    m_textures["icon_defense"] = env->resources_manager->get_texture(path + "icon_defense.png");
    m_textures["icon_rest"] = env->resources_manager->get_texture(path + "icon_rest.png");
    m_textures["icon_run"] = env->resources_manager->get_texture(path + "icon_run.png");

    create_buttons();
}

void
Action::draw_self()
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["bracket"].get(), 278 * scale_w, 620 * scale_h);

    switch (m_state)
    {
        case ATTACK:
            break;
        case SKILL:
            draw_skill();
            break;
        case ITEM:
            break;
        case DEFENSE:
            draw_confirm_box("defense");
            break;
        case REST:
            draw_confirm_box("rest");
            break;
        case RUN:
            draw_confirm_box("run");
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

    if (button->id() == "attack")
    {
        m_state = ATTACK;
    }
    else if (button->id() == "skill")
    {
        m_state = SKILL;
    }
    else if (button->id() == "item")
    {
        m_state = ITEM;
    }
    else if (button->id() == "defense")
    {
        m_state = DEFENSE;
    }
    else if (button->id() == "rest")
    {
        m_state = REST;
    }
    else if (button->id() == "run")
    {
        m_state = RUN;
    }

    m_buttons["left_arrow"]->set_visible(m_state == SKILL);
    m_buttons["left_arrow"]->set_active(m_state == SKILL);

    m_buttons["right_arrow"]->set_visible(m_state == SKILL);
    m_buttons["right_arrow"]->set_active(m_state == SKILL);

    bool confirm = m_state == DEFENSE or m_state == REST or m_state == RUN;
    m_buttons["confirm"]->set_visible(confirm);
    m_buttons["confirm"]->set_active(confirm);
    m_buttons["reject"]->set_visible(confirm);
    m_buttons["reject"]->set_active(confirm);

    if (button->id() == "left_arrow")
    {
        int pos = m_mpt - 1;
        pos = pos < 0 ? 2 : pos;
        m_mpt = static_cast<SkillState>(pos);
    }
    else if (button->id() == "right_arrow")
    {
        int pos = (m_mpt + 1) % 3;
        m_mpt = static_cast<SkillState>(pos);
    }
    else
    {
        change_buttons();
    }


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


    int x = 294, y = 630;
    double w = 58 * scale_w;
    double h = 120 / 3 * scale_h;

    Button *button = new Button(this, "attack", path + "attack.png",
        x * scale_w, y * scale_h, w, h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "defense", path + "defense.png",
        x * scale_w, (y + 60) * scale_h, w, h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    x += 71;
    button = new Button(this, "skill", path + "skill.png",
        x * scale_w, y * scale_h, w, h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "rest", path + "rest.png",
        x * scale_w, (y + 60) * scale_h, w, h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    x += 71;
    button = new Button(this, "item", path + "item.png",
        x * scale_w, y * scale_h, w, h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "run", path + "run.png",
        x * scale_w, (y + 60) * scale_h, w, h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "confirm", "res/images/tower/squad/confirm.png",
        600 * scale_w, 684 * scale_h, 25 * scale_w, 25 * scale_h);
    button->set_sprites(2);
    m_buttons[button->id()] = button;
    button->set_active(false);
    button->set_visible(false);

    m_buttons[button->id()] = button;

    button = new Button(this, "reject", "res/images/combat/reject.png",
        672 * scale_w, 684 * scale_h, 25 * scale_w, 25 * scale_h);
    button->set_sprites(2);
    m_buttons[button->id()] = button;
    button->set_active(false);
    button->set_visible(false);

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

void
Action::draw_attack()
{

}

void
Action::draw_skill()
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    switch (m_mpt)
    {
        case MILITARY:
            env->canvas->draw(m_textures["bracket_m"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["military"].get(), 548 * scale_w, 532 * scale_h);
            break;
        case PSIONIC:
            env->canvas->draw(m_textures["bracket_p"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["psionic"].get(), 548 * scale_w, 532 * scale_h);
            break;
        case TECH:
            env->canvas->draw(m_textures["bracket_t"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["tech"].get(), 548 * scale_w, 532 * scale_h);
            break;
        default:
            break;
    }
}

void
Action::draw_confirm_box(string icon)
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["action_shelf"].get(), 528 * scale_w, 620 * scale_h);
    env->canvas->draw(m_textures["icon_" + icon].get(), Rect(0, 40, 58, 40), 620 * scale_w,
        634 * scale_h, 58 * scale_w, 40 * scale_h);
}
