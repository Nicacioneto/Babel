#include "central.h"
#include "colony.h"
#include <core/font.h>
#include <core/rect.h>

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

    double scale = env->canvas->scale();
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
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
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
    double scale = env->canvas->scale();
    font->set_size(18 * scale);
    Color color(170, 215, 190);
    
    env->canvas->draw(Rect(305 * scale, 605 * scale, 670 * scale, 116 * scale), color);
    env->canvas->draw("Chat Text", 305 * scale, 605 * scale, color);
}

void
Central::change_to_quests()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    double scale = env->canvas->scale();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18 * scale);

    env->canvas->set_blend_mode(Canvas::BLEND);
    Color color(170, 215, 190);

    env->canvas->draw("Name", 360 * scale, 188 * scale, color);

    shared_ptr<Texture> texture = env->resources_manager->get_texture(path + "big_list.png");
    Rect clip = Rect(0, 0, 602, 75/3);
    env->canvas->draw(texture.get(), clip, 310 * scale, (236 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (300 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (364 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (428 + 5) * scale);

    // Name
    color.set_a(100);
    env->canvas->draw("Get that", 360 * scale, 236 * scale, color);
    env->canvas->draw("Get Something", 360 * scale, 300 * scale, color);
    env->canvas->draw("Kill the enemy", 360 * scale, 364 * scale, color);
    env->canvas->draw("Explore the tower", 360 * scale, 428 * scale, color);

    env->canvas->draw("NEW", 855 * scale, 364 * scale, color);
    env->canvas->draw("NEW", 855 * scale, 428 * scale, color);

    env->canvas->set_blend_mode(Canvas::NONE);
}

void
Central::change_to_bestiary()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    double scale = env->canvas->scale();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18 * scale);

    Color color(170, 215, 190);
    env->canvas->draw("Name", 360 * scale, 188 * scale, color);
    
    shared_ptr<Texture> texture = env->resources_manager->get_texture(
        path + "central/research_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 236 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 300 * scale);

    // Name
    env->canvas->draw("Zoombie", 360 * scale, 236 * scale, color);
    env->canvas->draw("Monster Boss", 360 * scale, 300 * scale, color);

    texture = env->resources_manager->get_texture(path + "big_list.png");
    Rect clip = Rect(0, 0, 602, 75/3);
    env->canvas->draw(texture.get(), clip, 310 * scale, (236 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (300 + 5) * scale);
}

void
Central::change_to_timers()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";
    double scale = env->canvas->scale();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18 * scale);

    env->canvas->set_blend_mode(Canvas::BLEND);
    Color color(170, 215, 190);

    env->canvas->draw("Name", 360 * scale, 188 * scale, color);
    env->canvas->draw("Time", 855 * scale, 186 * scale, color);

    shared_ptr<Texture> texture = env->resources_manager->get_texture(
        path + "central/workshop_icon.png");

    env->canvas->draw(texture.get(), 310 * scale, 236 * scale);
    texture = env->resources_manager->get_texture(path + "central/research_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 300 * scale);
    texture = env->resources_manager->get_texture(path + "central/hospital_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 364 * scale);
    texture = env->resources_manager->get_texture(path + "central/workshop_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 428 * scale);
    texture = env->resources_manager->get_texture(path + "central/hospital_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 492 * scale);
    texture = env->resources_manager->get_texture(path + "central/research_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 556 * scale);

    texture = env->resources_manager->get_texture(path + "big_list.png");
    Rect clip = Rect(0, 0, 602, 75/3);
    env->canvas->draw(texture.get(), clip, 310 * scale, (236 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (300 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (364 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (428 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (492 + 5) * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, (556 + 5) * scale);

    // Name
    color.set_a(100);
    env->canvas->draw("Printing: 3D Gun", 360 * scale, 236 * scale, color);
    env->canvas->draw("Reviving: Isaac", 360 * scale, 300 * scale, color);
    env->canvas->draw("Researching: Laser Guns", 360 * scale, 364 * scale, color);
    env->canvas->draw("Printing: 3D Gun", 360 * scale, 428 * scale, color);
    env->canvas->draw("Reviving: Galileu", 360 * scale, 492 * scale, color);
    env->canvas->draw("Researching: Laser Guns", 360 * scale, 556 * scale, color);

    // Time
    env->canvas->draw("15:00", 855 * scale, 236 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 300 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 364 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 428 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 492 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 556 * scale, color);

    env->canvas->set_blend_mode(Canvas::NONE);
}
