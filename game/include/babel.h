#ifndef BABEL_H
#define BABEL_H

#include "game.h"
#include "environment.h"
#include "state.h"

class Babel : public Game
{
public:
	Babel();
    
private:
    Environment *env;
    void process_input();
};

#endif
