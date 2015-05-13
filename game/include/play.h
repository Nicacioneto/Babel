#ifndef PLAY_H
#define PLAY_H

#include "button.h"
#include "level.h"
#include <memory>

class Image;

class Play : public Level
{
public:
    Play(const string& next = "", const string& image = "res/images/menu/init-screen.png");
    
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Image> m_image;
    shared_ptr<Image> m_logo;
    shared_ptr<Image> m_slot_bar;
    Button *m_slot1;
    Button *m_slot2;
    Button *m_slot3;
    Button *m_back;

    void draw_self();
    void update_self(unsigned long elapsed);
};

#endif
