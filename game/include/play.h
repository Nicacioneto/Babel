#ifndef PLAY_H
#define PLAY_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class Play : public Level
{
public:
    Play(const string& next = "", const string& texture = "res/images/menu/init-screen.png");
    
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Texture> m_texture;
    shared_ptr<Texture> m_logo;
    shared_ptr<Texture> m_slot_bar;
    Button *m_slot1;
    Button *m_slot2;
    Button *m_slot3;
    Button *m_back;

    void draw_self();
};

#endif
