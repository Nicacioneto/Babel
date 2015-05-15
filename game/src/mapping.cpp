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

            Uint32 color = getpixel(env->canvas->bitmap(), posx, posy);
            putpixel(env->canvas->bitmap(), i, j, color);
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

            Uint32 color = getpixel(env->canvas->bitmap(), posx, posy);
            putpixel(env->canvas->bitmap(), i, j, color);
        }
        top_y += m_ratio;
        bot_y -= m_ratio;
    }
}