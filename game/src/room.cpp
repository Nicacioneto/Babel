#include "room.h"

Room::Room(int north, int east, int south, int west)
    : m_tiles { north, east, south, west }
{
}

int
Room::tile(int direction) const
{
    return m_tiles[direction];
}

void
Room::set_tile(int direction, int tile)
{
    m_tiles[direction] = tile;
}
