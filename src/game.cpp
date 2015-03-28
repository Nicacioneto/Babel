#include "../include/game.h"
#include <iostream>
#include <SDL/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define BPP 0

using namespace std;

Game::Game() {}

Game::~Game() {}

void Game::run() {
    cout << "Game loop" << endl;

    bool done = false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Error: " << SDL_GetError() << endl;
        exit(-1);
    }

    SDL_Surface *screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_SWSURFACE | SDL_ANYFORMAT);

    if (!screen) {
        cout << "Error: " << SDL_GetError() << endl;
        exit(-1);
    }

    while (not done) {
        process_timestep();
        process_input();
        run_ai();
        collision_step();
        done = update_objects();
        render();
    }
}

void Game::process_timestep() {}

void Game::process_input() {}

void Game::run_ai() {}

void Game::collision_step() {}

bool Game::update_objects() {
    return false;
}

void Game::render() {}
