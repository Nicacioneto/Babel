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

    Direction(int front = NORTH) : m_front(front) {}

    int front() const { return m_front; }
    int back() const { return (m_front + 2) % 4; }
    int next() const { return (m_front + 1) % 4; }
    int prev() const { return (m_front + 3) % 4; }

    void turn_left() { m_front = (m_front + 3) % 4; }
    void turn_right() { m_front = (m_front + 1) % 4; }

    pair<int, int> vector() { return Direction::m_vectors[m_front]; }

private:
    int m_front;
    static const pair<int, int> m_vectors[];
};

#endif
