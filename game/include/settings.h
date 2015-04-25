#ifndef SETTINGS_H
#define SETTINGS_H

#include "level.h"

#define X_BACK 49
#define Y_BACK 480
#define W_BUTTON 428
#define H_BUTTON 103

class Image;

class Settings : public Level
{
public:
    Settings(const string& next, const string& image);
    bool execute_action(const int x, const int y);

private:
    Image *m_image;

    void draw_self();

};

#endif
