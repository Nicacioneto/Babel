#include <SDL/SDL.h>

#include <stdlib.h>

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

    SDL_Rect center { 250, 150, 300, 300 }; 
    map_center(screen, door, center);

    SDL_Rect left { 250, 150, 250, 300 };
    map_left(screen, door, left, 0.6);

    SDL_Rect right { 550, 150, 250, 300 };
    map_right(screen, door, right, 0.6);

    SDL_Rect up { 0, 0, 800, 150 };
    map_up(screen, hall, up, 1.7);

/*    SDL_Rect center { 300, 200, 200, 200 }; 
    map_center(screen, door, center);

    SDL_Rect left { 300, 200, 150, 200 };
    map_left(screen, hall, left, 0.5);

    left.x = 150;
    left.y = 125;
    left.h = 350;
    map_left(screen, door, left, 0.5);

    SDL_Rect right { 500, 200, 150, 200 };
    map_right(screen, hall, right, 0.5);

    right.x = 650;
    right.y = 125;
    right.h = 350;
    map_right(screen, hall, right, 0.5); */


    SDL_Flip(screen);

    SDL_FreeSurface(door);
    SDL_FreeSurface(hall);

    SDL_Event event;

    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT or event.type == SDL_KEYDOWN)
            {
                return 0;
            }

            SDL_Delay(1);
        }
    }

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
    for (int i = dest.x; i < dest.x + dest.w; i++)
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

    for (int i = dest.x; i > dest.x - dest.w; --i)
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

    for (int i = dest.x; i < dest.x + dest.w; ++i)
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

    for (int j = dest.y; j <= dest.y + dest.h; ++j)
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
        
        left_x += ratio;
        right_x -= ratio;
    }
}