/*
 * Timer manager class
 *
 * Author: Tiamat
 * Date: 14/05/2015
 * License: LGPL. No copyright.
 */
#ifndef TIMER_H
#define TIMER_H

#include <core/exception.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <vector>
#include "mission.h"

using std::vector;

extern vector<Mission *> missions();
extern void create_threads(int slot);
extern void kill_threads();

class Mission;

extern void start_time(Mission *mission) throw (Exception);

#endif
