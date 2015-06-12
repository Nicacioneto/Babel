#include "colony.h"
#include "facilities.h"
#include <core/font.h>
#include <core/line.h>
#include <core/point.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

Facilities::Facilities(const string& next)
    : Level("facilities", next), m_screen(CHAT)
{
    string path = "res/images/colony/facilities/";
    
    Colony *colony = new Colony(this, "facilities");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Facilities::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    switch (m_screen)
    {
        case CHAT:
            change_to_chat();
            break;

        case MILITARY:
            change_to_military();
            break;

        case PSIONIC:
            change_to_psionic();
            break;

        case TECH:
            change_to_tech();
            break;
    }
}

bool
Facilities::on_message(Object *sender, MessageID id, Parameters)
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
    else if (button->id() != "facilities")
    {
        change_buttons();

        if (button->id() == "chat")
        {
            m_screen = CHAT;
        }
        else if (button->id() == "military")
        {
            m_screen = MILITARY;
        }
        else if (button->id() == "psionic")
        {
            m_screen = PSIONIC;
        }
        else if (button->id() == "tech")
        {
            m_screen = TECH;
        }

        button->change_state(Button::ACTIVE);
    }

    return true;
}

void
Facilities::create_buttons()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(24);

    const int x = (28 / W) * env->canvas->w();
    const int w = (190 / W) * env->canvas->w();
    const int h = (180/3 / H) * env->canvas->h();

    Button *button =  new Button(this, "facilities", path + "facilities_button.png",
        x, (218 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "chat", path + "colony_small_button.png",
        x, (322 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Chat");
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "military", path + "colony_small_button.png",
        x, (427 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Military");

    m_buttons[button->id()] = button;

    button = new Button(this, "psionic", path + "colony_small_button.png",
        x, (531 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Psionic");

    m_buttons[button->id()] = button;

    button = new Button(this, "tech", path + "colony_small_button.png",
        x, (635 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Technologic");

    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}

void
Facilities::change_buttons()
{
    for (auto b : m_buttons)
    {
        if (b.first != "facilities")
        {
            b.second->change_state(Button::IDLE);
        }
    }
}

void
Facilities::change_to_chat()
{
    Environment *env = Environment::get_instance();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);
    Color color(170, 215, 190);
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/chat.sav");
    map< string, map<string, string> > sections = settings->sections();
    string text = sections["Facilities"]["welcome"];
    env->canvas->draw(text, ((305+5) / W) * env->canvas->w(), (605 / H) * env->canvas->h(), color);
    env->canvas->draw(Rect((305 / W) * env->canvas->w(), (605 / H) * env->canvas->h(),
        (670 / W) * env->canvas->w(), (116 / H) * env->canvas->h()), color);
}

void
Facilities::change_to_military()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/facilities/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    shared_ptr<Texture> texture;

    for (int i = 0, y = 235; i < 5; ++i, y += 77)
    {
        texture = env->resources_manager->get_texture(path + "rifle.png");
        env->canvas->draw(texture.get(), (384 / W) * env->canvas->w(), (y / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "shotgun.png");
        env->canvas->draw(texture.get(), (462 / W) * env->canvas->w(), (y / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "pistol_red.png");
        env->canvas->draw(texture.get(), (540 / W) * env->canvas->w(), (y / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "sniper_red.png");
        env->canvas->draw(texture.get(), (618 / W) * env->canvas->w(), (y / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "rifle_red.png");
        env->canvas->draw(texture.get(), (696 / W) * env->canvas->w(), (y / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "shotgun_red.png");
        env->canvas->draw(texture.get(), (774 / W) * env->canvas->w(), (y / H) * env->canvas->h());

        Point a((360 / W) * env->canvas->w(), ((y-14) / H) * env->canvas->h());
        Point b((851 / W) * env->canvas->w(), ((y-14) / H) * env->canvas->h());
        env->canvas->draw(Line(a, b), color);
    }

    texture = env->resources_manager->get_texture(path + "wake.png");
    env->canvas->draw(texture.get(), (308 / W) * env->canvas->w(), (628 / H) * env->canvas->h());

    Rect rect((333 / W) * env->canvas->w(), (196 / H) * env->canvas->h(),
        (25 / W) * env->canvas->w(), (442 / H) * env->canvas->h());
    env->canvas->draw(rect, color);

    rect = Rect((340 / W) * env->canvas->w(), (535 / H) * env->canvas->h(),
        (10 / W) * env->canvas->w(), (102 / H) * env->canvas->h());
    env->canvas->fill(rect, Color(206, 178, 46));

    texture = env->resources_manager->get_texture(path + "meter.png");
    env->canvas->draw(texture.get(), (290 / W) * env->canvas->w(),
        ((535-13) / H) * env->canvas->h());

    env->canvas->draw("999", ((290+4) / W) * env->canvas->w(),
        ((535-13+1) / H) * env->canvas->h(), color);
}

void
Facilities::change_to_psionic()
{
    // Environment *env = Environment::get_instance();
    // string path = "res/images/colony/";
    // double scale = 1;
    // Color color(170, 215, 190);

    // shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    // env->canvas->set_font(font);
    // font->set_size(18 * scale);

    // env->canvas->draw("Name", 360 * scale, 188 * scale, color);
    // env->canvas->draw("Time", 855 * scale, 186 * scale, color);

    // shared_ptr<Texture> texture = env->resources_manager->get_texture(
    //     path + "icons/matter_power.png");
    // env->canvas->draw(texture.get(), 690 * scale, 188 * scale);

    // shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/items.sav");
    // map< string, map<string, string> > sections = settings->sections();

    // int y = 236;
    // for (auto section : sections)
    // {
    //     string name = section.first;
    //     string matter = section.second["matter"];
    //     string power = section.second["power"];
    //     string time = section.second["time"];

    //     env->canvas->draw(name, 360 * scale, y * scale, color);
    //     if (not matter.empty())
    //         env->canvas->draw(matter + "/" + power, 690 * scale, y * scale, color);
    //     if (not time.empty())
    //         env->canvas->draw(time, 855 * scale, y * scale, color);

    //     texture = env->resources_manager->get_texture(path + "icons/health.png");
    //     Rect clip = Rect(0, 25, 50, 50/2);
    //     env->canvas->draw(texture.get(), clip, 310 * scale, y * scale);

    //     texture = env->resources_manager->get_texture(path + "big_list.png");
    //     clip = Rect(0, 0, 602, 75/3);
    //     env->canvas->draw(texture.get(), clip, 310 * scale, (y + 5) * scale);

    //     y += 64;
    // }
}

void
Facilities::change_to_tech()
{
    // Environment *env = Environment::get_instance();
    // string path = "res/images/colony/";
    // double scale = 1;
    // Color color(170, 215, 190);

    // shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    // env->canvas->set_font(font);
    // font->set_size(18 * scale);

    // env->canvas->draw("Name", 360 * scale, 188 * scale, color);
    // env->canvas->draw("Class", 524 * scale, 186 * scale, color);
    // env->canvas->draw("Time", 855 * scale, 186 * scale, color);

    // shared_ptr<Texture> texture = env->resources_manager->get_texture(
    //     path + "icons/matter_power.png");
    // env->canvas->draw(texture.get(), 690 * scale, 188 * scale);

    // shared_ptr<Settings> settings = env->resources_manager->get_settings(
    //     "res/datas/characters.sav");
    // map< string, map<string, string> > sections = settings->sections();

    // int y = 236;
    // for (auto section : sections)
    // {
    //     string name = section.first;
    //     string class_ = section.second["class"];
    //     string matter = section.second["matter"];
    //     string power = section.second["power"];
    //     string time = section.second["time"];

    //     env->canvas->draw(name, 360 * scale, y * scale, color);
    //     env->canvas->draw(class_, 524 * scale, y * scale, color);
    //     if (not matter.empty())
    //         env->canvas->draw(matter + "/" + power, 690 * scale, y * scale, color);
    //     if (not time.empty())
    //         env->canvas->draw(time, 855 * scale, y * scale, color);

    //     texture = env->resources_manager->get_texture(path + "icons/health.png");
    //     Rect clip = Rect(0, 25, 50, 50/2);
    //     env->canvas->draw(texture.get(), clip, 310 * scale, y * scale);

    //     texture = env->resources_manager->get_texture(path + "big_list.png");
    //     clip = Rect(0, 0, 602, 75/3);
    //     env->canvas->draw(texture.get(), clip, 310 * scale, (y + 5) * scale);
    //     y += 64;
    // }
}
