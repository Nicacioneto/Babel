#ifndef ROOM_H
#define ROOM_H

class Room
{
public:
    Room(int north = 0, int east = 0, int south = 0, int west = 0);

    int tile(int direction) const;
    void set_tile(int direction, int tile);

private:
    int m_tiles[4];
};

#endif
