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
#include <SDL_thread.h>
#include <SDL_timer.h>
#include <vector>
#include "mission.h"

using std::vector;


extern vector<Mission *> missions();
extern void create_threads(int slot);
extern void kill_threads();

extern void start_time(const string& name = "", unsigned long start = 0,
    const string& icon = "") throw (Exception);

#endif
