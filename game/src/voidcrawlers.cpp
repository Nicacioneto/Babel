#include "voidcrawlers.h"

using namespace std;

VoidCrawlers::VoidCrawlers() throw (Exception)
	: m_fullscreen(false), m_w(800), m_h(600)
{
    env = Environment::get_instance();
}

void 
VoidCrawlers::process_input()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        env->video->clear();

        if (event.type == SDL_QUIT)
        {
            m_done = true;
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_f:
                    m_fullscreen = not m_fullscreen;
                    env->video->set_fullscreen(m_fullscreen);
                    break;

                case SDLK_UP:
                    m_w += 100;
                    m_h += 100;
                    env->video->set_resolution(m_w, m_h);
                    break;

                case SDLK_DOWN:
                    m_w -= 100;
                    m_h -= 100;
                    env->video->set_resolution(m_w, m_h);
                    break;
            }
        }
    }
}