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
    
private:
    unsigned long m_last;
    shared_ptr<Texture> m_texture;
    shared_ptr<Texture> m_logo;
    shared_ptr<Texture> m_slot_bar;
    Button *m_slots[3][2];
    Button *m_back;

    void update_self(unsigned long elapsed);
    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void slots();
};

#endif
