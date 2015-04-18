#include "babel.h"
#include "circle.h"
#include "image.h"
#include "point.h"
#include "rect.h"

using namespace std;

Babel::Babel() throw (Exception)
	: m_fullscreen(false)
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
    Rect rect;
    rect.set(0, 0);
    rect.set_dimensions(env->video->resolution().first, env->video->resolution().second);
    env->canvas->load_image("res/images/menu.png", rect);
}