/*
 * File class implementation.
 *
 * Author: Tiamat
 * Date: 14/05/2015
 * License: LGPL. No copyright.
 */
#include "file.h"

using std::endl;
using std::ifstream;
using std::ios;
using std::ofstream;

string read_file(string file) throw (Exception)
{
    ifstream fin(file.c_str());

    if (fin.is_open() == false)
    {
        throw Exception("File not found!");
    }

    string line, text;
    while (getline(fin, line))
    {
        text += line + '\n';
    }

    fin.close();

    return text;
}

void write_file(const string& text, string file, bool append)
{
    if (append)
    {
        ofstream fout(file.c_str(), ios::app);
        fout << text << endl;
    }
    else
    {
        ofstream fout(file.c_str());
        fout << text << endl;
    }
}
