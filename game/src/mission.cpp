/*
 * Mission class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "mission.h"

Mission::Mission(const string& name, unsigned long remainder, const string& icon, int energy, int matter)
    : m_name(name), m_remainder(remainder), m_icon(icon), m_energy(energy), m_matter(matter)
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
