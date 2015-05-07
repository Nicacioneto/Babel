#include "babel.h"
#include "credits.h"
#include "environment.h"
#include "frontend.h"
#include "menu.h"
#include "settings.h"

Babel::Babel()
    : Game("tiamat_logo")
{
    env = Environment::get_instance();
    env->events_manager->register_mouse_button_event_listener(this);
}

Babel::~Babel()
{
    env->events_manager->unregister_mouse_button_event_listener(this);
}

Level *
Babel::load_level(const string& id)
{
    m_id = id;

    if (id == "")
    {
        return nullptr;
    }
    else if (id == "menu")
    {
        return load_menu();
    }
    else if (id == "settings")
    {
        return load_settings();
    }
    else if (id == "credits")
    {
        return load_credits();
    }
    
    return load_frontend(id);
}

Level *
Babel::load_frontend(const string& id)
{
    if (id == "tiamat_logo")
    {
        return new FrontEnd("sdl_logo", "res/images/tiamat_logo.png");
    }
    else if (id == "sdl_logo")
    {
        return new FrontEnd("classification", "res/images/sdl_logo.png");
    }
    else if (id == "classification")
    {
        return new FrontEnd("menu", "res/images/classification.png");
    }

    return nullptr;
}

Level *
Babel::load_menu()
{
    return new Menu();
}

Level *
Babel::load_settings()
{
    return new Settings();
}

Level *
Babel::load_credits()
{
    return new Credits();
}

bool
Babel::onMouseButtonEvent(const MouseButtonEvent& event)
{
    if (event.state() == MouseButtonEvent::RELEASED and
        event.button() == MouseButtonEvent::LEFT)
    {
        if (m_id == "menu")
        {
            Menu *menu = dynamic_cast<Menu *>(m_level);
            menu->execute_action(event.x(), event.y());
            return true;
        }
        else if (m_id == "settings")
        {
            Settings *settings = dynamic_cast<Settings *>(m_level);
            settings->execute_action(event.x(), event.y());
            return true;
        }
        else if (m_id == "credits")
        {
            Credits *credits = dynamic_cast<Credits *>(m_level);
            credits->execute_action(event.x(), event.y());
            return true;
        }
    }

    return false;
}
