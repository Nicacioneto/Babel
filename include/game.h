#ifndef GAME_H
#define GAME_H

#include <iostream>

class Game {
public:
    Game();
    ~Game();
    
    void run();

private:
    void process_timestep();
    void process_input();
    void run_ai();
    void collision_step();
    bool update_objects();
    void render();
};

#endif