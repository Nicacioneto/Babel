#ifndef DUNGEON_H
#define DUNGEON_H

#include "room.h"
#include "direction.h"
#include <core/level.h>

#include <list>

using std::list;

class Texture;

#define MAXW 10
#define MAXH 10
#define MAXT 10

class Dungeon : public Level
{
public:
    Dungeon(int x = 0, int y = 0, int w = MAXW, int h = MAXH, int steps = 0,
        Direction direction = Direction());

    void move_backward();
    void move_forward();

    void turn_left();
    void turn_right();

private:
    void draw_self();
    void update_self(unsigned long elapsed);

    int m_x, m_y, m_w, m_h, m_steps;
    Direction m_direction;
    Room m_rooms[MAXW][MAXH];
    shared_ptr<Texture> m_tiles[MAXT + 1];

    list<Rect> planes(int w, int h);
};

#endif
