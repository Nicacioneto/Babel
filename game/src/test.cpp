#include <SDL/SDL.h>

#include <stdlib.h>

#define VELOCITY 10

Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void map_center(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest);

void map_left(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio);

void map_right(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio);

void map_up(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio);

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, 0);

    SDL_Surface *img = SDL_LoadBMP("res/images/door.bmp");
    SDL_Surface *door = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);
    
    img = SDL_LoadBMP("res/images/hall.bmp");
    SDL_Surface *hall = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);

    img = SDL_LoadBMP("res/images/ceiling.bmp");
    SDL_Surface *ceiling = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);

    SDL_Rect up { 0, 65, 800, 55 };
    map_up(screen, ceiling, up, 2.0);

    up.x = 110;
    up.y = 120;
    up.w = 580;
    up.h = 55;

    map_up(screen, ceiling, up, 2.0);

    up.x = 220;
    up.y = 175;
    up.w = 360;
    up.h = 55;

    map_up(screen, ceiling, up, 2.0);

    SDL_Rect center { 330, 230, 140, 140 }; 
    map_center(screen, door, center);

    /* 3 paredes com o mesmo tamanho */
    SDL_Rect left { 330, 230, 110, 140 };
    map_left(screen, hall, left, 0.5);

    left.x = 220;
    left.y = 175;
    left.w = 110;
    left.h = 250;
    map_left(screen, hall, left, 0.5);

    left.x = 110;
    left.y = 120;
    left.w = 110;
    left.h = 360;
    map_left(screen, hall, left, 0.5);

    SDL_Rect right { 470, 230, 110, 140 };
    map_right(screen, hall, right, 0.5);

    right.x = 580;
    right.y = 175;
    right.w = 110;
    right.h = 250;
    map_right(screen, hall, right, 0.5);

    right.x = 690;
    right.y = 120;
    right.w = 110;
    right.h = 360;
    map_right(screen, hall, right, 0.5);

    /* 3 parades da esquerda com o tamanho diferente */
    // SDL_Rect left { 330, 230, 55, 140 };
    // map_left(screen, hall, left, 0.5);

    // left.x = 275;
    // left.y = 200;
    // left.w = 165;
    // left.h = 200;
    // map_left(screen, hall, left, 0.5);

    // left.x = 110;
    // left.y = 120;
    // left.w = 110;
    // left.h = 360;
    // map_left(screen, hall, left, 0.5);

    // SDL_Rect right { 500, 200, 150, 200 };
    // map_right(screen, hall, right, 0.5);

    // right.x = 650;
    // right.y = 125;
    // right.h = 350;
    // map_right(screen, hall, right, 0.5); 


    SDL_Flip(screen);

    SDL_Event event;

    int step = 1;

    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP)
                {

                    center.x = 330 - VELOCITY*step;
                    center.y =  230  - VELOCITY*step*0.5;
                    center.w =  140 + VELOCITY*step*2;
                    center.h =  140 + VELOCITY*step;

                    map_center(screen, door, center);

                    up.x = 220 - VELOCITY*step;
                    up.y = 175 - VELOCITY*step*0.5;
                    up.w = 360 + VELOCITY*step*2;
                    map_up(screen, ceiling, up, 2);

                    up.x = 110 - VELOCITY*step;
                    up.y = 120 - VELOCITY*step*0.5;
                    up.w = 580 + VELOCITY*step*2;
                    map_up(screen, ceiling, up, 2);
                    
                    up.x = 0 - VELOCITY*step;
                    up.y = 65 - VELOCITY*step*0.5;
                    up.w = 800 + VELOCITY*step*2;
                    map_up(screen, ceiling, up, 2);

                    left.x = 330 - VELOCITY*step;
                    left.y = 230 - VELOCITY*step*0.5;
                    left.w = 110;
                    left.h = 140 + VELOCITY*step;
                    map_left(screen, hall, left, 0.5);

                    left.x = 220 - VELOCITY*step;
                    left.y = 175 - VELOCITY*step*0.5;
                    left.w = 110;
                    left.h = 250 + VELOCITY*step;
                    map_left(screen, hall, left, 0.5);

                    left.x = 110 - VELOCITY*step;
                    left.y = 120 - VELOCITY*step*0.5;
                    left.w = 110;
                    left.h = 360 + VELOCITY*step;
                    map_left(screen, hall, left, 0.5);

                    right.x = 470 + VELOCITY*step;
                    right.y = 230 - VELOCITY*step*0.5;
                    right.w = 110;
                    right.h = 140 + VELOCITY*step;
                    map_right(screen, hall, right, 0.5);

                    right.x = 580 + VELOCITY*step;
                    right.y = 175 - VELOCITY*step*0.5;
                    right.w = 110;
                    right.h = 250 + VELOCITY*step;
                    map_right(screen, hall, right, 0.5);

                    right.x = 690 + VELOCITY*step;
                    right.y = 120 - VELOCITY*step*0.5;
                    right.w = 110;
                    right.h = 360 + VELOCITY*step;
                    map_right(screen, hall, right, 0.5);

                    step++;
                    SDL_Flip(screen);
                }
            }
            SDL_Delay(10);
        }
        SDL_Delay(10);
    }

    SDL_FreeSurface(door);
    SDL_FreeSurface(hall);
    return 0;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
} 

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void
map_center(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest)
{
    for (int i = dest.x; i < dest.x + dest.w && i >= 0; i++)
    {
        for (int j = dest.y; j < dest.y + dest.h; j++)
        {
            double x = (double) (i - dest.x)/(dest.w);
            double y = (double) (j - dest.y)/(dest.h);

            int posx = x*img->w;
            int posy = y*img->h;

            Uint32 color = getpixel(img, posx, posy);
            putpixel(screen, i, j, color);
        }
    }
}

void map_left(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio)
{
    double top_y = dest.y;
    double bot_y = dest.y + dest.h - 1;

    for (int i = dest.x; i > dest.x - dest.w && i > 0; --i)
    {
        for (int j = top_y; j <= bot_y; ++j)
        {
            double x = (double) (i - dest.x)/(dest.w);
            double y = (double) (j - top_y)/(bot_y - top_y);

            int posx = x*img->w;
            int posy = y*img->h;

            Uint32 color = getpixel(img, posx, posy);
            putpixel(screen, i, j, color);
        }
        
        top_y -= ratio;
        bot_y += ratio;
    }
}

void map_right(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio)
{
    double top_y = dest.y;
    double bot_y = dest.y + dest.h - 1;

    for (int i = dest.x; i < dest.x + dest.w && i < 800; ++i)
    {
        for (int j = top_y; j <= bot_y; ++j)
        {
            double x = (double) (i - dest.x)/(dest.w);
            double y = (double) (j - top_y)/(bot_y - top_y);

            int posx = x*img->w;
            int posy = y*img->h;

            Uint32 color = getpixel(img, posx, posy);
            putpixel(screen, i, j, color);
        }
        
        top_y -= ratio;
        bot_y += ratio;
    }
}

void map_up(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio)
{
    double left_x = dest.x;
    double right_x = dest.x + dest.w - 1;

    for (int j = dest.y + dest.h; j > dest.y && j > 65; --j)
    {
        for (int i = left_x; i < right_x; ++i)
        {
            // double x = (double) (i - dest.x)/(dest.w);
            // double y = (double) (j - top_y)/(bot_y - top_y);
            double x = (double) (i - left_x)/(right_x - left_x);
            double y = (double) (j - dest.y)/(dest.h);

            int posx = x*img->w;
            int posy = y*img->h;

            Uint32 color = getpixel(img, posx, posy);
            putpixel(screen, i, j, color);
        }
        
        left_x -= ratio;
        right_x += ratio;
    }
}