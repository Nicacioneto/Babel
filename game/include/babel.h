#ifndef BABEL_H
#define BABEL_H

#include "game.h"
#include "environment.h"

class Babel : public Game
{
public:
	Babel() throw (Exception);
    void load_menu() throw (Exception);
    
private:
	bool m_fullscreen;
    int m_width, m_height;
    Environment *env;
    void process_input();
    void draw();
};

#endif