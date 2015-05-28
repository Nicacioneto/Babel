#include "menu.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/texture.h>

Menu::Menu(const string& next, const string& texture)
    : Level("menu", next), m_texture(nullptr), m_logo(nullptr), m_play(nullptr),
        m_options(nullptr), m_credits(nullptr), m_exit(nullptr)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(texture);
    m_logo = env->resources_manager->get_texture("res/images/menu/babel-logo.png");
    
    double scale = env->canvas->scale();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22 * scale);

    const int x_button = 188 * scale;
    const int y_button = env->canvas->h() - 149 * scale;
    const int w_button = 140 * scale;
    const int h_button = 60 * scale;
    const int spacing = 170 * scale;
    
    m_play = new Button(this, "start", "res/images/menu/button.png", x_button,
        y_button, w_button, h_button);
    m_play->set_text("Play Game");

    m_options = new Button(this, "options", "res/images/menu/button.png",
        x_button + spacing, y_button, w_button, h_button);
    m_options->set_text("Options");

    m_credits = new Button(this, "credits", "res/images/menu/button.png", x_button + 2 * spacing,
        y_button, w_button, h_button);
    m_credits->set_text("Credits");

    m_exit = new Button(this, "exit", "res/images/menu/button.png", x_button + 3 * spacing,
        y_button, w_button, h_button);
    m_exit->set_text("Exit");

    m_play->add_observer(this);
    m_options->add_observer(this);
    m_credits->add_observer(this);
    m_exit->add_observer(this);

    add_child(m_play);
    add_child(m_options);
    add_child(m_credits);
    add_child(m_exit);
}

void
Menu::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_texture.get());

    double scale = env->canvas->scale();
    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2, 25 * scale);
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
        set_next("play");
    }
    else if (button->id() == "options")
    {
        set_next("options");
    }
    else if (button->id() == "credits")
    {
        set_next("credits");
    }

    finish();

    return true;
}
