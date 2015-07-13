/*
 * Babel class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "babel.h"
#include "barracks.h"
#include "base.h"
#include "central.h"
#include "combat.h"
#include "credits.h"
#include "drone.h"
#include "dungeon.h"
#include "facilities.h"
#include "gameover.h"
#include "hospital.h"
#include "menu.h"
#include "options.h"
#include "planet.h"
#include "play.h"
#include "squad.h"
#include "squad_planet.h"
#include "timer.h"
#include "tower.h"
#include "workshop.h"
#include <core/keyboardevent.h>
#include <core/settings.h>
#include <core/systemevent.h>
#include <util/frontend.h>

using std::to_string;

Babel::Babel()
    : Game("tiamat_logo"), m_slot(0)
{
}

Babel::~Babel()
{
}

Level *
Babel::load_level(const string& id)
{
    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings(env->m_settings_path);
    m_slot = settings->read<int>("Slots", "slot", 0);

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
        return new Base(m_slot);
    }
    else if (id == "dungeon")
    {
        return new Dungeon(m_slot);
    }
    else if (id == "planet")
    {
        return new Planet(m_slot);
    }
    else if (id == "barracks")
    {
        return new Barracks(m_slot);
    }
    else if (id == "facilities")
    {
        return new Facilities(m_slot);
    }
    else if (id == "hospital")
    {
        return new Hospital(m_slot);
    }
    else if (id == "workshop")
    {
        return new Workshop(m_slot);
    }
    else if (id == "central")
    {
        return new Central(m_slot);
    }
    else if (id == "gameover")
    {
        return new GameOver();
    }
    else if (id == "combat")
    {
        return new Combat(m_slot);
    }
    else if (id == "tower")
    {
        return new Tower(m_slot);
    }
    else if (id == "squad")
    {
        return new Squad(m_slot);
    }
    else if (id == "squad_planet")
    {
        return new SquadPlanet(m_slot);
    }
    else if (id == "drone")
    {
        return new Squad(m_slot, Squad::DRONE);
    }

    return new Facilities(m_slot);
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
    if (event.state() == KeyboardEvent::PRESSED and
        event.key() == KeyboardEvent::ESCAPE)
    {
        m_level->set_next("menu");
        m_level->finish();
        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);
    }
    else if (event.state() == KeyboardEvent::PRESSED and
        event.key() == KeyboardEvent::F11)
    {
        shared_ptr<Settings> settings = env->resources_manager->get_settings(env->m_settings_path);

        env->video->set_fullscreen((env->video->fullscreen() + 1) % 2);
        settings->write<bool>("Game", "fullscreen", env->video->fullscreen());
        settings->save(env->m_settings_path);
    }

    return false;
}

bool
Babel::on_event(const SystemEvent& event)
{
    if (event.type() == SystemEvent::QUIT)
    {
        m_done = true;
        kill_threads();

        return true;
    }

    return false;
}
