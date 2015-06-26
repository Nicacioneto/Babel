/*
 * Hospital class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "colony.h"
#include "hospital.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Hospital::Hospital(int slot, const string& next)
    : Level("hospital", next), m_slot(slot), m_colony(nullptr), m_screen(CHAT), m_scenario(nullptr)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/hospital/";

    m_scenario = env->resources_manager->get_texture(path + "chat_scenario.png");

    m_colony = new Colony(slot, this, "hospital");
    m_colony->add_observer(this);
    add_child(m_colony);

    create_buttons();
}

void
Hospital::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_scenario.get(), 275 * env->canvas->w() / W, 173 * env->canvas->h() / H);

    switch (m_screen)
    {
        case CHAT:
            chat_screen();
            break;

        case ITEMS:
            items_screen();
            break;

        case RESEARCH:
            research_screen();
            break;

        case REVIVE:
            revive_screen();
            break;
    }
}

bool
Hospital::on_message(Object *sender, MessageID id, Parameters)
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
    else if (button->id() != "hospital")
    {
        Environment *env = Environment::get_instance();
        string path = "res/images/colony/hospital/";

        m_scenario = env->resources_manager->get_texture(path + "scenario.png");

        if (button->id() == "chat")
        {
            m_screen = CHAT;
            m_scenario = env->resources_manager->get_texture(path + "chat_scenario.png");
        }
        else if (button->id() == "items")
        {
            m_screen = ITEMS;
        }
        else if (button->id() == "research")
        {
            m_screen = RESEARCH;
        }
        else if (button->id() == "revive")
        {
            m_screen = REVIVE;
        }

        if (m_items.find(button->id()) == m_items.end())
        {
            change_buttons();
            button->change_state(Button::ACTIVE);
        }
        else
        {
            buy_item(button->id());
        }

        change_items();
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

    const int x = 28 * scale_w;
    const int w = 190 * scale_w;
    const int h = 180/3 * scale_h;

    Button *button =  new Button(this, "hospital", path + "hospital_button.png",
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

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    create_items();
}

void
Hospital::create_items()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/items.sav");
    auto sections = settings->sections();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    int y = 236;

    for(auto s : sections)
    {
        Button *button = new Button(this, s.first, "res/images/colony/big_list.png",
            310 * scale_w, (y+5) * scale_h, 602 * scale_w, 25 * scale_h);
        button->set_active(false);
        button->set_visible(false);

        m_items[button->id()] = button;

        button->add_observer(this);
        add_child(button);

        y += 64;
    }
}

void
Hospital::change_buttons()
{
    for (auto b : m_buttons)
    {
        if (b.first != "hospital")
        {
            b.second->change_state(Button::IDLE);
        }
    }
}

void
Hospital::change_items()
{
    bool visible = m_screen == ITEMS or m_screen == RESEARCH;
    bool active = m_screen == ITEMS;

    for (auto item : m_items)
    {
        item.second->set_active(active);
        item.second->set_visible(visible);
    }
}

void
Hospital::buy_item(const ObjectID id)
{
    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/items.sav");

    int qnt_earned = settings->read<int>(id, "qnt_earned", 0);
    int qnt_max = settings->read<int>(id, "qnt_max", 0);

    int matter = m_colony->matter() - settings->read<int>(id, "matter", 0);
    int energy = m_colony->energy() - settings->read<int>(id, "energy", 0);

    if (matter >= 0 and energy >= 0 and qnt_earned < qnt_max)
    {
        m_colony->set_matter(matter);
        m_colony->set_energy(energy);

        settings->write<int>(id, "qnt_earned", ++qnt_earned);
        settings->save("res/datas/slot" + to_string(m_slot) + "/items.sav");
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
Hospital::items_screen()
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
    env->canvas->draw("Qnt.", 855 * scale_w, 186 * scale_h, color);

    shared_ptr<Texture> texture = env->resources_manager->get_texture(
        path + "icons/matter_energy.png");
    env->canvas->draw(texture.get(), 690 * scale_w, 188 * scale_h);

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/items.sav");
    auto sections = settings->sections();

    int y = 236;
    for (auto section : sections)
    {
        string name = section.first;
        string matter = section.second["matter"];
        string energy = section.second["energy"];
        string qnt_earned = section.second["qnt_earned"];
        string qnt_max = section.second["qnt_max"];

        env->canvas->draw(name, 360 * scale_w, y * scale_h, color);
        if (not matter.empty())
        {
            env->canvas->draw(matter + "/" + energy, 690 * scale_w,
                y * scale_h, color);
        }
        if (not qnt_earned.empty())
        {
            env->canvas->draw(qnt_earned + "/" + qnt_max, 855 * scale_w,
                y * scale_h, color);
        }

        texture = env->resources_manager->get_texture(path + "icons/health.png");
        Rect clip = Rect(0, 25, 50, 50/2);
        env->canvas->draw(texture.get(), clip, 310 * scale_w,
            y * scale_h,
            50 * scale_w, 25 * scale_h);

        y += 64;
    }

    env->canvas->draw("TOTAL", 607 * scale_w, 633 * scale_h, color);
    font->set_size(16);
    env->canvas->draw("800", 800 * scale_w, 633 * scale_h, Color::RED);
    env->canvas->draw("/", 837 * scale_w, 633 * scale_h, color);
    env->canvas->draw("176", 855 * scale_w, 633 * scale_h, color);
    env->canvas->draw("176", 855 * scale_w, 660 * scale_h, color);
    env->canvas->draw("176", 800 * scale_w, 660 * scale_h, color);
    env->canvas->draw("/", 837 * scale_w, 660 * scale_h, color);
    env->canvas->draw("176", 855 * scale_w, 660 * scale_h, color);
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

    int y = 236;
    for (auto section : sections)
    {
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

    int y = 236;
    for (auto section : sections)
    {
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
