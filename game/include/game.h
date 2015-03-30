#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL2/SDL.h>

class Game {
public:
    Game();
    ~Game();
    
    void run();

private:
	SDL_Window* window = NULL;
	SDL_Surface* screen = NULL;

    void process_timestep();
    void process_input();
    void run_ai();
    void collision_step();
    bool update_objects();
    void render();
};

#endif