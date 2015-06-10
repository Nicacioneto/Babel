#include "mapping.h"
#include <core/environment.h>
#include <core/bitmap.h>
#include <cmath>

Mapping::Mapping(double vertical_ratio, double horizontal_ratio)
    : m_vertical_ratio(vertical_ratio), m_horizontal_ratio(horizontal_ratio)
{
}

void
Mapping::draw_center(Bitmap *screen, Bitmap *bitmap, const Rect clip)
{
    int w = clip.x() + clip.w();
    int h = clip.y() + clip.h();

    for (int i = clip.x(); i < w; ++i)
    {
        for (int j = clip.y(); j < h; ++j)
        {
            double kx = (double) (i - clip.x())/clip.w();
            int posx = kx*bitmap->w();

            double ky = (double) (j - clip.y())/clip.h();
            int posy = ky*bitmap->h();

            Uint32 color = bitmap->getpixel(static_cast<SDL_Surface *>(bitmap->data()), posx, posy);
            bitmap->putpixel(static_cast<SDL_Surface *>(screen->data()), i, j, color);
        }
    }
}

void
Mapping::draw_walls(Bitmap *screen, Bitmap *bitmap, const Rect front, const Rect back)
{
    int w = abs(front.x() - back.x());
    int signal = front.x() <= back.x() ? 1 : -1;

    double top_y = front.y();
    double bot_y = front.y() + front.h();

    Environment *env = Environment::get_instance();

    for (int i = front.x(); i != back.x(); i += signal)
    {
        if (i < 0 or i >= env->canvas->w())
        {
            top_y += m_vertical_ratio;
            bot_y -= m_vertical_ratio;
            continue;
        }

        double num = abs(i - front.x());
        double kx = num/w;
        int posx = kx*bitmap->w();

        for (int j = top_y; j <= bot_y; ++j)
        {
            if (j < 0 or j >= env->canvas->h())
            {
                continue;
            }

            double ky = (double) fabs(j - top_y)/(bot_y - top_y);
            int posy = ky*bitmap->h();

            Uint32 color = bitmap->getpixel(static_cast<SDL_Surface *>(bitmap->data()), posx, posy);
            bitmap->putpixel(static_cast<SDL_Surface *>(screen->data()), i, j, color);
        }
        top_y += m_vertical_ratio;
        bot_y -= m_vertical_ratio;
    }
}

void
Mapping::draw_ceiling_floor(Bitmap *screen, Bitmap *bitmap, const Rect front, const Rect back)
{
    int h = abs(front.y() - back.y());
    int signal = front.y() <= back.y() ? 1 : -1;

    double left_x = front.x();
    double right_x = front.x() + front.w();

    Environment *env = Environment::get_instance();

    for (int i = front.y(); i != back.y(); i += signal)
    {
        if (i < 0 or i >= env->canvas->h())
        {
            left_x += m_horizontal_ratio;
            right_x -= m_horizontal_ratio;
            continue;
        } 

        double num = abs(i - front.y());
        double ky = num/h;
        int posy = ky*bitmap->h();

        for (int j = left_x; j <= right_x; ++j)
        {
            if (j < 0 or j >= env->canvas->w())
            {
                continue;
            }

            double kx = (double) fabs(j - left_x)/(right_x - left_x);
            int posx = kx*bitmap->w();

            Uint32 color = bitmap->getpixel(static_cast<SDL_Surface *>(bitmap->data()), posx, posy);
            bitmap->putpixel(static_cast<SDL_Surface *>(screen->data()), j, i, color);
        }
        left_x += m_horizontal_ratio;
        right_x -= m_horizontal_ratio;
    }
}
