#include "babel.h"
#include "button.h"
#include "circle.h"
#include "image.h"
#include "point.h"
#include "rect.h"
#include "state_menu.h"
#include <iostream>

using namespace std;

Babel::Babel() 
    : Game("a")
    // m_fullscreen(false)
    // m_state(new StateMenu())
{
    env = Environment::get_instance();
}

void 
Babel::process_input()
{
    SDL_Event event;

    SDL_Delay(5);
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            m_done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_f:
                    m_fullscreen = not m_fullscreen;
                    env->video->set_fullscreen(m_fullscreen);
                    break;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            //Waited implementation
        }
    }
}

void
Babel::draw()
{
    env->canvas->clear();
    load_menu();
    env->canvas->update();
}

void
Babel::load_menu() throw (Exception)
{
    m_state->load();
}
