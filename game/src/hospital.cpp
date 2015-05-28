#include "colony.h"
#include "hospital.h"
#include <core/font.h>
#include <core/rect.h>

Hospital::Hospital(const string& next)
    : Level("hospital", next), m_scenario(nullptr), m_reset(nullptr),
        m_buy(nullptr), m_screen(CHAT)
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();

    string path = "res/images/colony/";
    m_scenario = env->resources_manager->get_texture(path + "hospital/hospital_chat_scenario.png");

    m_reset = new Button(this, "reset", path + "hospital/reset.png",
        855 * scale, 693 * scale, 57 * scale, 52/2 * scale);
    m_reset->change_state(Button::HIDE);

    m_buy = new Button(this, "buy", path + "hospital/buy.png",
        772 * scale, 693 * scale, 58 * scale, 78/3 * scale);
    m_buy->set_sprites(3);
    m_buy->change_state(Button::HIDE);

    m_reset->add_observer(this);
    m_buy->add_observer(this);
    add_child(m_reset);
    add_child(m_buy);
    
    Colony *colony = new Colony(this, "hospital");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Hospital::draw_self(double, double)
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

        case ITEMS:
            change_to_items();
            break;

        case RESEARCH:
            change_to_research();
            break;

        case REVIVE:
            change_to_revive();
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
        string path = "res/images/colony/";
        change_buttons();

        if (button->id() == "chat")
        {
            m_screen = CHAT;
            m_scenario = env->resources_manager->get_texture(path + "hospital/hospital_chat_scenario.png");
            m_reset->change_state(Button::HIDE);
            m_buy->change_state(Button::HIDE);
        }
        else if (button->id() == "items")
        {
            m_screen = ITEMS;
            m_scenario = env->resources_manager->get_texture(path + "hospital/hospital_scenario.png");
            m_reset->change_state(Button::IDLE);
            m_buy->change_state(Button::IDLE);
        }
        else if (button->id() == "research")
        {
            m_screen = RESEARCH;
            m_scenario = env->resources_manager->get_texture(path + "hospital/hospital_scenario.png");
            m_reset->change_state(Button::HIDE);
            m_buy->change_state(Button::HIDE);
        }
        else if (button->id() == "revive")
        {
            m_screen = REVIVE;
            m_scenario = env->resources_manager->get_texture(path + "hospital/hospital_scenario.png");
            m_reset->change_state(Button::HIDE);
            m_buy->change_state(Button::HIDE);
        }

        button->change_state(Button::ACTIVE);

        if (button->id() == "reset")
        {
            button->change_state(Button::ON_HOVER);
        }
    }

    return true;
}

void
Hospital::create_buttons()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    string path = "res/images/colony/";

    Button *button =  new Button(this, "hospital", path + "hospital_button.png",
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

    button = new Button(this, "items", path + "colony_small_button.png",
        28 * scale, 427 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Items");

    m_buttons[button->id()] = button;

    button = new Button(this, "research", path + "colony_small_button.png",
        28 * scale, 531 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Research");

    m_buttons[button->id()] = button;

    button = new Button(this, "revive", path + "colony_small_button.png",
        28 * scale, 635 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Revive");

    m_buttons[button->id()] = button;

    for (auto it : m_buttons)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}

void
Hospital::change_buttons()
{
    for (auto it : m_buttons)
    {
        if (it.first != "hospital")
        {
            it.second->change_state(Button::IDLE);
        }
    }
}

void
Hospital::change_to_chat()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    double scale = env->canvas->scale();
    font->set_size(18 * scale);
    Color color(170, 215, 190);
    
    env->canvas->draw(Rect(305 * scale, 605 * scale, 670 * scale, 116 *scale), color);
    env->canvas->draw("Chat Text", 305 * scale, 605 * scale, color);
}

void
Hospital::change_to_items()
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
    env->canvas->draw("Qnt.", 855 * scale, 186 * scale, color);

    // temp texture
    shared_ptr<Texture> texture = env->resources_manager->get_texture(path + "hospital/material_power_icon.png");
    env->canvas->draw(texture.get(), 690 * scale, 188 * scale);

    // temp texture
    texture = env->resources_manager->get_texture(path + "central/workshop_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 236 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 300 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 364 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 428 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 492 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 556 * scale);

    // temp texture
    texture = env->resources_manager->get_texture(path + "colony_small_button.png");
    Rect clip(0, 25, 190, 60);
    env->canvas->draw(texture.get(), clip, 310 * scale, 236 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 300 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 364 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 428 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 492 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 556 * scale);

    color.set_a(100);
    env->canvas->draw("Health Potion I", 360 * scale, 236 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 300 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 364 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 428 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 492 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 556 * scale, color);

    env->canvas->draw("70/100", 690 * scale, 236 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 300 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 364 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 428 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 492 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 556 * scale, color);

    env->canvas->draw("10/20", 855 * scale, 236 * scale, color);
    env->canvas->draw("13/20", 855 * scale, 300 * scale, color);
    env->canvas->draw("14/15", 855 * scale, 364 * scale, color);
    env->canvas->draw("10/10", 855 * scale, 428 * scale, color);
    env->canvas->draw("13/20", 855 * scale, 492 * scale, color);
    env->canvas->draw("13/20", 855 * scale, 556 * scale, color);

    env->canvas->draw("TOTAL", 607 * scale, 633 * scale, color);
    font->set_size(16 * scale);
    env->canvas->draw("800", 800 * scale, 633 * scale, Color::RED);
    env->canvas->draw("/", 837 * scale, 633 * scale, color);
    env->canvas->draw("176", 855 * scale, 633 * scale, color);
    env->canvas->draw("176", 855 * scale, 660 * scale, color);
    env->canvas->draw("176", 800 * scale, 660 * scale, color);
    env->canvas->draw("/", 837 * scale, 660 * scale, color);
    env->canvas->draw("176", 855 * scale, 660 * scale, color);

    env->canvas->set_blend_mode(Canvas::NONE);
}

void
Hospital::change_to_research()
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

    // temp texture
    shared_ptr<Texture> texture = env->resources_manager->get_texture(path + "hospital/material_power_icon.png");
    env->canvas->draw(texture.get(), 690 * scale, 188 * scale);

    // temp texture
    texture = env->resources_manager->get_texture(path + "central/workshop_icon.png");
    env->canvas->draw(texture.get(), 310 * scale, 236 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 300 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 364 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 428 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 492 * scale);
    env->canvas->draw(texture.get(), 310 * scale, 556 * scale);

    // temp texture
    texture = env->resources_manager->get_texture(path + "colony_small_button.png");
    Rect clip(0, 25, 190, 60);
    env->canvas->draw(texture.get(), clip, 310 * scale, 236 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 300 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 364 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 428 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 492 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 556 * scale);

    env->canvas->draw("Health Potion I", 360 * scale, 236 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 300 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 364 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 428 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 492 * scale, color);
    env->canvas->draw("Hyper-Metabolism", 360 * scale, 556 * scale, color);

    env->canvas->draw("70/100", 690 * scale, 236 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 300 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 364 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 428 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 492 * scale, color);
    env->canvas->draw("70/100", 690 * scale, 556 * scale, color);

    env->canvas->draw("15:00", 855 * scale, 236 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 300 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 364 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 428 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 492 * scale, color);
    env->canvas->draw("15:00", 855 * scale, 556 * scale, color);

    env->canvas->set_blend_mode(Canvas::NONE);
}

void
Hospital::change_to_revive()
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
    env->canvas->draw("Class", 524 * scale, 186 * scale, color);
    env->canvas->draw("Time", 855 * scale, 186 * scale, color);

    shared_ptr<Texture> texture = env->resources_manager->get_texture(path + "hospital/material_power_icon.png");
    env->canvas->draw(texture.get(), 690 * scale, 188 * scale);

    texture = env->resources_manager->get_texture(path + "hospital/health_icon.png");
    Rect clip = Rect(0, 25, 50, 50/2);
    env->canvas->draw(texture.get(), clip, 310 * scale, 236 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 300 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 364 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 428 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 492 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 556 * scale);

    // Name
    env->canvas->draw("Isaac", 360 * scale, 236 * scale, color);
    env->canvas->draw("Albert", 360 * scale, 300 * scale, color);
    env->canvas->draw("Newton", 360 * scale, 364 * scale, color);
    env->canvas->draw("Clarke", 360 * scale, 428 * scale, color);
    env->canvas->draw("Brooker", 360 * scale, 492 * scale, color);
    env->canvas->draw("Michael", 360 * scale, 556 * scale, color);

    // Class
    env->canvas->draw("Soldier", 524 * scale, 236 * scale, color);
    env->canvas->draw("Enforcer", 524 * scale, 300 * scale, color);
    env->canvas->draw("Ghost", 524 * scale, 364 * scale, color);
    env->canvas->draw("Medic", 524 * scale, 428 * scale, color);
    env->canvas->draw("Sabouteur", 524 * scale, 492 * scale, color);
    env->canvas->draw("Soldier", 524 * scale, 556 * scale, color);

    // Power
    env->canvas->draw("70/120", 690 * scale, 428 * scale, color);
    
    // Time
    env->canvas->draw("15:00", 855 * scale, 428 * scale, color);

    texture = env->resources_manager->get_texture(path + "big_list.png");
    clip = Rect(0, 0, 602, 75/3);
    env->canvas->draw(texture.get(), clip, 310 * scale, 236 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 300 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 364 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 428 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 492 * scale);
    env->canvas->draw(texture.get(), clip, 310 * scale, 556 * scale);

    env->canvas->set_blend_mode(Canvas::NONE);
}
