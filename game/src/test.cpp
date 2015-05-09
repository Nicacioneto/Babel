#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>

#define VELOCITY 10
#define NONE 0
#define HALL 1
#define DOOR 2

typedef struct _Position
{
    int front = 0;
    int left = 0;
    int right = 0;
} Position;

Uint32 getpixel(SDL_Surface *surface, int x, int y);

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void map_center(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest);

void map_left(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio);

void map_right(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio);

void map_up(SDL_Surface *screen, SDL_Surface *img, const SDL_Rect& dest,
    double ratio);

void draw(SDL_Surface *screen, std::vector<Position> map, int step);

SDL_Surface *door;
SDL_Surface *hall;
SDL_Surface *ceiling;

int main()
{
    std::vector<std::vector<Position>> map;
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, 0);

    SDL_Surface *img = SDL_LoadBMP("res/images/door.bmp");
    door = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);
    
    img = SDL_LoadBMP("res/images/hall.bmp");
    hall = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);

    img = SDL_LoadBMP("res/images/ceiling.bmp");
    ceiling = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);

    std::vector<Position> temp;
    Position pos;
    pos.front = DOOR;
    pos.left = HALL;
    pos.right = HALL;
    temp.push_back(pos);

    pos.front = NONE;
    pos.left = DOOR;
    pos.right = HALL;
    temp.push_back(pos);
    
    pos.front = NONE;
    pos.left = HALL;
    pos.right = DOOR;
    temp.push_back(pos);
    map.push_back(temp);


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

    int step = 0;

    draw(screen, map[0], step);

    SDL_Flip(screen);

    SDL_Event event;
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
                    step++;
                    draw(screen, map[0], step);
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

void draw(SDL_Surface *screen, std::vector<Position> map, int step)
{
    SDL_Rect center;
    SDL_Rect up;
    SDL_Rect left;
    SDL_Rect right;

    up.x = 220 - VELOCITY*step;
    up.y = 175 - VELOCITY*step*0.5;
    up.w = 360 + VELOCITY*step*2;
    up.h = 55;
    map_up(screen, ceiling, up, 2);

    up.x = 110 - VELOCITY*step;
    up.y = 120 - VELOCITY*step*0.5;
    up.w = 580 + VELOCITY*step*2;
    up.h = 55;
    map_up(screen, ceiling, up, 2);
    
    up.x = 0 - VELOCITY*step;
    up.y = 65 - VELOCITY*step*0.5;
    up.w = 800 + VELOCITY*step*2;
    map_up(screen, ceiling, up, 2);
    up.h = 55;

    if (map[0].front)
    {
        center.x = 330 - VELOCITY*step;
        center.y =  230 - VELOCITY*step*0.5;
        center.w =  140 + VELOCITY*step*2;
        center.h =  140 + VELOCITY*step;
        if (map[0].front == HALL)
            map_center(screen, hall, center);
        else if (map[0].front == DOOR)
            map_center(screen, door, center);
    }

    left.x = 330 - VELOCITY*step;
    left.y = 230 - VELOCITY*step*0.5;
    left.w = 110;
    left.h = 140 + VELOCITY*step;
    if (map[0].left == HALL)
        map_left(screen, hall, left, 0.5);
    else if (map[0].left == DOOR)
        map_left(screen, door, left, 0.5);

    left.x = 220 - VELOCITY*step;
    left.y = 175 - VELOCITY*step*0.5;
    left.w = 110;
    left.h = 250 + VELOCITY*step;
    if (map[1].left == HALL)
        map_left(screen, hall, left, 0.5);
    else if (map[1].left == DOOR)
        map_left(screen, door, left, 0.5);

    left.x = 110 - VELOCITY*step;
    left.y = 120 - VELOCITY*step*0.5;
    left.w = 110;
    left.h = 360 + VELOCITY*step;
    if (map[2].left == HALL)
        map_left(screen, hall, left, 0.5);
    else if (map[2].left == DOOR)
        map_left(screen, door, left, 0.5);

    right.x = 470 + VELOCITY*step;
    right.y = 230 - VELOCITY*step*0.5;
    right.w = 110;
    right.h = 140 + VELOCITY*step;
    if (map[0].right == HALL)
        map_right(screen, hall, right, 0.5);
    if (map[0].right == DOOR)
        map_right(screen, door, right, 0.5);

    right.x = 580 + VELOCITY*step;
    right.y = 175 - VELOCITY*step*0.5;
    right.w = 110;
    right.h = 250 + VELOCITY*step;
    if (map[1].right == HALL)
        map_right(screen, hall, right, 0.5);
    if (map[1].right == DOOR)
        map_right(screen, door, right, 0.5);

    right.x = 690 + VELOCITY*step;
    right.y = 120 - VELOCITY*step*0.5;
    right.w = 110;
    right.h = 360 + VELOCITY*step;
    if (map[2].right == HALL)
        map_right(screen, hall, right, 0.5);
    if (map[2].right == DOOR)
        map_right(screen, door, right, 0.5);
}