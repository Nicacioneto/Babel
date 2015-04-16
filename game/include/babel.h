#ifndef BABEL_H
#define BABEL_H

#include "game.h"
#include "environment.h"

class Babel : public Game
{
public:
	Babel() throw (Exception);
    
private:
	bool m_fullscreen;
    int m_w, m_h;
    Environment *env;
    void process_input();
    void draw();
};

#endif
