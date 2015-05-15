#include "dungeon.h"
#include "file.h"
#include "mapping.h"
#include <core/rect.h>
#include <vector>

using std::vector;

Dungeon::Dungeon(int x, int y, int w, int h, int steps, Direction direction)
    : Level("", ""), m_x(x), m_y(y), m_w(w), m_h(h), m_steps(steps), m_direction(direction)
{
    load_map();
    // m_rooms[0][0].set_tile(Direction::EAST, 1);
    // m_rooms[0][0].set_tile(Direction::SOUTH, 2);
    // m_rooms[0][0].set_tile(Direction::WEST, 1);

    // m_rooms[1][0].set_tile(Direction::EAST, 1);
    // m_rooms[1][0].set_tile(Direction::WEST, 1);

    // m_rooms[2][0].set_tile(Direction::NORTH, 1);
    // m_rooms[2][0].set_tile(Direction::WEST, 1);

    // m_rooms[2][1].set_tile(Direction::NORTH, 1);
    // m_rooms[2][1].set_tile(Direction::SOUTH, 1);

    // m_rooms[2][2].set_tile(Direction::NORTH, 1);
    // m_rooms[2][2].set_tile(Direction::EAST, 1);

    // m_rooms[1][2].set_tile(Direction::EAST, 1);
    // m_rooms[1][2].set_tile(Direction::SOUTH, 1);

    // m_rooms[1][1].set_tile(Direction::NORTH, 1);
    // m_rooms[1][1].set_tile(Direction::WEST, 1);

    // m_rooms[0][1].set_tile(Direction::WEST, 1);
    // m_rooms[0][1].set_tile(Direction::SOUTH, 1);

    // m_rooms[0][2].set_tile(Direction::NORTH, 1);
    // m_rooms[0][2].set_tile(Direction::EAST, 2);
    // m_rooms[0][2].set_tile(Direction::SOUTH, 1);

    env = Environment::get_instance();
    m_tiles[1] = env->resources_manager->get_texture("res/images/hall.bmp");
    m_tiles[2] = env->resources_manager->get_texture("res/images/door.bmp");
}

void
Dungeon::update_self(unsigned long)
{
}

void
Dungeon::draw_self()
{
    env->canvas->clear();

    list<Rect> ps = planes(env->canvas->w(), env->canvas->h());
    Rect front = ps.front();
    ps.pop_front();

    Mapping mapping;

    int idx = m_x, idy = m_y;

    while (not ps.empty())
    {
        Rect back = ps.front();
        ps.pop_front();

        Rect f { front.x(), front.y(), 0, front.h() };
        Rect b { back.x(), back.y(), 0, back.h() };

// printf("\nNew frame\n");
// printf("front = (%d, %d), %dx%d\n", front.x(), front.y(), front.w(), front.h());
// printf("back = (%d, %d), %dx%d\n", back.x(), back.y(), back.w(), back.h());
// printf("f = (%d, %d), %dx%d\n", f.x(), f.y(), f.w(), f.h());
// printf("b = (%d, %d), %dx%d\n", b.x(), b.y(), b.w(), b.h());

        int east_tile = m_rooms[idx][idy].tile(m_direction.prev());
// printf("east tile = %d\n", east_tile);

        if (east_tile)
        {
            mapping.draw(m_tiles[east_tile].get(), f, b);
        }
        
        f.set_x(f.x() + front.w());
        b.set_x(b.x() + back.w());

// printf("f = (%d, %d), %dx%d\n", f.x(), f.y(), f.w(), f.h());
// printf("b = (%d, %d), %dx%d\n", b.x(), b.y(), b.w(), b.h());

        int west_tile = m_rooms[idx][idy].tile(m_direction.next());
// printf("west tile = %d\n", west_tile);

        if (west_tile)
        {
            mapping.draw(m_tiles[west_tile].get(), f, b);
        }


        int north_tile = m_rooms[idx][idy].tile(m_direction.front());

// printf("north tile = %d\n", north_tile);
        if (north_tile)
        {
// printf("back = (%d, %d), %dx%d\n", back.x(), back.y(), back.w(), back.h());
            mapping.draw(m_tiles[north_tile].get(), back);
            break;
        }
        else
        {
            pair<int, int> v = m_direction.vector();
// printf("vx = %d, vy = %d\n", v.first, v.second);
            int newx = idx + v.first;
            int newy = idy + v.second;
// printf("newx = %d, newy = %d\n", newx, newy);
            if (newx >= 0 and newx < m_w and newy >= 0 and newy < m_h)
            {
                idx = newx;
                idy = newy;
                front = back;
// printf("idx = %d, idy = %d\n", idx, idy);
            }
            else
            {
                break;
            }
        }
    }
}

list<Rect>
Dungeon::planes(int sw, int sh)
{
    static constexpr unsigned short center_size = 200;
    short centerx = (sw - center_size)/2;
    short centery = (sh - center_size)/2;

    Rect center { (double)centerx, (double)centery, (double)center_size, (double)center_size };
// printf("center = (%d, %d), %dx%d\n", center.x(), center.y(), center.w(), center.h());

    static constexpr double ratio = 0.6;
    short dx = centerx / 3;
    short dy = dx * ratio;

// printf("dx = %d, dy = %d\n", dx, dy);

    list<Rect> ps;

    short x = center.x() - (m_steps ? (m_steps*dx)/4 : dx);
    short y = center.y() - (m_steps ? (m_steps*dy)/4 : dy);
    unsigned short w = center.w() + 2*(center.x() - x);
    unsigned short h = center.h() + 2*(center.y() - y);

    x += dx;
    y += dy;
    w -= 2*dx;
    h -= 2*dy;

    Rect plane { (double)x, (double)y, (double)w, (double)h };
    ps.push_front(plane);

    do
    {
        x -= dx;
        y -= dy;
        w += 2*dx;
        h += 2*dy;

        Rect next { (double)x, (double)y, (double)w, (double)h };
        ps.push_front(next);
    } while (x > 0);

    // for (auto r : ps)
    // {
    //     // printf("plane = (%d, %d), %dx%d\n", r.x(), r.y(), r.w(), r.h());
    // }

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

void
Dungeon::load_map()
{
    string s = read_file("map.txt");

    vector<vector<int>> map;

    vector<int> p;
    for (unsigned int i = 0; i < s.size(); ++i)
    {
        char c = s[i];

        if (c == ' ')
        {
            continue;
        }

        if (c == '\n')
        {
            map.push_back(p);
            p.clear();
            continue;
        }

        p.push_back(c - '0');
    }

    for (unsigned int u = 0, i = map.size()-1; u < map.size(); ++u, --i)
    {
        for (unsigned int v = 0, j = 0; v < map[i].size(); ++v)
        {
            switch (v % 4)
            {
                case 0:
                    m_rooms[i][j].set_tile(Direction::NORTH, map[u][v]);
                    break;
                case 1:
                    m_rooms[i][j].set_tile(Direction::EAST, map[u][v]);
                    break;
                case 2:
                    m_rooms[i][j].set_tile(Direction::SOUTH, map[u][v]);
                    break;
                case 3:
                    m_rooms[i][j].set_tile(Direction::WEST, map[u][v]);
                    j++;
                    break;
            }
        }
    }
}
