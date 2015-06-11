#include "babel.h"
#include "base.h"
#include "central.h"
#include "combat.h"
#include "credits.h"
#include "dungeon.h"
#include "facilities.h"
#include "gameover.h"
#include "hospital.h"
#include "menu.h"
#include "options.h"
#include "planet.h"
#include "play.h"
#include <util/frontend.h>
#include <core/keyboardevent.h>
#include <core/settings.h>

Babel::Babel()
    : Game("tiamat_logo")
{
}

Babel::~Babel()
{
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
        return new Menu();
    }
    else if (id == "options")
    {
        return new Options();
    }
    else if (id == "credits")
    {
        return new Credits();
    }
    else if (id == "play")
    {
        return new Play();
    }
    else if (id == "base")
    {
        return new Base();
    }
    else if (id == "dungeon")
    {
        return new Dungeon();
    }
    else if (id == "planet")
    {
        return new Planet();
    }
    else if (id == "barracks")
    {
        // TODO
        return new Base();
    }
    else if (id == "facilities")
    {
        return new Facilities();
    }
    else if (id == "hospital")
    {
        return new Hospital();
    }
    else if (id == "workshop")
    {
        // TODO
        return new Base();
    }
    else if (id == "central")
    {
        return new Central();
    }
    else if (id == "gameover")
    {
        return new GameOver();
    }
    else if (id == "combat")
    {
        return new Combat();
    }
    
    return load_frontend(id);
}

Level *
Babel::load_frontend(const string& id)
{
    string path = "res/images/frontend/";
    if (id == "tiamat_logo")
    {
        return new FrontEnd(id, "sdl_logo", path + "tiamat-logo.png");
    }
    else if (id == "sdl_logo")
    {
        return new FrontEnd(id, "classification", path + "sdl-logo.png");
    }
    else if (id == "classification")
    {
        return new FrontEnd(id, "menu", path + "classification.png");
    }

    return nullptr;
}

bool
Babel::on_event(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED
        and event.key() == KeyboardEvent::ESCAPE)
    {
        m_level->set_next("menu");
        m_level->finish();
        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);
    }
    else if (event.state() == KeyboardEvent::PRESSED
        and event.key() == KeyboardEvent::F11)
    {
        shared_ptr<Settings> settings = env->resources_manager->get_settings(env->m_settings_path);

        if (env->video->fullscreen())
        {
            env->video->set_fullscreen(false);
            settings->write<bool>("Game", "fullscreen", false);
        }
        else
        {
            env->video->set_fullscreen(true);
            settings->write<bool>("Game", "fullscreen", true);
        }

        settings->save(env->m_settings_path);
    }

    return false;
}
