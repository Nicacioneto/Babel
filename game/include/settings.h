#ifndef SETTINGS_H
#define SETTINGS_H

#include "level.h"

#define X_BACK 49
#define Y_BACK 480
#define W_BACK_BUTTON 428
#define H_BACK_BUTTON 103
#define X_UP_RESOLUTION 449
#define Y_UP_RESOLUTION 169
#define X_DOWN_RESOLUTION 449
#define Y_DOWN_RESOLUTION 187
#define W_RESOLUTION_BUTTON 21
#define H_RESOLUTION_BUTTON 13
#define MAX_RESOLUTION 1310
#define MIN_RESOLUTION 800
#define SCALE 1.28 // Scale to keep display standart

class Image;

class Settings : public Level
{
public:
    Settings(const string& next = "", const string& image = "res/images/settings.png");
    bool execute_action(const int x, const int y);

private:
    Image *m_image;
    Image *m_resolution;
    int m_resolution_position = 150;
    int m_x_back = X_BACK;
    int m_y_back = Y_BACK;
    int m_w_back_button = W_BACK_BUTTON;
    int m_h_back_button = H_BACK_BUTTON;
    int m_x_up_resolution = X_UP_RESOLUTION;
    int m_y_up_resolution = Y_UP_RESOLUTION;
    int m_x_down_resolution = X_DOWN_RESOLUTION;
    int m_y_down_resolution = Y_DOWN_RESOLUTION;
    int m_w_resolution_button = W_RESOLUTION_BUTTON;
    int m_h_resolution_button = H_RESOLUTION_BUTTON;

    void draw_self();
    void update_coordinates_buttons();
};

#endif
