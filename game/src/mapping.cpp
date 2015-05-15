#include "mapping.h"
#include <core/environment.h>
#include "sdlaux.h"
#include <cmath>
#include <core/texture.h>

Mapping::Mapping(double ratio)
    : m_ratio(ratio)
{
}

void
Mapping::draw(Texture *texture, const Rect clip)
{
    int w = clip.x() + clip.w();
    int h = clip.y() + clip.h();

    for (int i = clip.x(); i < w; ++i)
    {
        for (int j = clip.y(); j < h; ++j)
        {
            double kx = (double) (i - clip.x())/clip.w();
            int posx = kx*texture->w();

            double ky = (double) (j - clip.y())/clip.h();
            int posy = ky*texture->h();

            Environment *env = Environment::get_instance();
            SDL_Surface * pSurface = SDL_CreateRGBSurface(0, env->canvas->w(), env->canvas->h(),
                32, 0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
            Uint32 color = getpixel((SDL_Surface *)texture->data(), posx, posy);
            putpixel(pSurface, i, j, color);
        }
    }
}

void
Mapping::draw(Texture *texture, const Rect front, const Rect back)
{
    int diff = front.x() < 0 ? -front.x() : 0;
    int w = abs(front.x() - back.x());
    int signal = front.x() <= back.x() ? 1 : -1;
printf("diff = %d, w = %d, signal = %d\n", diff, w, signal);

    double top_y = front.y();
    double bot_y = front.y() + front.h();
printf("top_y = %.3f, bot_y = %.3f\n", top_y, bot_y);

    Environment *env = Environment::get_instance();

    for (int i = front.x(); i != back.x(); i += signal)
    {
        if (i < 0 or i >= env->canvas->w())
        {
            top_y += m_ratio;
            bot_y -= m_ratio;
            continue;
        }

        double num = abs(i - front.x());
        double kx = num/w;
        int posx = kx*texture->w();

        for (int j = top_y; j <= bot_y; ++j)
        {
            if (j < 0 or j >= env->canvas->h())
                continue;

            double ky = (double) fabs(j - top_y)/(bot_y - top_y);
            int posy = ky*texture->h();


            // SDL_Texture * pTexture = SDL_CreateTexture(env->canvas->renderer(), SDL_PIXELFORMAT_RGBA8888,
            //     SDL_TEXTUREACCESS_STREAMING, env->canvas->w(), env->canvas->h());

            SDL_Surface * pSurface = SDL_CreateRGBSurface(0, env->canvas->w(), env->canvas->h(),
                32, 0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
            Uint32 color = getpixel((SDL_Surface *)texture->data(), posx, posy);
            putpixel(pSurface, i, j, color);
        }
        top_y += m_ratio;
        bot_y -= m_ratio;
    }
}