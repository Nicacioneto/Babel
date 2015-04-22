#ifndef BABEL_H
#define BABEL_H

#include "game.h"

class Babel : public Game
{
public:
    Babel();
    
    void init(const string& title, int w = 800, int h = 600) throw (Exception);
    void run();

private:
    Level * load_frontend(const string& id);
    Level * load_menu();
};

#endif
