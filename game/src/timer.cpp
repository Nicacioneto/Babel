/*
 * Timer class implementation.
 *
 * Author: Tiamat
 * Date: 14/05/2015
 * License: LGPL. No copyright.
 */
#include "timer.h"
#include "mission.h"

using std::vector;

vector<Mission*> m_missions;


int monitoring_mission(void *ptr)
{
	Mission *mission = (Mission *) ptr;
    unsigned long remainder = mission->remainder();

    while(remainder > 0 )
    {
    	SDL_Delay(1000);
    	--remainder;
    	mission->set_remainder(remainder);
    }


    return 0;
}

void start_time(const string& name, unsigned long start, const string& icon) throw (Exception)
{
	Mission *mission = new Mission(name, start, icon);

	m_missions.push_back(mission);
    
    SDL_CreateThread(monitoring_mission, "monitoring mission", m_missions[m_missions.size() - 1]);
}

vector<Mission*> missions()
{
	return m_missions;
}