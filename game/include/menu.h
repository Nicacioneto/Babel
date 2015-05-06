#ifndef MENU_H
#define MENU_H

#include "level.h"
#include <memory>

#define X_START 119
#define Y_START 54
#define X_SETTINGS 119
#define Y_SETTINGS 190
#define X_CREDITS 119
#define Y_CREDITS 335
#define X_EXIT 119
#define Y_EXIT 475
#define W_BUTTON 428
#define H_BUTTON 103

class Image;

class Menu : public Level
{
public:
    Menu(const string& next = "", const string& image = "res/images/menu.png");
    bool execute_action(const int x, const int y);

private:
    shared_ptr<Image> m_image;

    int m_x_start = X_START;
    int m_y_start = Y_START;
    int m_x_settings = X_SETTINGS;
    int m_y_settings = Y_SETTINGS;
    int m_x_credits = X_CREDITS;
    int m_y_credits = Y_CREDITS;
    int m_x_exit = X_EXIT;
    int m_y_exit = Y_EXIT;
    int m_w_button = W_BUTTON;
    int m_h_button = H_BUTTON;

    void draw_self();
    void update_coordinates_buttons();
};

#endif
