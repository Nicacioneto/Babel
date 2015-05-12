#include <SDL/SDL.h>

#include "dungeon.h"
#include "mapping.h"

Dungeon::Dungeon(int w, int h, int x, int y, int steps, Direction direction)
    : m_w(w), m_h(h), m_x(x), m_y(y), m_steps(steps), m_direction(direction)
{
//    m_rooms[1][0].set_tile(Direction::NORTH, 2);

//    m_rooms[0][0].set_tile(Direction::NORTH, 1);
    m_rooms[0][0].set_tile(Direction::EAST, 1);
    m_rooms[0][0].set_tile(Direction::SOUTH, 2);
    m_rooms[0][0].set_tile(Direction::WEST, 1);

    m_rooms[1][0].set_tile(Direction::EAST, 1);
    m_rooms[1][0].set_tile(Direction::WEST, 1);

    m_rooms[2][0].set_tile(Direction::NORTH, 1);
    m_rooms[2][0].set_tile(Direction::WEST, 1);

    m_rooms[2][1].set_tile(Direction::NORTH, 1);
    m_rooms[2][1].set_tile(Direction::SOUTH, 1);

    m_rooms[2][2].set_tile(Direction::NORTH, 1);
    m_rooms[2][2].set_tile(Direction::EAST, 1);

    m_rooms[1][2].set_tile(Direction::EAST, 1);
    m_rooms[1][2].set_tile(Direction::SOUTH, 1);

    m_rooms[1][1].set_tile(Direction::NORTH, 1);
    m_rooms[1][1].set_tile(Direction::WEST, 1);

    m_rooms[0][1].set_tile(Direction::WEST, 1);
    m_rooms[0][1].set_tile(Direction::SOUTH, 1);

    m_rooms[0][2].set_tile(Direction::NORTH, 1);
    m_rooms[0][2].set_tile(Direction::EAST, 2);
    m_rooms[0][2].set_tile(Direction::SOUTH, 1);

    SDL_Surface *img = SDL_LoadBMP("res/images/door.bmp");
    m_tiles[2] = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);

    img = SDL_LoadBMP("res/images/hall.bmp");
    m_tiles[1] = SDL_DisplayFormat(img);
    SDL_FreeSurface(img);
}

void
Dungeon::draw(SDL_Surface *screen)
{
    SDL_FillRect(screen, 0, 0);

    list<SDL_Rect> ps = planes(screen->w, screen->h);
    SDL_Rect front = ps.front();
    ps.pop_front();

    Mapping mapping(screen);

    int idx = m_x, idy = m_y;

    while (not ps.empty())
    {
        SDL_Rect back = ps.front();
        ps.pop_front();

        SDL_Rect f { front.x, front.y, 0, front.h };
        SDL_Rect b { back.x, back.y, 0, back.h };

printf("\nNew frame\n");
printf("front = (%d, %d), %dx%d\n", front.x, front.y, front.w, front.h);
printf("back = (%d, %d), %dx%d\n", back.x, back.y, back.w, back.h);
printf("f = (%d, %d), %dx%d\n", f.x, f.y, f.w, f.h);
printf("b = (%d, %d), %dx%d\n", b.x, b.y, b.w, b.h);

        int east_tile = m_rooms[idx][idy].tile(m_direction.prev());
printf("east tile = %d\n", east_tile);

        if (east_tile)
        {
            mapping.draw(m_tiles[east_tile], f, b);
        } 
        
        f.x += front.w;
        b.x += back.w;

printf("f = (%d, %d), %dx%d\n", f.x, f.y, f.w, f.h);
printf("b = (%d, %d), %dx%d\n", b.x, b.y, b.w, b.h);

        int west_tile = m_rooms[idx][idy].tile(m_direction.next());
printf("west tile = %d\n", west_tile);

        if (west_tile)
        {
            mapping.draw(m_tiles[west_tile], f, b);
        }


        int north_tile = m_rooms[idx][idy].tile(m_direction.front());

printf("north tile = %d\n", north_tile);
        if (north_tile)
        {
printf("back = (%d, %d), %dx%d\n", back.x, back.y, back.w, back.h);
            mapping.draw(m_tiles[north_tile], back);
            break;
        } 
        else
        {
            pair<int, int> v = m_direction.vector();
printf("vx = %d, vy = %d\n", v.first, v.second);
            int newx = idx + v.first;
            int newy = idy + v.second;
printf("newx = %d, newy = %d\n", newx, newy);
            if (newx >= 0 and newx < m_w and newy >= 0 and newy < m_h)
            {
                idx = newx;
                idy = newy;
                front = back;
printf("idx = %d, idy = %d\n", idx, idy);
            } 
            else
            {
                break;
            }
        }
    }
/*

            int east_tile = m_rooms[idx][idy].tile((Direction) peast);
printf("east tite = %d\n", east_tile);
            if (east_tile)
            {
                mapping.draw(m_tiles[east_tile], f, b);
            }

            --idx;
        }

        return;
//        while (fx > 0)
//        {
//            map_title(screen, fx, fy, fh, bx, by, bh, 
        int center
        int cutx = centerx / 3;
        double ratio = 0.6;

printf("centery = %d\n", centery);
printf("cutx = %d\n", cutx);

        if (m_steps == 0)
        {
            // left
            if (east_tile)
            {
                int signal = -1;
                int x = cutx*2;
                int y = (centerx - x)*signal*ratio + centery;
                int bt = (centery + center_size) + (centery - y);
                int h = bt - y;
printf("x = %d\n", x);
printf("y = %d\n", y);
printf("bt = %d\n", bt);
printf("h = %d\n", h);
                maptile(screen, m_tiles[east_tile], x, y, x, h, signal, ratio,
                    m_tiles[east_tile]->w - 1, m_tiles[east_tile]->h - 1, 
                    m_tiles[east_tile]->w, m_tiles[east_tile]->h);
            }

//void maptile(SDL_Surface *screen, SDL_Surface *img, int posx, int posy,
 //   int w, int h, int signal, double ratio, int imgx, int imgy, int imgw, int imgh)
        } 
    }
*/
}
list<SDL_Rect>
Dungeon::planes(int sw, int sh)
{
    static constexpr unsigned short center_size = 200;
    short centerx = (sw - center_size)/2;
    short centery = (sh - center_size)/2;

    SDL_Rect center { centerx, centery, center_size, center_size };
printf("center = (%d, %d), %dx%d\n", center.x, center.y, center.w, center.h);

    static constexpr double ratio = 0.6;
    short dx = centerx / 3;
    short dy = dx * ratio;

printf("dx = %d, dy = %d\n", dx, dy);

    list<SDL_Rect> ps;

    short x = center.x - (m_steps ? (m_steps*dx)/4 : dx);
    short y = center.y - (m_steps ? (m_steps*dy)/4 : dy);
    unsigned short w = center.w + 2*(center.x - x);
    unsigned short h = center.h + 2*(center.y - y);

    x += dx;
    y += dy;
    w -= 2*dx;
    h -= 2*dy;

    SDL_Rect plane { x, y, w, h };
    ps.push_front(plane);

    do {
        x -= dx;
        y -= dy;
        w += 2*dx;
        h += 2*dy;

        SDL_Rect next { x, y, w, h };
        ps.push_front(next);
    } while (x > 0);

    for (auto r : ps)
    {
        printf("plane = (%d, %d), %dx%d\n", r.x, r.y, r.w, r.h);
    }

    return ps;
}

void
Dungeon::move_forward()
{
    int next = (m_steps + 1) % 4;

    if (next)
    {
        m_steps = next;
        
    } 
    else 
    {
        int tile = m_rooms[m_x][m_y].tile(m_direction.front());

        if (tile == 0)
        {
            pair<int, int> v = m_direction.vector();
            m_x += v.first;
            m_y += v.second;
            m_steps = 0;
        }
    }
}

void
Dungeon::move_backward()
{
    int next = m_steps - 1;

    if (next > -1)
    {
        m_steps = next;
    } 
    else 
    {
        int tile = m_rooms[m_x][m_y].tile(m_direction.back());

        if (tile == 0)
        {
            pair<int, int> v = m_direction.vector();
            m_x -= v.first;
            m_y -= v.second;
            m_steps = 3;
        }
    }
}

void
Dungeon::turn_left()
{
    m_direction.turn_left();
    m_steps = 2;
}

void
Dungeon::turn_right()
{
    m_direction.turn_right();
    m_steps = 2;
}
