#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define BPP 0

class Game {
public:
    Game();
    ~Game();
    
    void run();

private:
	SDL_Window *m_window = NULL;
	SDL_Surface *m_screen = NULL;

    void process_timestep();
    void process_input();
    void run_ai();
    void collision_step();
    bool update_objects();
    void render();
};

#endif