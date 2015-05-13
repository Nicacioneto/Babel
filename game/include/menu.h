#ifndef MENU_H
#define MENU_H

#include "button.h"
#include "level.h"
#include <memory>

class Image;

class Menu : public Level
{
public:
    Menu(const string& next = "", const string& image = "res/images/menu/init-screen.png");
    
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Image> m_image;
    shared_ptr<Image> m_logo;
    Button *m_play;
    Button *m_settings;
    Button *m_credits;
    Button *m_exit;

    void draw_self();
    void update_self(unsigned long elapsed);
};

#endif
