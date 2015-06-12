#include "central.h"
#include "colony.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>

Central::Central(const string& next)
    : Level("central", next), m_scenario(nullptr), m_screen(CHAT)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_scenario = env->resources_manager->get_texture(path + "central/central_chat_scenario.png");

    Colony *colony = new Colony(this, "central");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Central::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    double scale = 1;
    env->canvas->draw(m_scenario.get(), 275 * scale, 173 * scale);

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
    double scale = 1;
    string path = "res/images/colony/";

    Button *button =  new Button(this, "central", path + "central_button.png",
        28 * scale, 218 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "chat", path + "colony_small_button.png",
        28 * scale, 322 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Chat");
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "quests", path + "colony_small_button.png",
        28 * scale, 427 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Quests");

    m_buttons[button->id()] = button;

    button = new Button(this, "bestiary", path + "colony_small_button.png",
        28 * scale, 531 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Bestiary");

    m_buttons[button->id()] = button;

    button = new Button(this, "timers", path + "colony_small_button.png",
        28 * scale, 635 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Timers");

    m_buttons[button->id()] = button;

    for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
    {
        auto id = it->first;
        m_buttons[id]->add_observer(this);
        add_child(m_buttons[id]);
    }
}

void
Central::change_buttons()
{
    for (auto it : m_buttons)
    {
        if (it.first != "central")
        {
            it.second->change_state(Button::IDLE);
        }
    }
}

void
Central::change_to_chat()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    double scale = 1;
    font->set_size(18 * scale);
    Color color(170, 215, 190);
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/chat.sav");
    map< string, map<string, string> > sections = settings->sections();
    string text = sections["Central"]["welcome"];
    env->canvas->draw(text, (305 + 5) * scale, 605 * scale, color);
    env->canvas->draw(Rect(305 * scale, 605 * scale, 670 * scale, 116 * scale), color);
}

void
Central::change_to_quests()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    double scale = 1;
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18 * scale);

    env->canvas->draw("Name", 360 * scale, 188 * scale, color);

    shared_ptr<Texture> texture;
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/quests.sav");
    map< string, map<string, string> > sections = settings->sections();

    int y = 236;
    for (auto section : sections)
    {
        string name = section.first;
        string new_ = section.second["new"];

        env->canvas->draw(name, 360 * scale, y * scale, color);
        if (not new_.empty())
        {
            env->canvas->draw("NEW", 690 * scale, y * scale, color);
        }

        texture = env->resources_manager->get_texture(path + "big_list.png");
        Rect clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale, (y + 5) * scale);

        y += 64;
    }
}

void
Central::change_to_bestiary()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    double scale = 1;
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18 * scale);

    env->canvas->draw("Name", 360 * scale, 188 * scale, color);
    
    shared_ptr<Texture> texture;
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/bestiary.sav");
    map< string, map<string, string> > sections = settings->sections();

    int y = 236;
    for (auto section : sections)
    {
        string name = section.first;
        string icon = section.second["icon"];

        env->canvas->draw(name, 360 * scale, y * scale, color);
        if (not icon.empty())
        {
            texture = env->resources_manager->get_texture(path + "icons/" + icon + ".png");
            env->canvas->draw(texture.get(), 310 * scale, y * scale);
        }

        texture = env->resources_manager->get_texture(path + "big_list.png");
        Rect clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale, (y + 5) * scale);

        y += 64;
    }
}

void
Central::change_to_timers()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    double scale = 1;
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18 * scale);

    env->canvas->draw("Name", 360 * scale, 188 * scale, color);
    env->canvas->draw("Time", 855 * scale, 186 * scale, color);

    shared_ptr<Texture> texture;
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/timers.sav");
    map< string, map<string, string> > sections = settings->sections();

    int y = 236;
    for (auto section : sections)
    {
        string name = section.first;
        string time = section.second["time"];
        string icon = section.second["icon"];

        env->canvas->draw(name, 360 * scale, y * scale, color);
        if (not time.empty())
        {
            env->canvas->draw(time, 855 * scale, y * scale, color);
        }
        if (not icon.empty())
        {
            texture = env->resources_manager->get_texture(path + "icons/" + icon + ".png");
            env->canvas->draw(texture.get(), 310 * scale, y * scale);
        }

        texture = env->resources_manager->get_texture(path + "big_list.png");
        Rect clip = Rect(0, 0, 602, 75/3);
        env->canvas->draw(texture.get(), clip, 310 * scale, (y + 5) * scale);

        y += 64;
    }
}
