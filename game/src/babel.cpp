#include "babel.h"
#include "credits.h"
#include "dungeon.h"
#include "gamemode.h"
#include "gameover.h"
#include "menu.h"
#include "play.h"
#include "options.h"
#include <util/frontend.h>

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
        return load_menu();
    }
    else if (id == "options")
    {
        return load_options();
    }
    else if (id == "credits")
    {
        return load_credits();
    }
    else if (id == "play")
    {
        return load_play();
    }
    else if (id == "gamemode")
    {
        return load_gamemode();
    }
    else if (id == "gameover")
    {
        return load_gameover();
    }
    else if (id == "dungeon")
    {
        return load_dungeon();
    }
    
    return load_frontend(id);
}

Level *
Babel::load_frontend(const string& id)
{
    if (id == "tiamat_logo")
    {
        return new FrontEnd(id, "sdl_logo", "res/images/frontend/tiamat-logo.png");
    }
    else if (id == "sdl_logo")
    {
        return new FrontEnd(id, "classification", "res/images/frontend/sdl-logo.png");
    }
    else if (id == "classification")
    {
        return new FrontEnd(id, "menu", "res/images/frontend/classification.png");
    }

    return nullptr;
}

Level *
Babel::load_menu()
{
    return new Menu();
}

Level *
Babel::load_options()
{
    return new Options();
}

Level *
Babel::load_credits()
{
    return new Credits();
}

Level *
Babel::load_play()
{
    return new Play();
}

Level *
Babel::load_gamemode()
{
    return new GameMode();
}

Level *
Babel::load_gameover()
{
    return new GameOver();
}

Level *
Babel::load_dungeon()
{
    return new Dungeon();
}
