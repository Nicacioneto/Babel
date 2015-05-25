#include "gamemode.h"

GameMode::GameMode(const string& next, const string&)
    : Level("", next), m_babelmode(nullptr), m_colonymode(nullptr), m_planetmode(nullptr),
        m_babel(nullptr), m_colony(nullptr), m_planet(nullptr)
{
    Environment *env = Environment::get_instance();

    m_babelmode = env->resources_manager->get_texture("res/images/modes/babelmode.png");
    m_colonymode = env->resources_manager->get_texture("res/images/modes/colonymode.png");
    m_planetmode = env->resources_manager->get_texture("res/images/modes/planetmode.png");

    double scale = env->canvas->scale();

    const int x_button = 25 * scale;
    const int y_button = 28 * scale;
    const int w_button = 305 * scale;
    const int h_button = 716 * scale;

    m_babel = new Button(this, "tower", "res/images/modes/button.png", x_button,
        y_button, w_button, h_button);
    m_colony = new Button(this, "colony", "res/images/modes/button.png", 2 * x_button + w_button,
        y_button, w_button, h_button);
    m_planet = new Button(this, "planet", "res/images/modes/button.png",
        3 * x_button + 2 * w_button, y_button, w_button, h_button);

    m_babel->add_observer(this);
    m_colony->add_observer(this);
    m_planet->add_observer(this);

    add_child(m_babel);
    add_child(m_colony);
    add_child(m_planet);
}

void
GameMode::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    
    env->canvas->draw(m_babelmode.get(), m_babel->x(), m_babel->y());
    env->canvas->draw(m_colonymode.get(), m_colony->x(), m_colony->y());
    env->canvas->draw(m_planetmode.get(), m_planet->x(), m_planet->y());
}

bool
GameMode::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "tower")
    {
        set_next("dungeon");
    }
    else if (button->id() == "colony")
    {
        set_next("gameover");
    }
    else if (button->id() == "planet")
    {
        set_next("gameover");
    }

    finish();

    return true;
}
