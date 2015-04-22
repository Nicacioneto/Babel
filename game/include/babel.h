#ifndef BABEL_H
#define BABEL_H

#include "game.h"
#include "environment.h"
#include "state.h"

class Babel : public Game
{
public:
	Babel();
    void load_menu() throw (Exception);
    
private:
    string m_id;
	bool m_fullscreen;
    int m_width, m_height;
    State* m_state;
    Environment *env;
    void process_input();
    void draw();
};

#endif
