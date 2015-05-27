#ifndef CREDITS_H
#define CREDITS_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class Credits : public Level
{
public:
    Credits(const string& next = "menu", const string& image = "res/images/menu/init-screen.png");

    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Texture> m_texture;
    shared_ptr<Texture> m_logo;
    shared_ptr<Texture> m_credits;
    Button *m_back;

    void draw_self();
};

#endif
