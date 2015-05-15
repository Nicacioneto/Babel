#include "menu.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/texture.h>

#define X_BUTTON 188
#define W_BUTTON 140
#define H_BUTTON 60
#define SPACING 170

Menu::Menu(const string& next, const string& texture)
    : Level("", next), m_texture(nullptr), m_logo(nullptr), m_play(nullptr),
        m_settings(nullptr), m_credits(nullptr), m_exit(nullptr)
{
    env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(texture);
    m_logo = env->resources_manager->get_texture("res/images/menu/babel-logo.png");
    
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    
    const int y_button = env->canvas->h() - 149;
    
    m_play = new Button(this, "start", "res/images/menu/button.png", X_BUTTON,
        y_button, W_BUTTON, H_BUTTON);
    m_play->set_text("Play Game", Color(170, 215, 190));

    m_settings = new Button(this, "settings", "res/images/menu/button.png", X_BUTTON + SPACING * 2,
        y_button, W_BUTTON, H_BUTTON);
    m_settings->set_text("Options", Color(170, 215, 190));

    m_credits = new Button(this, "credits", "res/images/menu/button.png", X_BUTTON + SPACING * 3,
        y_button, W_BUTTON, H_BUTTON);
    m_credits->set_text("Credits", Color(170, 215, 190));

    m_exit = new Button(this, "exit", "res/images/menu/button.png", X_BUTTON + SPACING * 4,
        y_button, W_BUTTON, H_BUTTON);
    m_exit->set_text("Exit", Color(170, 215, 190));

    m_play->add_observer(this);
    m_settings->add_observer(this);
    m_credits->add_observer(this);
    m_exit->add_observer(this);

    add_child(m_play);
    add_child(m_settings);
    add_child(m_credits);
    add_child(m_exit);

}

void
Menu::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    const int y_button = env->canvas->h() - 149.0 * scale;

    m_play->set_position(scale * X_BUTTON, y_button);
    m_play->set_dimensions(W_BUTTON * scale, H_BUTTON * scale);
    
    m_settings->set_position(scale * (X_BUTTON + SPACING), y_button);
    m_settings->set_dimensions(W_BUTTON * scale, H_BUTTON * scale);

    m_credits->set_position(scale * (X_BUTTON + SPACING * 2), y_button);
    m_credits->set_dimensions(W_BUTTON * scale, H_BUTTON * scale);
    
    m_exit->set_position(scale * (X_BUTTON + SPACING * 3), y_button);
    m_exit->set_dimensions(W_BUTTON * scale, H_BUTTON * scale);

    shared_ptr<Font> font = env->canvas->font();
    font->set_size(22 * scale);
}

void
Menu::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_texture.get());

    double scale = env->canvas->scale();

    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2,
        25 * scale);
}

bool
Menu::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "start")
    {
        m_next = "play";
    }
    else if (button->id() == "settings")
    {
        m_next = "settings";
    }
    else if (button->id() == "credits")
    {
        m_next = "credits";
    }

    m_done = true;
    return true;
}
