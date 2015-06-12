#ifndef DUNGEON_H
#define DUNGEON_H

#include "direction.h"
#include "room.h"
#include <core/level.h>
#include <core/listener.h>
#include <list>

using std::list;

class Bitmap;
class KeyboardEvent;
class Texture;

#define MAXT 10

class Dungeon : public Level, Listener
{
public:
    Dungeon(int x = 0, int y = 0, int w = 1, int h = 1, int steps = 0,
        Direction direction = Direction());
    ~Dungeon();

    void move_backward();
    void move_forward();

    void turn_left();
    void turn_right();

    bool on_event(const KeyboardEvent& event);

    typedef enum { MOVING, WAITING } State;

private:
    void steps_to_foward();
    void steps_to_backward();

    void update_self(unsigned long);
    void draw_self();

    void load_map();
    void load_tiles();

    int m_x, m_y, m_w, m_h, m_steps;
    Direction m_direction;
    State m_state;
    int m_delta;
    unsigned long m_last;
    Room **m_rooms;

    shared_ptr<Bitmap> m_tiles[MAXT + 1];
    Bitmap *m_screen;
    
    list<Rect> planes(int w, int h, double& ratio);
    Rect center(int sw, int sh) const;

};

#endif
