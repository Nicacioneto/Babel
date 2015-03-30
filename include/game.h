#ifndef GAME
#define GAME

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <stdio.h>

#include <vector>
#include <memory>
#include <time.h>
#include <stack>

#include "include/state.h"

using std::string;
using std::cout;
using std::endl;

//class State;

class Game{

public:
    Game(string title, int width, int height);
    ~Game();

    SDL_Renderer* Get_render;
    static Game& Get_instancia();
    static State& Get_estado();

    void Pilha_push(State* state);

    void Run();

    int get_Window_Width();
    int get_Window_Height();

private:
    int width;
    int height;
    int frame;
    float delta_time;
    void Delta_time();
    static Game* instacia;

    SDL_Window* window;
    SDL_Renderer* render;

    State* pilha;

//    std::stack< std::unique_ptr<State> >  estado_pilha;

    std::stack<std::unique_ptr<State>> estado_pilha;

//    stack<State*> estado_pilha;
    bool saida;

};


#endif // GAME

