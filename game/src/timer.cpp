/*
 * Timer class implementation.
 *
 * Author: Tiamat
 * Date: 14/05/2015
 * License: LGPL. No copyright.
 */
#include "timer.h"
#include "mission.h"
#include <core/environment.h>
#include <core/settings.h>

using std::vector;
using std::to_string;

vector<Mission *> m_missions;
vector<SDL_Thread *> threads;
bool kill = false;
int m_slot = 0;

void set_reward(Mission *mission)
{
    Environment *env = Environment::get_instance();

    string file = mission->file();
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/" + file + ".sav");

    if (file == "colony")
    {
        int energy = settings->read<int>("Colony", "energy", 0);
        int matter = settings->read<int>("Colony", "matter", 0);

        energy += mission->energy();
        matter += mission->matter();

        settings->write<int>("Colony", "energy", energy);
        settings->write<int>("Colony", "matter", matter);
    }
    else if (file == "items")
    {
        string name = mission->name();

        settings->write<int>(name, "research_energy", 0);
        settings->write<int>(name, "research_matter", 0);
        settings->write<string>(name, "time", "00:00");
    }
    settings->save("res/datas/slot" + to_string(m_slot) + "/" + file + ".sav");
}

int monitoring_mission(void *ptr)
{
    Mission *mission = (Mission *) ptr;
    unsigned long remainder = mission->remainder();

    while (remainder > 0 && m_slot)
    {
        if (kill)
        {
            return 0;
        }
        --remainder;
        mission->set_remainder(remainder);
        SDL_Delay(1000);
    }

    set_reward(mission);

    return 0;
}

void start_time(const string& name, unsigned long start, const string& icon,
    int energy, int matter, const string& file) throw (Exception)
{
    Mission *mission = new Mission(name, start, icon, energy, matter, file);
    m_missions.push_back(mission);
    
    SDL_Thread *thread = SDL_CreateThread(monitoring_mission, "monitoring mission",
        m_missions[m_missions.size() - 1]);
    threads.push_back(thread);
}

vector<Mission *> missions()
{
    return m_missions;
}

void create_threads(int slot)
{
    m_slot = slot;
    Environment *env = Environment::get_instance();
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/timers.sav");
    auto sections = settings->sections();

    for (auto section : sections)
    {
        string name = section.first;
        unsigned long remainder = atol(section.second["remainder"].c_str());
        string icon = section.second["icon"];
        int energy = atoi(section.second["energy"].c_str());
        int matter = atoi(section.second["matter"].c_str());
        string file = section.second["file"];
        start_time(name, remainder, icon, energy, matter, file);
    }
}

void kill_threads()
{
    kill = true;
    Environment *env = Environment::get_instance();
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/timers.sav");

    for (auto mission : m_missions)
    {
        string name = mission->name();

        settings->write<unsigned long>(name, "remainder", mission->remainder());
        settings->write<string>(name, "icon", mission->icon());
        settings->write<int>(name, "energy", mission->energy());
        settings->write<int>(name, "matter", mission->matter());
        settings->write<string>(name, "file", mission->file());

        settings->save("res/datas/slot" + to_string(m_slot) + "/timers.sav");
    }
    for (size_t i = 0; i < threads.size(); ++i)
    {
        SDL_WaitThread(threads[i], nullptr);
    }
}
