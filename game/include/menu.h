#ifndef MENU_H
#define MENU_H

#include "level.h"

class Image;

class Menu : public Level
{
public:
    Menu(const string& next, const string& image);

private:
    Image *m_image;
    int m_x, m_y;

    void draw_self();
};

#endif
