/*
 * Class that represents the Mission
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef MISSION_H
#define MISSION_H

#include <string>

using std::string;

class Mission
{
public:
    Mission(const string& name = "", unsigned long remainder = 0, const string& icon = "",
        int energy = 0, int matter = 0, const string& file = "");
    string name();
    unsigned long remainder();
    void set_remainder(unsigned long remainder);
    string icon();
    int energy();
    int matter();
    string file();

private:
    string m_name;
    unsigned long m_remainder;
    string m_icon;
    int m_energy;
    int m_matter;
    string m_file;
};

#endif
