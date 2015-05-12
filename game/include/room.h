#ifndef ROOM_H
#define ROOM_H

class Room
{
public:
    Room(int north = 0, int east = 0, int south = 0, int west = 0)
        : m_tiles { north, east, south, west }
    {
    }

    int tile(int direction) const
    {
        return m_tiles[direction];
    }

    void set_tile(int direction, int tile)
    {
        m_tiles[direction] = tile;
    }

private:
    int m_tiles[4];
};

#endif
