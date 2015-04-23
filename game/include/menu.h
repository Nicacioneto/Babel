#ifndef MENU_H
#define MENU_H

#include "level.h"

#define X_START 119
#define Y_START 154
#define X_SETTINGS 119
#define Y_SETTINGS 313
#define X_EXIT 119
#define Y_EXIT 475
#define X_BACK 49
#define Y_BACK 480
#define W_BUTTON 428
#define H_BUTTON 103

class Image;

class Menu : public Level
{
public:
    Menu(const string& next, const string& image);
    bool execute_action(const int x, const int y);

private:
    Image *m_image;

    void draw_self();

};

#endif
