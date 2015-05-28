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
    Level * load_colony();
    Level * load_credits();
    Level * load_dungeon();
    Level * load_gameover();
    Level * load_menu();
    Level * load_options();
    Level * load_planet();
    Level * load_play();
    Level * load_hospital();
};

#endif
