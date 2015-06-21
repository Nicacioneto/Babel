/*
 * Class that represents the Room
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef ROOM_H
#define ROOM_H

class Room
{
public:
    Room(int north = 0, int east = 0, int south = 0,
        int west = 0, int roof = 0, int floor = 0);

    int tile(int direction) const;
    void set_tile(int direction, int tile);

private:
    int m_tiles[6];
};

#endif
