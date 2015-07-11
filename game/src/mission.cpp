/*
 * Mission class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "mission.h"
#include <core/settings.h>
#include <core/environment.h>

using std::to_string;
using std::shared_ptr;

Mission::Mission(const string& name, unsigned long remainder, const string& icon,
    int energy, int matter, const string& file, int m, int p, int t, vector<string> c, int slot)
    : m_name(name), m_remainder(remainder), m_icon(icon), m_energy(energy),
        m_matter(matter), m_file(file), m_m(m), m_p(p), m_t(t), m_c(c), m_slot(slot)
{
}

string
Mission::name()
{
    return m_name;
}

unsigned long
Mission::remainder()
{
    return m_remainder;
}

void
Mission::set_remainder(unsigned long remainder)
{
    m_remainder = remainder;
}

string
Mission::icon()
{
    return m_icon;
}

int
Mission::energy()
{
    return m_energy;
}

int
Mission::matter()
{
    return m_matter;
}

int
Mission::m()
{
    return m_m;
}

int
Mission::p()
{
    return m_p;
}

int
Mission::t()
{
    return m_t;
}

string
Mission::file()
{
    return m_file;
}

void
Mission::available_character(bool available)
{
    Environment *env = Environment::get_instance();
    
    string path = "res/datas/slot" + to_string(m_slot) + "/characters.sav";
    shared_ptr<Settings> characters = env->resources_manager->get_settings(path);
    for (auto c : m_c)
    {
        for (auto s : characters->sections())
        {
            if (c == s.first)
            {
                characters->write<bool>(c, "available", available);
            }
        }
    }
    characters->save(path);
}
