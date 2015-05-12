#include "button.h"
#include "font.h"
#include "menu.h"
#include "rect.h"
#include "resourcesmanager.h"
#include <iostream>

#define X_BUTTON 107
#define W_BUTTON 140
#define H_BUTTON 60
#define SPACING 165

Menu::Menu(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_logo(nullptr), m_start(nullptr), m_loadgame(nullptr),
        m_settings(nullptr), m_credits(nullptr), m_exit(nullptr)
{
    env = Environment::get_instance();

    m_image = env->resources_manager->get_image(image);
    m_logo = env->resources_manager->get_image("res/images/menu/babel_logo.png");
    
    int y_button = env->canvas->h() - 149;
    m_start = new Button(this, "start", "res/images/menu/button.png", X_BUTTON,
        y_button, W_BUTTON, H_BUTTON);
    m_loadgame = new Button(this, "loadgame", "res/images/menu/button.png", X_BUTTON + SPACING,
        y_button, W_BUTTON, H_BUTTON);
    m_settings = new Button(this, "settings", "res/images/menu/button.png", X_BUTTON + SPACING * 2,
        y_button, W_BUTTON, H_BUTTON);
    m_credits = new Button(this, "credits", "res/images/menu/button.png", X_BUTTON + SPACING * 3,
        y_button, W_BUTTON, H_BUTTON);
    m_exit = new Button(this, "exit", "res/images/menu/button.png", X_BUTTON + SPACING * 4,
        y_button, W_BUTTON, H_BUTTON);

    m_start->add_observer(this);
    m_loadgame->add_observer(this);
    m_settings->add_observer(this);
    m_credits->add_observer(this);
    m_exit->add_observer(this);

    add_child(m_start);
    add_child(m_loadgame);
    add_child(m_settings);
    add_child(m_credits);
    add_child(m_exit);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
}

void
Menu::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    int y_button = env->canvas->h() - 149.0 * scale;

    m_start->set_position(scale * X_BUTTON, y_button);
    m_loadgame->set_position(scale * (X_BUTTON + SPACING), y_button);
    m_settings->set_position(scale * (X_BUTTON + SPACING * 2), y_button);
    m_credits->set_position(scale * (X_BUTTON + SPACING * 3), y_button);
    m_exit->set_position(scale * (X_BUTTON + SPACING * 4), y_button);

    shared_ptr<Font> font = env->canvas->font();
    font->set_size(22 * scale);
}

void
Menu::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());

    int x_logo = (env->canvas->w() - m_logo->w())/2;
    env->canvas->draw(m_logo.get(), x_logo, 25);

    double scale = env->canvas->scale();

    set_position(m_start->x() + 15 * scale, m_start->y() + 15 * scale);
    env->canvas->draw("New Game", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(m_loadgame->x() + 15 * scale, m_loadgame->y() + 15 * scale);
    env->canvas->draw("Load Game", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(m_settings->x() + 32 * scale, m_settings->y() + 15 * scale);
    env->canvas->draw("Options", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(m_credits->x() + 32 * scale, m_credits->y() + 15 * scale);
    env->canvas->draw("Credits", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(m_exit->x() + 50 * scale, m_exit->y() + 15 * scale);
    env->canvas->draw("Exit", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));
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
        m_next = "gameover";
    }
    else if (button->id() == "loadgame")
    {
        m_next = "gameover";
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
