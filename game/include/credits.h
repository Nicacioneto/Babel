#ifndef CREDITS_H
#define CREDITS_H

#include "button.h"
#include "level.h"
#include <memory>

class Image;

class Credits : public Level
{
public:
    Credits(const string& next = "", const string& image = "res/images/init_screen.png");

    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Image> m_image;
    shared_ptr<Image> m_logo;
    shared_ptr<Image> m_credits;
    Button *m_back;

    void draw_self();
    void update_self(unsigned long elapsed);
};

#endif
