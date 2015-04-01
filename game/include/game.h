#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <string>

#define WIDTH 800
#define HEIGHT 600
#define BPP 0

class Game {
public:
    Game(std::string);
    ~Game();
    
    void run();

private:
	SDL_Window *window = nullptr;
	SDL_Surface *screen = nullptr;

    void process_timestep();
    void process_input();
    void run_ai();
    void collision_step();
    bool update_objects();
    void render();
};

#endif
