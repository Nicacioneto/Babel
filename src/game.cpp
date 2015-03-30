#include "include/game.h"

Game* Game::instacia = 0;

Game::Game(string title, int width, int height){

    this->width = width;
    this->height = height;

    saida = false;

    if(instacia == NULL)
        instacia = this;

    int erro = SDL_Init(SDL_INIT_VIDEO);
    if (erro != 0){

        cout << endl << "CRASH VIDEO" << endl;
        cout << SDL_GetError() << endl;

    }

    if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF)== 0){

        cout << endl << "CRASH IMAGEM" << endl;
        cout << SDL_GetError() << endl;

    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

    if(render == NULL){

        cout << endl << "CRASH WINDOW" << endl;
        cout << SDL_GetError() << endl;
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(render == NULL){

        cout << endl << "CRASH RENDER" << endl;
        cout << SDL_GetError() << endl;
    }
}

Game::~Game(){

    IMG_Quit();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

}

Game& Game::Get_instancia(){

    return *instacia;
}

void Game::Pilha_push(State *state){

    pilha = state;

}

//Game& Game::Get_estado(){

//    pilha =
//}

void Game::Run(){

    pilha = NULL;
    estado_pilha.emplace(pilha);

    while(!estado_pilha.empty() && estado_pilha.top()->Requested_quit()){

    Delta_time();
  //  Input_manager::Get_instacia.update();

    if(pilha != NULL){

        estado_pilha.emplace(pilha);
        pilha = NULL;

        }
    while(!estado_pilha.empty() && !(estado_pilha.top()->Requested_delete())){

        estado_pilha.pop();

        if(pilha != NULL){

            estado_pilha.emplace(pilha);
            pilha = NULL;

            }
        }

    if(!estado_pilha.empty()){

        estado_pilha.top()->Update(delta_time);
        estado_pilha.top()->Rendering();
        SDL_RenderPresent(render);
        SDL_Delay(33);

        }
    }
}

void Game::Delta_time(){

    int tempo = SDL_GetTicks();
    frame = tempo;
    delta_time = (float)((tempo-frame)/1000);

}

int Game::get_Window_Height(){

    int h,w;
    SDL_GetWindowSize(window, &w, &h);

    return h;

}

int Game::get_Window_Width(){

    int h,w;
    SDL_GetWindowSize(window, &w, &h);

    return w;

}
