#ifndef BABEL_H
#define BABEL_H

#include <core/game.h>

class Babel : public Game
{
public:
    Babel();
    ~Babel();

private:
    Level * load_level(const string& id);
    Level * load_frontend(const string& id);
    Level * load_menu();
    Level * load_settings();
    Level * load_credits();
    Level * load_play();
    Level * load_gamemode();
    Level * load_gameover();
    Level * load_dungeon();
};

#endif
