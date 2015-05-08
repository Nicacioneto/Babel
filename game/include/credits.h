#ifndef CREDITS_H
#define CREDITS_H

#include "level.h"
#include <memory>

class Image;

class Credits : public Level
{
public:
    Credits(const string& next = "", const string& image = "res/images/credits.png");

    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Image> m_image;

    void draw_self();
    void update_coordinates_buttons();
};

#endif
