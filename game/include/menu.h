/*
 * Class that represents the Menu
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef MENU_H
#define MENU_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class Menu : public Level
{
public:
    Menu(const string& next = "", const string& texture = "res/images/menu/init-screen.png");

private:
    shared_ptr<Texture> m_texture;
    shared_ptr<Texture> m_logo;
    Button *m_play;
    Button *m_options;
    Button *m_credits;
    Button *m_exit;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
};

#endif
