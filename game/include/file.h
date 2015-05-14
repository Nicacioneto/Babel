/*
 * File manager class
 *
 * Author: Tiamat
 * Date: 14/05/2015
 * License: LGPL. No copyright.
 */
#include <fstream>

using std::string;

class File
{
public:
    string readText(string file);
    void writeTextOnFile(const string& text, string file, bool append=0);
};
