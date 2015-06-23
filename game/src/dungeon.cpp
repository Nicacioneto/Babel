/*
 * Dungeon class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "dungeon.h"
#include "file.h"
#include "mapping.h"
#include "options.h"
#include <core/bitmap.h>
#include <core/keyboardevent.h>
#include <core/rect.h>
#include <core/settings.h>
#include <ctime>
#include <sstream>
#include <vector>

#define PROBABILITY_STEPWISE 2;

using std::vector;
using std::to_string;

Dungeon::Dungeon(int slot, int steps, int probability_combat)
    : Level("dungeon", ""), m_slot(slot), m_steps(steps), m_delta(0),
        m_probability_combat(probability_combat), m_last(0), m_state(WAITING)
{
    Environment *env = Environment::get_instance();

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/dungeon.sav");

    m_x = settings->read<int>("Dungeon", "x", 0);
    m_y = settings->read<int>("Dungeon", "y", 0);

    int direction = settings->read<int>("Dungeon", "direction", 0);
    m_direction = Direction(direction);

    m_screen = new Bitmap(env->video->canvas());

    load_tiles();
    load_map();

    env->events_manager->register_listener(this);

    env->music->play("res/music/Pandora_s_Music_Box.ogg", -1);
}

Dungeon::~Dungeon()
{
    Environment *env = Environment::get_instance();

    env->events_manager->unregister_listener(this);
    env->music->stop();

    for (int i = 0; i < m_w; ++i)
    {
        delete [] m_rooms[i];
    }

    delete [] m_rooms;
}

bool
Dungeon::on_event(const KeyboardEvent& event)
{
    switch (event.state())
    {
        case KeyboardEvent::PRESSED:
            switch (event.key())
            {
                case KeyboardEvent::UP:
                case KeyboardEvent::W:
                    move_forward();
                    return true;
                case KeyboardEvent::DOWN:
                case KeyboardEvent::S:
                    move_backward();
                    return true;
                case KeyboardEvent::RIGHT:
                case KeyboardEvent::D:
                    turn_right();
                    return true;
                case KeyboardEvent::LEFT:
                case KeyboardEvent::A:
                    turn_left();
                    return true;
                case KeyboardEvent::ESCAPE:
                    set_next("tower");
                    finish();
                    return true;
                default:
                    return false;
            }

        default:
            return false;
    }

    return false;
}

void
Dungeon::steps_to_foward()
{
    int next = (m_steps + m_delta) % 4;

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
            calculate_probability_combat();
        }

        m_state = WAITING;
    }
}

void
Dungeon::steps_to_backward()
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
            calculate_probability_combat();
        }

        m_state = WAITING;
    }
}

void
Dungeon::update_self(unsigned long elapsed)
{
    if (m_state == WAITING)
    {
        return;
    }

    if (m_last == 0)
    {
        m_last = elapsed;
    }

    if (elapsed - m_last < 5)
    {
        return;
    }

    m_last = elapsed;

    if (m_delta == 1)
    {
        steps_to_foward();
    }
    else if (m_delta == -1)
    {
        steps_to_backward();
    }
}

void
Dungeon::draw_self()
{
    Environment *env = Environment::get_instance();
    m_screen->clear();

    double ratio;

    list<Rect> ps = planes(env->canvas->w(), env->canvas->h(), ratio);
    Rect front = ps.front();
    ps.pop_front();

    Mapping mapping(ratio, 1/ratio);

    int idx = m_x, idy = m_y;
    bool blocked = false;
    Rect back;
    Rect center_rect = center(env->canvas->w(), env->canvas->h());
    int levels = 0;

    while (not ps.empty())
    {
        ++levels;

        back = ps.front();
        ps.pop_front();

        Rect f { front.x(), front.y(), 0, front.h() };
        Rect b { back.x(), back.y(), 0, back.h() };

        int west_tile = m_rooms[idx][idy].tile(m_direction.prev());
        if (west_tile)
        {
            mapping.draw_walls(m_screen, m_tiles[west_tile].get(), f, b);
        }
        
        f.set_x(f.x() + front.w());
        b.set_x(b.x() + back.w());

        int east_tile = m_rooms[idx][idy].tile(m_direction.next());

        if (east_tile)
        {
            mapping.draw_walls(m_screen, m_tiles[east_tile].get(), f, b);
        }

        f = Rect(front.x(), front.y(), front.w(), 0);
        b = Rect(back.x(), back.y(), back.w(), 0);

        int roof_tile = m_rooms[idx][idy].tile(m_direction.roof());

        if (roof_tile)
        {
            mapping.draw_ceiling_floor(m_screen, m_tiles[roof_tile].get(), f, b);
        }

        f.set_y(f.y() + front.h());
        b.set_y(b.y() + back.h());
        int floor_tile = m_rooms[idx][idy].tile(m_direction.floor());

        if (floor_tile)
        {
            mapping.draw_ceiling_floor(m_screen, m_tiles[floor_tile].get(), f, b);
        }
        
        int north_tile = m_rooms[idx][idy].tile(m_direction.front());

        if (north_tile)
        {
            mapping.draw_center(m_screen, m_tiles[north_tile].get(), back);
            blocked = true;
            break;
        }
        else
        {
            pair<int, int> v = m_direction.vector();
            int newx = idx + v.first;
            int newy = idy + v.second;
            if (newx >= 0 and newx < m_w and newy >= 0 and newy < m_h)
            {
                idx = newx;
                idy = newy;
                front = back;
            }
            else
            {
                break;
            }
        }
    }

    if (not blocked or (blocked and levels > 3))
    {
        m_screen->fill(center_rect, 0);
    }

    env->canvas->draw(m_screen);

    auto texture = env->resources_manager->get_texture("res/images/dungeon/compass.png");

    int x = 140 * m_direction.front();
    env->canvas->draw(texture.get(), Rect(x, 0, 140, 140),
        (60/1024.0) * env->canvas->w(),
        (600/800.0) * env->canvas->h(),
        140, 140);
}

Rect
Dungeon::center(int sw, int sh) const
{
    unsigned short centerw = sw/4;
    unsigned short centerh = sh/4;

    short centerx = (sw - centerw)/2;
    short centery = (sh - centerh)/2;

    Rect center { (double)centerx, (double)centery, (double)centerw, (double)centerh };

    return center;
}

list<Rect>
Dungeon::planes(int sw, int sh, double& ratio)
{
    unsigned short centerw = sw/4;
    unsigned short centerh = sh/4;

    short centerx = (sw - centerw)/2;
    short centery = (sh - centerh)/2;

    Rect center { (double)centerx, (double)centery, (double)centerw, (double)centerh };

    ratio = center.y() / center.x();

    short dx = centerx / 3;
    short dy = dx * ratio;

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
    
    return ps;
}

void
Dungeon::move_forward()
{
    if (m_state == WAITING)
    {
        m_state = MOVING;
        m_delta = 1;
        m_last = 10000;
    }
}

void
Dungeon::move_backward()
{
    if (m_state == WAITING)
    {
        m_state = MOVING;
        m_delta = -1;
        m_last = 10000;
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
    string file = read_file("res/maps/map.txt");

    std::stringstream ss;
    ss << file;

    vector<vector<int>> map;
    vector<int> p;

    int north, east, south, west, roof, floor;
    char garbage;

    while (ss >> north >> east >> south >> west >> roof >> floor >> garbage)
    {
        p.push_back(north);
        p.push_back(east);
        p.push_back(south);
        p.push_back(west);
        p.push_back(roof);
        p.push_back(floor);

        if (garbage == ';')
        {
            map.push_back(p);
            p.clear();
        }
    }

    m_w = map.size();
    m_h = map[0].size();

    m_rooms = new Room*[m_w];
    for (int i = 0; i < m_w; ++i)
    {
        m_rooms[i] = new Room[m_h];
    }

    for (unsigned int u = 0, i = map.size()-1; u < map.size(); ++u, --i)
    {
        for (unsigned int v = 0, j = 0; v < map[i].size(); ++v)
        {
            switch (v % 6)
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
                    break;
                case 4:
                    m_rooms[i][j].set_tile(Direction::ROOF, map[u][v]);
                    break;
                case 5:
                    m_rooms[i][j].set_tile(Direction::FLOOR, map[u][v]);
                    j++;
                    break;
            }
        }
    }
}

void
Dungeon::load_tiles()
{
    Environment *env = Environment::get_instance();
    
    for (int i = 0; i < MAXT; ++i)
    {
        string img = "res/images/dungeon/" + to_string(i);
        try
        {
            m_tiles[i] = env->resources_manager->get_bitmap(img + ".png");
        }
        catch (Exception) {}
    }
}

void
Dungeon::calculate_probability_combat()
{
    srand (time(NULL));

    int random = rand() % 100;

    if (random < m_probability_combat)
    {
        Environment *env = Environment::get_instance();
        string path = "res/datas/slot" + to_string(m_slot) + "/dungeon.sav";
        
        shared_ptr<Settings> settings = env->resources_manager->get_settings(path);
        settings->write<int>("Dungeon", "x", m_x);
        settings->write<int>("Dungeon", "y", m_y);
        settings->write<int>("Dungeon", "direction", m_direction.front());

        settings->save("res/datas/slot" + to_string(m_slot) + "/dungeon.sav");
        set_next("combat");
        finish();
    }
    else
    {
        m_probability_combat += PROBABILITY_STEPWISE;
    }
}
