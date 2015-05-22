#ifndef DIRECTION_H
#define DIRECTION_H

#include <iostream>

using std::pair;
using std::make_pair;

class Direction
{
public:
    static constexpr int NORTH = 0;
    static constexpr int EAST = 1;
    static constexpr int SOUTH = 2;
    static constexpr int WEST = 3;
    static constexpr int ROOF = 4;
    static constexpr int FLOOR = 5;
    
    Direction(int front = NORTH);

    int front() const;
    int back() const;
    int next() const;
    int prev() const;
    int roof() const;
    int floor() const;

    void turn_left();
    void turn_right();

    pair<int, int> vector();

private:
    int m_front;
};

#endif
