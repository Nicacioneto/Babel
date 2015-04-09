#ifndef VOIDCRAWLERS_H
#define VOIDCRAWLERS_H

#include "game.h"
#include "environment.h"

class VoidCrawlers : public Game
{
public:
	VoidCrawlers() throw (Exception);
    
private:
	bool m_fullscreen;
    int m_w, m_h;
    Environment *env;
    void process_input();
};

#endif
