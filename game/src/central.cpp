/*
 * Central class implemantation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "central.h"
#include "colony.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>
#include "timer.h"

#define W 1024.0
#define H 768.0

using std::to_string;

Central::Central(int slot, const string& next)
    : Level("central", next), m_slot(slot), m_screen(CHAT), m_scenario(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_scenario = env->resources_manager->get_texture(path + "central/central_chat_scenario.png");

    Colony *colony = new Colony(slot, this, "central");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Central::update_self(unsigned long elapsed)
{
    m_last = elapsed;
}

void
Central::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_scenario.get(), 275 * env->canvas->w() / W, 173 * env->canvas->h() / H);

    switch (m_screen)
    {
        case CHAT:
            change_to_chat();
            break;

        case QUESTS:
            change_to_quests();
            break;

        case BESTIARY:
            change_to_bestiary();
            break;

        case TIMERS:
            change_to_timers();
            break;
    }
}

bool
Central::on_message(Object *sender, MessageID id, Parameters)
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
    }
    else if (button->id() != "central")
    {
        Environment *env = Environment::get_instance();
        string path = "res/images/colony/";
        change_buttons();

        if (button->id() == "chat")
        {
            m_screen = CHAT;
            m_scenario = env->resources_manager->get_texture(
                path + "central/central_chat_scenario.png");
        }
        else if (button->id() == "quests")
        {
            m_screen = QUESTS;
            m_scenario = env->resources_manager->get_texture(
                path + "central/central_scenario.png");
        }
        else if (button->id() == "bestiary")
        {
            m_screen = BESTIARY;
            m_scenario = env->resources_manager->get_texture(
                path + "central/central_scenario.png");
        }
        else if (button->id() == "timers")
        {
            m_screen = TIMERS;
            m_scenario = env->resources_manager->get_texture(
                path + "central/central_scenario.png");
        }
        
        button->change_state(Button::ACTIVE);
    }

    return true;
}

void
Central::create_buttons()
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

    Button *button =  new Button(this, "central", path + "central_button.png",
        x, 218 * scale_h, w, h);
    button->set_sprites(3);
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "chat", path + "colony_small_button.png",
        x, 322 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Chat");
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "quests", path + "colony_small_button.png",
        x, 427 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Quests");

    m_buttons[button->id()] = button;

    button = new Button(this, "bestiary", path + "colony_small_button.png",
        x, 531 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Bestiary");

    m_buttons[button->id()] = button;

    button = new Button(this, "timers", path + "colony_small_button.png",
        x, 635 * scale_h, w, h);
    button->set_sprites(3);
    button->set_text("Timers");

    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        auto id = b.first;
        m_buttons[id]->add_observer(this);
        add_child(m_buttons[id]);
    }
}

void
Central::change_buttons()
{
    for (auto b : m_buttons)
    {
        if (b.first != "central")
        {
            b.second->change_state(Button::IDLE);
        }
    }
}

void
Central::change_to_chat()
{
    Environment *env = Environment::get_instance();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);
    Color color(170, 215, 190);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");
    auto sections = settings->sections();
    string text = sections["Central"]["welcome"];
    env->canvas->draw(text, (305 + 5) * scale_w, 605 * scale_h, color);
    env->canvas->draw(Rect(305 * scale_w, 605 * scale_h, 670 * scale_w, 116 * scale_h), color);
}

void
Central::change_to_quests()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Name", 360 * scale_w, 188 * scale_h, color);

    shared_ptr<Texture> texture;
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/quests.sav");
    auto sections = settings->sections();

    int y = 236;
    for (auto section : sections)
    {
        string name = section.first;
        string new_ = section.second["new"];

        env->canvas->draw(name, 360 * scale_w, y * scale_h, color);
        if (not new_.empty())
        {
            env->canvas->draw("NEW", 690 * scale_w,
                y * scale_h, color);
        }

        texture = env->resources_manager->get_texture(path + "big_list.png");
        Rect clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale_w, (y+5) * scale_h,
            602 * scale_w, 25 * scale_h);

        y += 64;
    }
}

void
Central::change_to_bestiary()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Name", 360 * scale_w, 188 * scale_h, color);
    
    shared_ptr<Texture> texture;
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/bestiary.sav");
    auto sections = settings->sections();

    int y = 236;
    for (auto section : sections)
    {
        string name = section.first;
        string icon = section.second["icon"];

        if (icon.back() == '\r')
        {
            icon.pop_back();
        }

        env->canvas->draw(name, 360 * scale_w, y * scale_h, color);
        if (not icon.empty())
        {
            texture = env->resources_manager->get_texture(path + "icons/" + icon + ".png");
            env->canvas->draw(texture.get(), 310 * scale_w, y * scale_h);
        }

        texture = env->resources_manager->get_texture(path + "big_list.png");
        Rect clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale_w,
            (y+5) * scale_h, 602 * scale_w, 25 * scale_h);

        y += 64;
    }
}

void
Central::change_to_timers()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Name", 360 * scale_w, 188 * scale_h, color);
    env->canvas->draw("Time", 855 * scale_w, 186 * scale_h, color);

    shared_ptr<Texture> texture;
    int y = 236;

    vector<Mission*> m = missions();
    for (auto mission : m)
    {
        env->canvas->draw(mission->name(), 360 * scale_w, y * scale_h, color);
        string str_remainder;
        if (mission->remainder())
        {
            unsigned long seconds = mission->remainder();
            unsigned long minutes = seconds / 60;
            seconds %= 60;

            string sec = seconds < 10 ? "0" + to_string(seconds) : to_string(seconds);
            string min = minutes < 10 ? "0" + to_string(minutes) : to_string(minutes);
            
            str_remainder = min + ":" + sec;
        }
        else
        {
            str_remainder = "Ok!";
        }

        env->canvas->draw(str_remainder, 855 * scale_w, y * scale_h, color);

        if (mission->icon() != "")
        {
            texture = env->resources_manager->get_texture(path + "icons/" +
                mission->icon() + ".png");
            env->canvas->draw(texture.get(), 310 * scale_w, y * scale_h);
        }

        texture = env->resources_manager->get_texture(path + "big_list.png");
        Rect clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale_w,
            (y+5) * scale_h, 602 * scale_w, 25 * scale_h);

        y += 64;
    }
}
