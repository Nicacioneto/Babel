#ifndef MENU_H
#define MENU_H

#include "level.h"

#define X_START 119
#define Y_START 154
#define X_SETTINGS 119
#define Y_SETTINGS 313
#define X_EXIT 119
#define Y_EXIT 475
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

	int m_x_start = 119;
	int m_y_start = 154;
	int m_x_settings = 119;
	int m_y_settings = 313;
	int m_x_exit = 119;
	int m_y_exit = 475;
	int m_w_button = 428;
	int m_h_button = 103;

    void draw_self();
    void update_coodirnates_buttons();

};

#endif
