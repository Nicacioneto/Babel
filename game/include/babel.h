#ifndef BABEL_H
#define BABEL_H

#include "game.h"
#include "mousebuttoneventlistener.h"
#include "mousebuttonevent.h"

class Babel : public Game, public MouseButtonEventListener
{
public:
    Babel();
    
    void init(const string& title, int w = 800, int h = 600) throw (Exception);
    void run();

private:
    Level * load_frontend(const string& id);
    Level * load_menu();
    Level * load_settings();
    Level * load_credits();
    Level * load_level();

    bool onMouseButtonEvent(const MouseButtonEvent& event);
};

#endif
