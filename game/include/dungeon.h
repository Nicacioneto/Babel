#ifndef DUNGEON_H
#define DUNGEON_H

#include "room.h"
#include "direction.h"

#include <list>

using std::list;

#define MAXW 10
#define MAXH 10
#define MAXT 10

class Dungeon
{
public:
    Dungeon(int w, int h, int x, int y, int steps = 0, Direction direction =
        Direction());

    void draw(SDL_Surface *screen);

    void move_backward();
    void move_forward();

    void turn_left();
    void turn_right();

private:
    int m_w, m_h, m_x, m_y, m_steps;
    Direction m_direction;
    Room m_rooms[MAXW][MAXH];
    SDL_Surface* m_tiles[MAXT + 1];

    list<SDL_Rect> planes(int w, int h);
};

#endif
