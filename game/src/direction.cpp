#include "direction.h"

static const pair<int, int> m_vectors[]
{ 
    make_pair(1, 0), 
    make_pair(0, 1), make_pair(-1, 0), make_pair(0, -1)
};

Direction::Direction(int front) 
    : m_front(front)
{
}

int
Direction::front() const
{
    return m_front;
}

int
Direction::back() const
{
    return (m_front + 2) % 4;
}

int
Direction::next() const
{
    return (m_front + 1) % 4;
}

int
Direction::prev() const
{
    return (m_front + 3) % 4;
}

void
Direction::turn_left()
{
    m_front = (m_front + 3) % 4;
}

void
Direction::turn_right()
{
    m_front = (m_front + 1) % 4;
}


pair<int, int>
Direction::vector()
{
    return m_vectors[m_front];
}

// const pair<int, int> Direction::m_vectors[]
// {
//     make_pair(1, 0), 
//     make_pair(0, 1), make_pair(-1, 0), make_pair(0, -1)
// };
