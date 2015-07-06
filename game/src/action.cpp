/*
 * Action class implementation
 *
 * Author: Tiamat
 * Date: 01/07/2015
 * License: LGPL. No copyright.
 */
#include "action.h"
#include "button.h"
#include "character.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/texture.h>

using std::to_string;

#define W 1024.0
#define H 768.0
#define ITEMS_PER_PAGE 5

Action::Action(int slot, Object *parent)
    : Object(parent), m_slot(slot), m_page(1), m_max_pages(0), m_mpt(MILITARY)
{
    parent->add_observer(this);
    load_textures();
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
            draw_item();
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

    if (not button)
    {
        return false;
    }

    if (id == Button::clickedID)
    {
        clicked_event(button);
    }
    else if (id == Button::hoverID)
    {
        hover_event(button);
    }

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
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    button = new Button(this, "reject", "res/images/combat/reject.png",
        672 * scale_w, 684 * scale_h, 25 * scale_w, 25 * scale_h);
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

    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/items.sav");

    auto sections = m_settings->sections();
    
    m_max_pages = (sections.size() / ITEMS_PER_PAGE) +
        (sections.size() % ITEMS_PER_PAGE != 0);

    y = 580;

    for (auto s : sections)
    {
        Button *button = new Button(this, s.first,
            545 * scale_w, y * scale_h, 195 * scale_w, 25 * scale_h);
        button->set_visible(false);
        button->set_active(false);
        m_items_buttons[button->id()] = button;

        button->add_observer(this);
        add_child(button);

        y += 30;
    }
}

void
Action::change_buttons()
{
    for (auto b : m_buttons)
    {
        bool active = m_state == ATTACK and b.first == "attack";
        active = active or (m_state == SKILL and b.first == "skill");
        active = active or (m_state == ITEM and b.first == "item");
        active = active or (m_state == DEFENSE and b.first == "defense");
        active = active or (m_state == REST and b.first == "rest");
        active = active or (m_state == RUN and b.first == "run");

        if (active)
        {
            b.second->change_state(Button::ACTIVE);
        }
        else
        {
            b.second->change_state(Button::IDLE);
        }
    }
}

void
Action::set_current_character(Character *character)
{
    m_character = character;
}

void
Action::draw_skill()
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int x, y, w, h;
    x = 530 * scale_w;
    y = 582 * scale_h;
    w = 38 * scale_w;
    h = 37 * scale_h;
    string skill = "skill";
    int hability = 0;

    switch (m_mpt)
    {
        case MILITARY:
            env->canvas->draw(m_textures["bracket_m"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["military"].get(), 548 * scale_w, 532 * scale_h);
            skill += "_m_";
            hability = m_character->military();
            break;
        case PSIONIC:
            env->canvas->draw(m_textures["bracket_p"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["psionic"].get(), 548 * scale_w, 532 * scale_h);
            skill += "_p_";
            hability = m_character->psionic();
            break;
        case TECH:
            env->canvas->draw(m_textures["bracket_t"].get(), 528 * scale_w, 522 * scale_h);
            env->canvas->draw(m_textures["tech"].get(), 548 * scale_w, 532 * scale_h);
            skill += "_t_";
            hability = m_character->tech();
            break;
    }

    for (int i = 1; i <= 4; ++i, y += h)
    {
        x = 546 * scale_w;
        for (int j = 1; j <= 5; ++j, x += w)
        {
            if ((i - 1) * 5 + j <= hability)
            {
                env->canvas->draw(m_textures[skill + to_string((i - 1)*5 + j)].get(), x, y);
            }
            else
            {
                env->canvas->draw(m_textures[skill + "locked"].get(), x, y);
            }
        }
    }
}

void
Action::draw_item()
{
    Environment *env = Environment::get_instance();
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    font->set_size(14);
    env->canvas->set_font(font);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["item_shelf"].get(), 528 * scale_w, 522 * scale_h);

    int y = 580, i = 0;
    for (auto section : m_settings->sections())
    {
        if (++i < (m_page - 1) * ITEMS_PER_PAGE or i > ITEMS_PER_PAGE * m_page)
        {
            change_button_state(m_items_buttons[section.first], false);
            continue;
        }

        change_button_state(m_items_buttons[section.first], true, y);

        string qnt_earned = m_settings->read<string>(section.first, "qnt_earned", "");


        env->canvas->draw(m_textures["research"].get(),
            540 * scale_w, y * scale_h, 50 * scale_w, 25 * scale_h);

        env->canvas->draw(section.first, 590 * scale_w, (y+5) * scale_h, color);
        env->canvas->draw(qnt_earned + "/" + section.second["qnt_max"],
            710 * scale_w, (y+5) * scale_h, color);

        y += 30;
    }

    if (m_hover_button != "" and m_items_buttons[m_hover_button]->state() == Button::ON_HOVER)
    {
        env->canvas->draw(m_textures["inspect_item"].get(),
            525 * scale_w, 382 * scale_h);
        env->canvas->draw(m_hover_button, 550 * scale_w, 400 * scale_h, color);
    }

    // TODO not work
    font->set_size(18);
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

void
Action::load_textures()
{
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
    m_textures["item_shelf"] = env->resources_manager->get_texture(path + "item_shelf.png");
    m_textures["inspect_item"] = env->resources_manager->get_texture(path +
        "inspect_item.png");
    m_textures["inspect_military"] = env->resources_manager->get_texture(path +
        "inspect_military.png");
    m_textures["inspect_psionic"] = env->resources_manager->get_texture(path +
        "inspect_psionic.png");
    m_textures["inspect_tech"] = env->resources_manager->get_texture(path + "inspect_tech.png");

    path = "res/images/colony/barracks/";
    m_textures["skill_m_locked"] = env->resources_manager->get_texture(path + "Skill_M_Locked.png");
    m_textures["skill_p_locked"] = env->resources_manager->get_texture(path + "Skill_P_Locked.png");
    m_textures["skill_t_locked"] = env->resources_manager->get_texture(path + "Skill_T_Locked.png");

    for (int i = 1; i <= 20; ++i)
    {
        string index = to_string(i);
        m_textures["skill_m_" + index] = env->resources_manager->get_texture(path + "Skill_M_" +
            index + ".png");
        m_textures["skill_p_" + index] = env->resources_manager->get_texture(path + "Skill_P_" +
            index + ".png");
        m_textures["skill_t_" + index] = env->resources_manager->get_texture(path + "Skill_T_" +
            index + ".png");
    }

    path = "res/images/colony/icons/";
    m_textures["research"] = env->resources_manager->get_texture(path + "research.png");

}

void
Action::change_button_state(Button *button, bool state, int y)
{
    button->set_visible(state);
    button->set_active(state);

    if (state)
    {
        Environment *env = Environment::get_instance();
        button->set_y((y+5) * env->canvas->h() / H);
    }
}

void
Action::active_buttons(ActionState state)
{

    for (auto s : m_settings->sections())
    {
        bool active = m_items_buttons[s.first]->visible();
        m_items_buttons[s.first]->set_active(state == ITEM and active);
        m_items_buttons[s.first]->set_visible(state == ITEM and active);
    }
}

void
Action::hover_event(Button *button)
{
    for (auto section : m_settings->sections())
    {
        if (button->id() == section.first)
        {
            m_hover_button = button->id();
        }
    }
}

void
Action::clicked_event(Button *button)
{
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

    m_buttons["left_arrow"]->set_visible(m_state == SKILL or m_state == ITEM);
    m_buttons["left_arrow"]->set_active(m_state == SKILL or m_state == ITEM);

    m_buttons["right_arrow"]->set_visible(m_state == SKILL or m_state == ITEM);
    m_buttons["right_arrow"]->set_active(m_state == SKILL or m_state == ITEM);

    bool confirm = m_state == DEFENSE or m_state == REST or m_state == RUN;
    m_buttons["confirm"]->set_visible(confirm);
    m_buttons["confirm"]->set_active(confirm);
    m_buttons["reject"]->set_visible(confirm);
    m_buttons["reject"]->set_active(confirm);

    if (button->id() == "left_arrow")
    {
        if (m_state == SKILL)
        {
            int pos = m_mpt - 1;
            pos = pos < 0 ? 2 : pos;
            m_mpt = static_cast<SkillState>(pos);
        }
        else
        {
            if (m_page > 1)
            {
                m_page--;
            }
        }
    }
    else if (button->id() == "right_arrow")
    {
        if (m_state == SKILL)
        {
            int pos = (m_mpt + 1) % 3;
            m_mpt = static_cast<SkillState>(pos);
        }
        else
        {
            if (m_page < m_max_pages)
            {
                m_page++;
            }
        }
    }

    change_buttons();
    active_buttons(m_state);
}
