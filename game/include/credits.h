#ifndef CREDITS_H
#define CREDITS_H

#include "level.h"
#include <memory>

#define X_BACK_CREDITS 273
#define Y_BACK_CREDITS 612
#define W_BACK_CREDITS 428
#define H_BACK_CREDITS 103

class Image;

class Credits : public Level
{
public:
    Credits(const string& next = "", const string& image = "res/images/credits.png");

    bool execute_action(const int x, const int y);

private:
    shared_ptr<Image> m_image;

    int m_x_back = X_BACK_CREDITS;
    int m_y_back = Y_BACK_CREDITS;
    int m_w_back = W_BACK_CREDITS;
    int m_h_back = H_BACK_CREDITS;

    void draw_self();
    void update_coordinates_buttons();
};

#endif
