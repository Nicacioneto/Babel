#include "button.h"
#include "menu.h"
#include "rect.h"
#include "resourcesmanager.h"

#define X_BUTTON 107
#define W_BUTTON 140
#define H_BUTTON 60
#define SPACING 165

Menu::Menu(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_logo(nullptr), m_start(nullptr), m_loadgame(nullptr),
        m_settings(nullptr), m_credits(nullptr), m_exit(nullptr)
{
    env = Environment::get_instance();

    env->canvas->load_font("res/fonts/exo-2/Exo2.0-Regular.otf", 50);
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
}

void
Menu::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());

    int x_logo = (env->canvas->w() - m_logo->w())/2;
    env->canvas->draw(m_logo.get(), x_logo, 25);

    double scale = env->canvas->scale();

    int x = (m_start->x()+15)/scale, y = (m_start->y()+10)/scale;
    int w = m_start->w()-25, h = m_start->h()-25;
    env->canvas->draw_message("New Game", Rect(x, y, w, h), Color(170, 215, 190));

    x = (m_loadgame->x()+15)/scale, y = (m_loadgame->y()+10)/scale;
    w = m_loadgame->w()-25, h = m_loadgame->h()-25;
    env->canvas->draw_message("Load Game", Rect(x, y, w, h), Color(170, 215, 190));

    x = (m_settings->x()+15)/scale, y = (m_settings->y()+10)/scale;
    w = m_settings->w()-25, h = m_settings->h()-25;
    env->canvas->draw_message("Options", Rect(x, y, w, h), Color(170, 215, 190));

    x = (m_credits->x()+15)/scale, y = (m_credits->y()+10)/scale;
    w = m_credits->w()-25, h = m_credits->h()-25;
    env->canvas->draw_message("Credits", Rect(x, y, w, h), Color(170, 215, 190));

    x = (m_exit->x()+15)/scale, y = (m_exit->y()+10)/scale;
    w = m_exit->w()-25, h = m_exit->h()-25;
    env->canvas->draw_message("Exit", Rect(x, y, w, h), Color(170, 215, 190));
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
