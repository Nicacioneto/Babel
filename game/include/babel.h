#ifndef BABEL_H
#define BABEL_H

#include <core/game.h>

class Babel : public Game
{
public:
    Babel();
    ~Babel();

private:
	bool on_event(const KeyboardEvent& event);
	
    Level * load_level(const string& id);
    Level * load_frontend(const string& id);
};

#endif
