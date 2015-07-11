/*
 * Mission class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "mission.h"

Mission::Mission(const string& name, unsigned long remainder, const string& icon,
    int energy, int matter, const string& file, int m, int p, int t, vector<string> c)
    : m_name(name), m_remainder(remainder), m_icon(icon), m_energy(energy),
        m_matter(matter), m_file(file), m_m(m), m_p(p), m_t(t), m_c(c)
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
Mission::available_character()
{
    // TODO
    /*
    for (auto it : m_c)
    {
        it->set_available(true);
    }*/
}