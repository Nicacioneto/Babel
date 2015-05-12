#ifndef MAPPING_H
#define MAPPING_H

#include <SDL/SDL.h>
#include <math.h>

#include "sdlaux.h"

class Mapping
{
public:
    Mapping(SDL_Surface *screen, double ratio = 0.6)
        : m_screen(screen), m_ratio(ratio)
    {
    }

    void draw(SDL_Surface *img, const SDL_Rect& clip)
    {
        int w = clip.x + clip.w;
        int h = clip.y + clip.h;

        for (int i = clip.x; i < w; ++i)
        {
            for (int j = clip.y; j < h; ++j)
            {
                double kx = (double) (i - clip.x)/clip.w;
                int posx = kx*img->w;

                double ky = (double) (j - clip.y)/clip.h;
                int posy = ky*img->h;

                Uint32 color = getpixel(img, posx, posy);
                putpixel(m_screen, i, j, color);
            }
        }
    }

    void draw(SDL_Surface *img, const SDL_Rect& front, const SDL_Rect& back)
    {
        int diff = front.x < 0 ? -front.x : 0;
        int w = abs(front.x - back.x);
        int signal = front.x <= back.x ? 1 : -1;
printf("diff = %d, w = %d, signal = %d\n", diff, w, signal);

        double top_y = front.y;
        double bot_y = front.y + front.h;
printf("top_y = %.3f, bot_y = %.3f\n", top_y, bot_y);
        for (int i = front.x; i != back.x; i += signal)
        {
            if (i < 0 or i >= m_screen->w)
            {
                top_y += m_ratio;
                bot_y -= m_ratio;
                continue;
            }

//printf("front.x = %d, back.x = %d, diff = %d\n", front.x, back.x, abs(front.x - back.x));
            double num = abs(i - front.x);
//printf("num = %.3f, den = %.3f\n", num, w);
            double kx = num/w;
            int posx = kx*img->w;

//printf("kx = %.3f, posx = %d, i = %d\n", kx, posx, i);
//printf("topy = %.3f, boty = %.3f\n", top_y, bot_y);
            for (int j = top_y; j <= bot_y; ++j)
            {
                if (j < 0 or j >= m_screen->h)
                    continue;

                double ky = (double) fabs(j - top_y)/(bot_y - top_y);
                int posy = ky*img->h;

//printf("ky = %.3f, posy = %d, j = %d\n", ky, posy, j);
                Uint32 color = getpixel(img, posx, posy);
                putpixel(m_screen, i, j, color);
            }
            top_y += m_ratio;
            bot_y -= m_ratio;
        }
    }

private:
    SDL_Surface *m_screen;
    double m_ratio;
};

#endif
