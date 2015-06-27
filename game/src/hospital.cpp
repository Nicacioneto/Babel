/*
 * Hospital class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "colony.h"
#include "hospital.h"
#include "item.h"
#include "research.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>
#include <core/text.h>
#include <cstdlib>
#include <iostream>

#define W 1024.0
#define H 768.0

using std::to_string;

Hospital::Hospital(int slot, const string& next)
    : Level("hospital", next), m_slot(slot), m_colony(nullptr), m_screen("chat"),
    m_scenario(nullptr), m_page(1), m_max_pages(1), m_text(nullptr)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/hospital/";

    m_scenario = env->resources_manager->get_texture(path + "chat_scenario.png");

    m_colony = new Colony(slot, this, "hospital");
    m_colony->add_observer(this);
    add_child(m_colony);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);

    set_pages_text();
    create_buttons();

    Item *item = new Item(m_slot, m_colony, this);
    item->add_observer(this);
    add_child(item);
    item->set_visible(false);

    Research *research = new Research(m_slot, this);
    research->add_observer(this);
    add_child(research);
    research->set_visible(false);
}

void
Hospital::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_scenario.get(), 275 * env->canvas->w() / W, 173 * env->canvas->h() / H);

    if (m_text and m_screen != "chat")
    {
        m_text->draw();
    }

    if (m_screen == "chat")
        chat_screen();
    else if (m_screen == "revive")
        revive_screen();
}

bool
Hospital::on_message(Object *sender, MessageID id, Parameters p)
{
    if (id == "max_pages")
    {
        m_max_pages = atoi(p.c_str());
        set_pages_text();
        return true;
    }

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

    Environment *env = Environment::get_instance();
    string path = "res/images/colony/hospital/";

    m_scenario = env->resources_manager->get_texture(path + "scenario.png");

    if (button->id() == "left_arrow")
    {
        if (m_page > 1)
        {
            m_page--;
        }
        notify(m_screen, to_string(m_page));
    }
    else if (button->id() == "right_arrow")
    {
        if (m_page < m_max_pages)
        {
            m_page++;
        }
        notify(m_screen, to_string(m_page));
    }
    else
    {
        if (button->id() == "chat")
        {
            m_scenario = env->resources_manager->get_texture(path + "chat_scenario.png");
        }

        m_screen = button->id();
        notify(m_screen, "");
        
        m_page = 1;
        change_buttons();
        button->change_state(Button::ACTIVE);
    }

    return true;
}

void
Hospital::create_buttons()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(24);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int x = 28 * scale_w;
    int w = 190 * scale_w;
    int h = 180/3 * scale_h;

    Button *button =  new Button(this, "hospital", path + "hospital_button.png",
        x, 218 * scale_h, w, h);
    button->set_sprites(3);
    button->change_state(Button::ACTIVE);
    button->set_active(false);

    m_buttons[button->id()] = button;

    button = new Button(this, "chat", path + "colony_small_button.png",
        x, 322 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Chat");
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "items", path + "colony_small_button.png",
        x, 427 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Items");

    m_buttons[button->id()] = button;

    button = new Button(this, "research", path + "colony_small_button.png",
        x, 531 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Research");

    m_buttons[button->id()] = button;

    button = new Button(this, "revive", path + "colony_small_button.png",
        x, 635 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Revive");

    m_buttons[button->id()] = button;

    x = m_text->x() - 30 * scale_w;
    
    button = new Button(this, "left_arrow", path + "left_arrow.png",
        x, m_text->y(), 20 * scale_w, m_text->h());
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);

    m_buttons[button->id()] = button;
    
    x = m_text->x() + m_text->w() + 10 * scale_w;

    button = new Button(this, "right_arrow", path + "right_arrow.png",
        x, m_text->y(), 20 * scale_w, m_text->h());
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
Hospital::change_buttons()
{
    m_buttons["left_arrow"]->set_active(m_screen != "chat");
    m_buttons["left_arrow"]->set_visible(m_screen != "chat");

    m_buttons["right_arrow"]->set_active(m_screen != "chat");
    m_buttons["right_arrow"]->set_visible(m_screen != "chat");

    for (auto b : m_buttons)
    {
        if (b.first != "hospital")
        {
            b.second->change_state(Button::IDLE);
        }
    }
}

void
Hospital::chat_screen()
{
    Environment *env = Environment::get_instance();

    Color color(170, 215, 190);
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");
    auto sections = settings->sections();
    string text = sections["Hospital"]["welcome"];
    
    env->canvas->draw(text, (305 + 5) * scale_w, 605 * scale_h, color);
    env->canvas->draw(Rect(305 * scale_w, 605 * scale_h, +670 * scale_w, 116 * scale_h), color);
}

void
Hospital::research_screen()
{
    string path = "res/images/colony/";
    Color color(170, 215, 190);

    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Name", 360 * scale_w, 188 * scale_h, color);
    env->canvas->draw("Time", 855 * scale_w, 186 * scale_h, color);

    shared_ptr<Texture> texture = env->resources_manager->get_texture(
        path + "icons/matter_energy.png");
    env->canvas->draw(texture.get(), 690 * scale_w, 188 * scale_h);

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/items.sav");
    auto sections = settings->sections();
    update_max_pages(sections.size());

    int y = 236;
    int i = -1;
    for (auto section : sections)
    {
        i++;
        if (i < (m_page - 1) * BIG_LIST or i >= BIG_LIST * m_page)
        {
            continue;
        }

        string name = section.first;
        string matter = section.second["matter"];
        string energy = section.second["energy"];
        string time = section.second["time"];

        env->canvas->draw(name, 360 * scale_w, y * scale_h, color);
        if (not matter.empty())
        {
            env->canvas->draw(matter + "/" + energy, 690 * scale_w, y * scale_h, color);
        }
        if (not time.empty())
        {
            env->canvas->draw(time, 855 * scale_w, y * scale_h, color);
        }

        texture = env->resources_manager->get_texture(path + "icons/health.png");
        Rect clip = Rect(0, 25, 50, 50/2);
        env->canvas->draw(texture.get(), clip, 310 * scale_w,
            y * scale_h, 50 * scale_w, 25 * scale_h);

        texture = env->resources_manager->get_texture(path + "big_list.png");
        clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale_w,
            (y+5) * scale_h, 602 * scale_w, 25 * scale_h);

        y += 64;
    }
}

void
Hospital::revive_screen()
{
    string path = "res/images/colony/";
    Color color(170, 215, 190);

    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Name", 360 * scale_w, 188 * scale_h, color);
    env->canvas->draw("Class", 524 * scale_w, 186 * scale_h, color);
    env->canvas->draw("Time", 855 * scale_w, 186 * scale_h, color);

    shared_ptr<Texture> texture = env->resources_manager->get_texture(
        path + "icons/matter_energy.png");
    env->canvas->draw(texture.get(), 690 * scale_w, 188 * scale_h);

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/characters.sav");
    auto sections = settings->sections();
    update_max_pages(sections.size());

    int y = 236;
    int i = -1;
    for (auto section : sections)
    {
        i++;
        if (i < (m_page - 1) * BIG_LIST or i >= BIG_LIST * m_page)
        {
            continue;
        }
        
        string name = section.first;

        if (name == "Default")
        {
            continue;
        }

        string class_ = section.second["class"];
        string matter = section.second["matter"];
        string energy = section.second["energy"];
        string time = section.second["time"];

        env->canvas->draw(name, 360 * scale_w, y * scale_h, color);
        env->canvas->draw(class_, 524 * scale_w, y * scale_h, color);
        if (not matter.empty())
        {
            env->canvas->draw(matter + "/" + energy, 690 * scale_w, y * scale_h, color);
        }
        if (not time.empty())
        {
            env->canvas->draw(time, 855 * scale_w, y * scale_h, color);
        }

        texture = env->resources_manager->get_texture(path + "icons/health.png");
        Rect clip = Rect(0, 25, 50, 50/2);
        env->canvas->draw(texture.get(), clip, 310 * scale_w,
            y * scale_h, 50 * scale_w, 25 * scale_h);

        texture = env->resources_manager->get_texture(path + "big_list.png");
        clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale_w,
            (y+5) * scale_h, 602 * scale_w, 25 * scale_h);

        y += 64;
    }
}

void
Hospital::update_max_pages(int sections)
{
    m_max_pages = (sections / BIG_LIST) + (sections % BIG_LIST != 0);
    set_pages_text();
}

void
Hospital::set_pages_text()
{
    if (m_text)
    {
        delete m_text;
    }

    Environment *env = Environment::get_instance();
    m_text = new Text(this, to_string(m_page) + "/" + to_string(m_max_pages), Color(170, 215, 190));

    double x = (env->canvas->w() - m_text->w()) / 2 + (275  * env->canvas->w() / W) / 2;
    double y = env->canvas->h() - 100 * env->canvas->h() / H;
    m_text->set_position(x, y);
}
