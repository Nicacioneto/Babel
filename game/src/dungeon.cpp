#include "dungeon.h"
#include "file.h"
#include "mapping.h"
#include "options.h"
#include <core/bitmap.h>
#include <core/keyboardevent.h>
#include <core/rect.h>
#include <sstream>
#include <vector>

using std::endl;
using std::stringstream;
using std::vector;

Dungeon::Dungeon(int x, int y, int w, int h, int steps, Direction direction)
    : Level("", ""), m_x(x), m_y(y), m_w(w), m_h(h), m_steps(steps), m_direction(direction),
      m_state(WAITING), m_delta(0), m_last(0)
{
    Environment *env = Environment::get_instance();

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
                    move_forward();
                    return true;
                case KeyboardEvent::DOWN:
                    move_backward();
                    return true;
                case KeyboardEvent::RIGHT:
                    turn_right();
                    return true;
                case KeyboardEvent::LEFT:
                    turn_left();
                    return true;
                case KeyboardEvent::ESCAPE:
                    set_next("base");
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
        // env->canvas->draw(f, Color::WHITE);
        // env->canvas->draw(b, Color::YELLOW);

        if (west_tile)
        {
            mapping.draw_walls(m_screen, m_tiles[west_tile].get(), f, b);
        }
        
        f.set_x(f.x() + front.w());
        b.set_x(b.x() + back.w());
        // env->canvas->draw(f, Color::WHITE);
        // env->canvas->draw(b, Color::YELLOW);


        int east_tile = m_rooms[idx][idy].tile(m_direction.next());

        if (east_tile)
        {
            mapping.draw_walls(m_screen, m_tiles[east_tile].get(), f, b);
        }

        f = Rect(front.x(), front.y(), front.w(), 0);
        b = Rect(back.x(), back.y(), back.w(), 0);
        // env->canvas->draw(f, Color::WHITE);
        // env->canvas->draw(b, Color::YELLOW);


        int roof_tile = m_rooms[idx][idy].tile(m_direction.roof());

        if (roof_tile)
        {
            mapping.draw_ceiling_floor(m_screen, m_tiles[roof_tile].get(), f, b);
        }

        f.set_y(f.y() + front.h());
        b.set_y(b.y() + back.h());
        // env->canvas->draw(f, Color::WHITE);
        // env->canvas->draw(b, Color::YELLOW);

        int floor_tile = m_rooms[idx][idy].tile(m_direction.floor());

        if (floor_tile)
        {
            mapping.draw_ceiling_floor(m_screen, m_tiles[floor_tile].get(), f, b);
        }
        
        int north_tile = m_rooms[idx][idy].tile(m_direction.front());

        if (north_tile)
        {
            // env->canvas->fill(back, Color::RED);
            // env->canvas->update();
            // SDL_Delay(1000);
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
    string file = read_file("res/maps/map.txt");

    stringstream ss;
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
        string img = "res/images/dungeon/" + std::to_string(i);
        try
        {
            try
            {
                m_tiles[i] = env->resources_manager->get_bitmap(img + ".png");
            }
            catch (Exception)
            {
                m_tiles[i] = env->resources_manager->get_bitmap(img + ".bmp");
            }
        }
        catch (Exception) {}
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

    int next = (m_steps + 4 + m_delta) % 4;

    if (next)
    {
        m_steps = next;
    }
    else
    {
        int tile;

        if (m_delta > 0)
        {
            tile = m_rooms[m_x][m_y].tile(m_direction.front());
        }

        if (tile == 0)
        {
            pair<int, int> v = m_direction.vector();
            m_x += v.first;
            m_y += v.second;
            m_steps = 0;
        }

        m_state = WAITING;
    }

}
