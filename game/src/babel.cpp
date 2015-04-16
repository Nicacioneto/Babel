#include <ctime>
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
    env->canvas->clear();
    m_width = env->video->resolution().first;
    m_height = env->video->resolution().second;
    srand(time(NULL));
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

                case SDLK_UP:
                    m_width += 100;
                    m_height += 100;
                    env->video->set_resolution(m_width, m_height);
                    break;

                case SDLK_DOWN:
                    m_width -= 100;
                    m_height -= 100;
                    env->video->set_resolution(m_width, m_height);
                    break;
            }
        }
    }
}

void
Babel::draw()
{
    env->canvas->update();
}

void
Babel::load_menu() throw (Exception)
{
    Rect rect;
    rect.set(0, 0);
    rect.set_dimensions(m_width, m_height);
    env->canvas->load_image("res/images/menu.png", rect);
}
