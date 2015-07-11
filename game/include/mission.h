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
#include <vector>

using std::string;
using std::vector;


class Mission
{
public:
    Mission(const string& name = "", unsigned long remainder = 0, const string& icon = "",
        int energy = 0, int matter = 0, const string& file = "", int m = 0, int p = 0, int t = 0,
        vector<string> c = vector<string>(), int slot = 0);
    string name();
    unsigned long remainder();
    void set_remainder(unsigned long remainder);
    string icon();
    int energy();
    int matter();
    string file();
    int m();
    int p();
    int t();
    vector<string> c();
    void available_character(bool available);
    double calculate_percent();

private:
    string m_name;
    unsigned long m_remainder;
    string m_icon;
    int m_energy;
    int m_matter;
    string m_file;
    int m_m;
    int m_p;
    int m_t;
    vector<string> m_c;
    int m_slot;
};

#endif
