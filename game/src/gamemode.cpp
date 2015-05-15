#include "gamemode.h"

#define X_BUTTON 25
#define Y_BUTTON 28
#define W_BUTTON 305

GameMode::GameMode(const string& next, const string&)
    : Level("", next), m_babelmode(nullptr), m_colonymode(nullptr), m_planetmode(nullptr)
{
    env = Environment::get_instance();

    m_babelmode = env->resources_manager->get_texture("res/images/modes/babelmode.png");
    m_colonymode = env->resources_manager->get_texture("res/images/modes/colonymode.png");
    m_planetmode = env->resources_manager->get_texture("res/images/modes/planetmode.png");

    m_babel = new Button(this, "babel", "res/images/modes/button.png");
    m_colony = new Button(this, "colony", "res/images/modes/button.png");
    m_planet = new Button(this, "planet", "res/images/modes/button.png");

    m_babel->add_observer(this);
    m_colony->add_observer(this);
    m_planet->add_observer(this);

    add_child(m_babel);
    add_child(m_colony);
    add_child(m_planet);
}

void
GameMode::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    m_babel->set_position(X_BUTTON * scale, Y_BUTTON * scale);
    m_colony->set_position((2*X_BUTTON + W_BUTTON) * scale, Y_BUTTON * scale);
    m_planet->set_position((3*X_BUTTON + 2*W_BUTTON) * scale, Y_BUTTON * scale);
}

void
GameMode::draw_self()
{
    double scale = env->canvas->scale();

    env->canvas->clear();
    env->canvas->draw(m_babelmode.get(), X_BUTTON * scale, Y_BUTTON * scale);
    env->canvas->draw(m_colonymode.get(), (2*X_BUTTON + W_BUTTON) * scale, Y_BUTTON * scale);
    env->canvas->draw(m_planetmode.get(), (3*X_BUTTON + 2*W_BUTTON) * scale, Y_BUTTON * scale);

}

bool
GameMode::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "babel")
    {
        // m_next = "gameover";
        // m_done = true;
    }
    else if (button->id() == "colony")
    {
        // m_next = "gameover";
        // m_done = true;
    }
    else if (button->id() == "planet")
    {
        // m_next = "gameover";
        // m_done = true;
    }

    return true;
}
