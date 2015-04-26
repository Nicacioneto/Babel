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
#define MIN_RESOLUTION 625
#define SCALE 1.28

class Image;

class Settings : public Level
{
public:
    Settings(const string& next, const string& image);
    bool execute_action(const int x, const int y);

private:
    Image *m_image;
	int m_x_back = 49;
	int m_y_back = 480;
	int m_w_back_button = 428;
	int m_h_back_button = 103;
	int m_x_up_resolution = 449;
	int m_y_up_resolution = 169;
	int m_x_down_resolution = 449;
	int m_y_down_resolution = 187;
	int m_w_resolution_button = 21;
	int m_h_resolution_button = 13;

    void draw_self();
    void update_coodirnates_buttons();

};

#endif
