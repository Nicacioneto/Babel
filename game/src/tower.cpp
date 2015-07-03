/*
 * Tower class implementation
 *
 * Author: Tiamat
 * Date: 23/06/2015
 * License: LGPL. No copyright.
 */
#include "colony.h"
#include "tower.h"
#include <core/font.h>
#include <core/line.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Tower::Tower(int slot, const string& next)
    : Level("tower", next), m_slot(slot), m_actual_floor(1), m_unlocked_floors(1)
{
    Colony *colony = new Colony(slot, this, "tower");
    colony->add_observer(this);
    add_child(colony);

    Environment *env = Environment::get_instance();
    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/tower.sav");

    m_actual_floor = m_settings->read<int>("Tower", "actual_floor", 1);
    m_unlocked_floors = m_settings->read<int>("Tower", "unlocked_floors", 1);

    load_texture();
    create_buttons();
}

void
Tower::load_texture()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/tower/";

    m_textures["babel"] = env->resources_manager->get_texture(path + "babel_headline.png");
    m_textures["char1"] = env->resources_manager->get_texture(path + "char1.png");
    m_textures["char2"] = env->resources_manager->get_texture(path + "char2.png");
    m_textures["char3"] = env->resources_manager->get_texture(path + "char3.png");
    m_textures["char4"] = env->resources_manager->get_texture(path + "char4.png");
    m_textures["colony"] = env->resources_manager->get_texture(path + "colony_headline.png");
    m_textures["drone1"] = env->resources_manager->get_texture(path + "drone1.png");
    m_textures["drone2"] = env->resources_manager->get_texture(path + "drone2.png");
    m_textures["hostile"] = env->resources_manager->get_texture(path + "hostile.png");
    m_textures["quest"] = env->resources_manager->get_texture(path + "quest.png");
    m_textures["squad"] = env->resources_manager->get_texture(path + "squad.png");
}

void
Tower::create_buttons()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/tower/";
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button =  new Button(this, "inspect_drone", path + "inspect.png",
        512 * scale_w, 626 * scale_h, 25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "change_drone", path + "change.png",
        545 * scale_w, 626 * scale_h, 25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "send_drone", path + "send_button.png",
        300 * scale_w, 618 * scale_h, 300 * scale_w, 105 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "inspect_team", path + "inspect.png",
        850 * scale_w, 626 * scale_h, 25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "change_team", path + "change.png",
        885 * scale_w, 626 * scale_h, 25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "send_team", path + "send_button.png",
        644 * scale_w, 618 * scale_h, 300 * scale_w, 105 * scale_h);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    // Select floor
    int x, y = 638 + 34;
    for (int i = 1; i <= 15; ++i)
    {
        if (i % 2)
        {
            x = 134;
            y -= 34;
        }
        else
        {
            x = 90;
            y -= 20;
        }

        string f = i > m_unlocked_floors ? "L" : to_string(i);
        button = new Button(this, "floor" + to_string(i), path + "floor" + f + ".png",
            x * scale_w, y * scale_h, 35 * scale_w, 35 * scale_h);
        button->set_sprites(1);
        button->set_active(f != "L");
        m_floors[button->id()] = button;
        button->add_observer(this);
        add_child(button);
    }
}

void
Tower::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    Color color(170, 215, 190);
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(24);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    // env->canvas->draw(m_textures["babel"].get(), 193 * scale_w, 25 * scale_h);

    env->canvas->draw(m_textures["quest"].get(), 308 * scale_w, 317 * scale_h);
    env->canvas->draw(m_textures["hostile"].get(), 308 * scale_w, 478 * scale_h);
    env->canvas->draw(m_textures["drone2"].get(), 312 * scale_w, 626 * scale_h);
    env->canvas->draw(m_textures["squad"].get(), 655 * scale_w, 626 * scale_h);
    env->canvas->draw(m_textures["quest"].get(), 308 * scale_w, 317 * scale_h);

    // Send team
    env->canvas->draw(m_textures["drone2"].get(), 680 * scale_w, 680 * scale_h);
    env->canvas->draw(Line(Point(724 * scale_w, 683 * scale_h),
        Point(724 * scale_w, 712 * scale_h)), color);
    env->canvas->draw(m_textures["char1"].get(), 734 * scale_w, 680 * scale_h);
    env->canvas->draw(m_textures["char2"].get(), 780 * scale_w, 680 * scale_h);
    env->canvas->draw(m_textures["char3"].get(), 826 * scale_w, 680 * scale_h);
    env->canvas->draw(m_textures["char4"].get(), 872 * scale_w, 680 * scale_h);

    Point a(358 * scale_w, 308 * scale_h);
    Point b(910 * scale_w, 308 * scale_h);
    env->canvas->draw(Line(a, b), Color(89, 112, 100));

    a = Point(358 * scale_w, 470 * scale_h);
    b = Point(910 * scale_w, 470 * scale_h);
    env->canvas->draw(Line(a, b), Color(89, 112, 100));

    env->canvas->draw("Select Floor", 60 * scale_w, 210 * scale_h, color);
    env->canvas->draw("Floor " + to_string(m_actual_floor), 360 * scale_w, 204 * scale_h, color);
    env->canvas->draw("Quests", 360 * scale_w, 324 * scale_h, color);
    env->canvas->draw("Hostile Activity", 360 * scale_w, 480 * scale_h, color);
    env->canvas->draw("Send Drone", 360 * scale_w, 626 * scale_h, color);
    env->canvas->draw("Send Team", 704 * scale_w, 626 * scale_h, color);

    font->set_size(16);
    env->canvas->draw("One does not go into Floor " + to_string(m_actual_floor),
        360 * scale_w, 250 * scale_h, color);
    env->canvas->draw("Find the missing artifact located at Floor " + to_string(m_actual_floor),
        360 * scale_w, 370 * scale_h, color);
    env->canvas->draw("Kill 10 Nano Ghouls", 360 * scale_w, 390 * scale_h, color);
    env->canvas->draw("Nano Ghouls", 360 * scale_w, 512 * scale_h, color);
    env->canvas->draw("Tecnhoviking", 360 * scale_w, 532 * scale_h, color);
    env->canvas->draw("Hornet Drone", 360 * scale_w, 666 * scale_h, color);
    env->canvas->draw("+10 Barrier Strenght", 360 * scale_w, 686 * scale_h, color);
    env->canvas->draw("15:00", 535 * scale_w, 695 * scale_h, color);
}

bool
Tower::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);
    Environment *env = Environment::get_instance();
    
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
    else if (button->id() == "inspect_drone")
    {
        // TODO
    }
    else if (button->id() == "inspect_team")
    {
        shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
            to_string(m_slot) + "/colony.sav");
        settings->write<string>("Barracks", "prev", "tower");
        settings->save("res/datas/slot" + to_string(m_slot) + "/colony.sav");
        set_next("barracks");
        finish();
    }
    else if (button->id() == "change_drone")
    {
        // TODO
    }
    else if (button->id() == "change_team")
    {
        set_next("squad");
        finish();
    }
    else if (button->id() == "send_drone")
    {
        // TODO
    }
    else if (button->id() == "send_team")
    {
        string path = "res/datas/slot" + to_string(m_slot) + "/squad.sav";
        shared_ptr<Settings> settings = env->resources_manager->get_settings(path);

        string hero1 = settings->read<string>("Squad", "hero1", "");

        hero1.empty() ? set_next("squad") : set_next("dungeon");
        finish();
    }
    else if (button->id().find("floor") != string::npos)
    {
        m_actual_floor = atoi(button->id().substr(5).c_str());
    }

    return true;
}
