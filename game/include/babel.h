/*
 * Class that represents the Babel game
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef BABEL_H
#define BABEL_H

#include <core/game.h>

class Babel : public Game
{
public:
    Babel();
    ~Babel();

private:
    int m_slot, m_frontend;

    bool on_event(const KeyboardEvent& event);
    bool on_event(const SystemEvent& event);

    Level * load_level(const string& id);
    Level * load_frontend(const string& id);
};

#endif
