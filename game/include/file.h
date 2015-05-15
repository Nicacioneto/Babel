/*
 * File manager class
 *
 * Author: Tiamat
 * Date: 14/05/2015
 * License: LGPL. No copyright.
 */
#ifndef FILE_H
#define FILE_H

#include <fstream>

using std::string;

extern string read_file(string file);
extern void write_file(const string& text, string file, bool append=0);

#endif
