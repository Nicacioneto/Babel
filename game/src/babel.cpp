#include "babel.h"
#include <ctime>
#include "point.h"
#include "circle.h"

using namespace std;

Babel::Babel() throw (Exception)
	: m_fullscreen(false), m_w(800), m_h(600)
{
    env = Environment::get_instance();
    env->canvas->clear();
    srand(time(NULL));
}

void 
Babel::process_input()
{
    SDL_Event event;

    SDL_Delay(1);
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
        env->canvas->clear();
        draw_home();
    }
}

void
Babel::draw()
{
    env->canvas->update();
}

void 
Babel::draw_home()
{
    Point point;
    Circle circle;
    Color color;

    for(int i = 0; i < 300; i++){
        point.set(rand() % m_w, rand() % m_h);
        color.set(254, 254, 254);
        env->canvas->draw(point, color);
    }
    
    circle.set(Point(m_w/2, m_h-1), m_w/2);
    color.set(159, 153, 172);
    env->canvas->fill(circle, color);

    // env->canvas->update();
}
