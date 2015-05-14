/*
 * File class implementation.
 *
 * Author: Tiamat
 * Date: 14/05/2015
 * License: LGPL. No copyright.
 */
#include "file.h"

string
File::readText(string file)
{
    const string directory_files = "files/";
    file = directory_files + file;
    ifstream fin(file.c_str());

    if (fin.is_open() == false)
    {
        return "5";
    }

    string line, text;
    getline(fin, line);
    text += line;

    return text;
}

void
File::writeTextOnFile(const string& text, string file, bool append)
{
    const string directory_files = "files/";
    file = directory_files + file;
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
