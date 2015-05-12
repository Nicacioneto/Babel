#include <SDL/SDL.h>

#include "dungeon.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, 0);
    SDL_Event event;

    Dungeon dungeon(3, 3, 0, 0);
    dungeon.draw(screen);
    SDL_Flip(screen);

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
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 0;
                }

                if (event.key.keysym.sym == SDLK_UP)
                {
                    dungeon.move_forward();
                    dungeon.draw(screen);
                    SDL_Flip(screen);
                }

                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    dungeon.move_backward();
                    dungeon.draw(screen);
                    SDL_Flip(screen);
                }

                if (event.key.keysym.sym == SDLK_RIGHT) {
                    dungeon.turn_right();
                    dungeon.draw(screen);
                    SDL_Flip(screen);
                }
 
                if (event.key.keysym.sym == SDLK_LEFT) {
                    dungeon.turn_left();
                    dungeon.draw(screen);
                    SDL_Flip(screen);
                }
            }
        }

        SDL_Delay(10);
    }
 
    return 0;
}

/*
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <cmath>

using namespace std;

#define VELOCITY 10
#define NONE 0
#define HALL 1
#define DOOR 2

void maptile(SDL_Surface *screen, SDL_Surface *img, int posx, int posy, int w, 
    int h, int signal, double ratio, int imgx, int imgy, int imgw, int imgh);

typedef enum { NORTH, EAST, SOUTH, WEST } Direction;

typedef struct _Position
{
    int front = 0;
    int left = 0;
    int right = 0;
} Position;


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

    std::vector< std::vector<Position> > map;

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


     3 parades da esquerda com o tamanho diferente 
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

    Dungeon dungeon(2, 2, 1, 0);;
    dungeon.draw(screen);
    SDL_Flip(screen);

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
                    dungeon.move_forward();
                    dungeon.draw(screen);
                    SDL_Flip(screen);
                } else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    dungeon.move_backward();
                    dungeon.draw(screen);
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

void maptile(SDL_Surface *screen, SDL_Surface *img, int posx, int posy,
    int w, int h, int signal, double ratio, int imgx, int imgy, int imgw, int imgh)
{
    double top_y = posy;
    double bot_y = posy + h - 1;

    int finalx = posx + signal*w;

    for (int i = posx; i != finalx; i += signal)
    {
        for (int j = top_y; j <= bot_y; ++j)
        {
            double x = (double) abs(posx - i)/w;
            double y = (double) (j - top_y)/(bot_y - top_y);

            if (signal < 0)
            {
                x = 1.0 - x;
            }

            int px = x*imgw + imgx;
            int py = y*imgh + imgy;

            Uint32 color = getpixel(img, px, posy);
            putpixel(screen, i, j, color);
        }
        
        top_y -= ratio;
        bot_y += ratio;
    }
}
*/
