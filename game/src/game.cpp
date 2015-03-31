#include "../include/game.h"
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

Game::Game() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        exit(-1);
    }

    m_window = SDL_CreateWindow("Void Crawlers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (m_window == NULL) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(-1);
    }

    m_screen = SDL_GetWindowSurface(m_window);

    if (!m_screen) {
        cout << "Surface can not be created! SDL_: " << SDL_GetError() << endl;
        exit(-1);
    }
}

Game::~Game() {
    SDL_FreeSurface( m_screen );
    m_screen = NULL;

    //Destroy m_window
    SDL_DestroyWindow( m_window );
    m_window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void Game::run() {
    cout << "Game loop" << endl;

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

    while( SDL_PollEvent( &event ) != 0 )
    {
        if( event.type == SDL_QUIT )
            return true;
    }

    return false;
}

void Game::render() {}
