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

#define PROBABILITY_STEPWISE 2

using std::vector;
using std::to_string;

Dungeon::Dungeon(int slot, int steps, int probability_combat)
    : Level("dungeon", ""), m_slot(slot), m_steps(steps), m_delta(0),
        m_probability_combat(probability_combat), m_last(0), m_state(WAITING),
        m_levels(0), m_front_blocked(false), m_door(0), m_screen(nullptr)
{
    Environment *env = Environment::get_instance();

    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/tower.sav");

    m_actual_floor = m_settings->read<string>("Tower", "actual_floor", "1");

    m_x = m_settings->read<int>(m_actual_floor, "x", 0);
    m_y = m_settings->read<int>(m_actual_floor, "y", 0);

    int direction = m_settings->read<int>(m_actual_floor, "direction", 0);
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
    if (m_state == DOOR)
    {
        return false;
    }

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
                case KeyboardEvent::P:
                    m_probability_combat=100;
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
Dungeon::pass_door()
{
    if (m_direction.front() == Direction::EAST)
    {
        m_y++;
    }
    else if (m_direction.front() == Direction::WEST)
    {
        m_y--;
    }
    else if (m_direction.front() == Direction::NORTH)
    {
        m_x++;
    }
    else
    {
        m_x--;
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

    if (m_state == MOVING)
    {
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
    else if (m_state == DOOR)
    {
        if (elapsed - m_last < 100)
        {
            return;
        }

        m_last = elapsed;
        ++m_door;

        if (m_door > 12)
        {
            m_state = WAITING;
            pass_door();
            m_door = 4;

            Environment *env = Environment::get_instance();
            env->music->resume();
        }
    }

    if (m_levels == 1 and m_delta == 1)
    {
        m_front_blocked = true;
    }

    if (m_x == m_in.first and m_y == m_in.second)
    {
        set_next("tower");
        set_visible(false);
        finish();
    }
    else if (m_x == m_out.first and m_y == m_out.second)
    {
        Environment *env = Environment::get_instance();
        string path = "res/datas/def/tower.sav";
        shared_ptr<Settings> settings = env->resources_manager->get_settings(path);

        m_x = settings->read<int>(m_actual_floor, "x", 0);
        m_y = settings->read<int>(m_actual_floor, "y", 0);
        int direction = settings->read<int>(m_actual_floor, "direction", 0);
        m_direction = Direction(direction);

        int unlocked_floors = m_settings->read<int>("Tower", "unlocked_floors", 1);
        if (atoi(m_actual_floor.c_str()) + 1 > unlocked_floors)
        {
            m_settings->write<int>("Tower", "unlocked_floors", unlocked_floors + 1);
            m_settings->save("res/datas/slot" + to_string(m_slot) + "/tower.sav");
        }

        save_position();
        set_next("tower");
        finish();
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
    m_levels = 0;
    int north_tile = 0, east_tile = 0, west_tile = 0, floor_tile = 0, roof_tile = 0;

    while (not ps.empty())
    {
        ++m_levels;

        back = ps.front();
        ps.pop_front();

        Rect f { front.x(), front.y(), 0, front.h() };
        Rect b { back.x(), back.y(), 0, back.h() };

        west_tile = m_rooms[idx][idy].tile(m_direction.prev());
        if (west_tile)
        {
            mapping.draw_walls(m_screen, m_tiles[west_tile].get(), f, b);
        }
        
        f.set_x(f.x() + front.w());
        b.set_x(b.x() + back.w());

        east_tile = m_rooms[idx][idy].tile(m_direction.next());

        if (east_tile)
        {
            mapping.draw_walls(m_screen, m_tiles[east_tile].get(), f, b);
        }

        f = Rect(front.x(), front.y(), front.w(), 0);
        b = Rect(back.x(), back.y(), back.w(), 0);

        roof_tile = m_rooms[idx][idy].tile(m_direction.roof());

        if (roof_tile)
        {
            mapping.draw_ceiling_floor(m_screen, m_tiles[roof_tile].get(), f, b);
        }

        f.set_y(f.y() + front.h());
        b.set_y(b.y() + back.h());
        floor_tile = m_rooms[idx][idy].tile(m_direction.floor());

        if (floor_tile)
        {
            mapping.draw_ceiling_floor(m_screen, m_tiles[floor_tile].get(), f, b);
        }
        
        north_tile = m_rooms[idx][idy].tile(m_direction.front());

        if (north_tile)
        {
            if (m_state == DOOR)
            {
                mapping.draw_center(m_screen, m_tiles[m_door].get(), back);
            }
            else
            {
                mapping.draw_center(m_screen, m_tiles[north_tile].get(), back);
            }
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

    if (not blocked or (blocked and m_levels > 3))
    {
        m_screen->fill(center_rect, 0);
    }

    if (m_front_blocked)
    {
        if (north_tile == 4) // when north is a door
        {
            m_state = DOOR;
            
            Environment *env = Environment::get_instance();
            env->music->pause();
            env->sfx->play("res/sfx/openning_door.ogg", 1);
        }
        
        m_front_blocked = false;
    }

    env->canvas->draw(m_screen);

    auto texture = env->resources_manager->get_texture("res/images/dungeon/compass.png");

    double scale_w = env->canvas->w() / 1024.0;
    double scale_h = env->canvas->h() / 768.0;
    int x = 416 * m_direction.front();
    env->canvas->draw(texture.get(), Rect(x, 0, 416, 58),
        (1024 - 416) / 2 * scale_w,
        600 * scale_h,
        416, 58);
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
    string f = m_settings->read<string>("Tower", "actual_floor", "1");
    string file = read_file("res/maps/map" + f + ".txt");

    std::stringstream ss;
    ss << file;

    vector<vector<int>> map;
    vector<int> p;

    int north, east, south, west, roof, floor;
    char garbage;

    ss >> m_in.first >> m_in.second >> m_out.first >> m_out.second;
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

    m_door = 4; //init door
}

void
Dungeon::calculate_probability_combat()
{
    srand (time(NULL));

    int random = rand() % 100;

    if (random < m_probability_combat)
    {
        save_position();
        set_next("combat");
        finish();
    }
    else
    {
        m_probability_combat += PROBABILITY_STEPWISE;
    }
}

void
Dungeon::save_position()
{
    m_settings->write<int>(m_actual_floor, "x", m_x);
    m_settings->write<int>(m_actual_floor, "y", m_y);
    m_settings->write<int>(m_actual_floor, "direction", m_direction.front());

    m_settings->save("res/datas/slot" + to_string(m_slot) + "/tower.sav");
}
