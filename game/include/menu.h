#ifndef MENU_H
#define MENU_H

#include "level.h"
#include <memory>

class Image;

class Menu : public Level
{
public:
    Menu(const string& next = "", const string& image = "res/images/menu.png");
    
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Image> m_image;

    void draw_self();
    void update_coordinates_buttons();
};

#endif
