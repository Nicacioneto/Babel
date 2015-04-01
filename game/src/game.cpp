#include "game.h"
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

Game::Game(string game_name) {
    int initialize = SDL_Init(SDL_INIT_VIDEO); 
    if (initialize != 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        exit(-1);
    }

    this->window = SDL_CreateWindow(game_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (this->window == nullptr) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(-1);
    }

    this->screen = SDL_GetWindowSurface(this->window);

    if (!this->screen) {
        cout << "Surface could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(-1);
    }
}

Game::~Game() {
    SDL_FreeSurface(this->screen);
    this->screen = nullptr;

    SDL_DestroyWindow(this->window);
    this->window = nullptr;

    SDL_Quit();
}

void Game::run() {
    bool done = false;
    
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
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            return true;
        }
    }

    return false;
}

void Game::render() {}
