#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "button.h"
#include <core/level.h>
#include <memory>

class Texture;

class GameMode : public Level
{
public:
    GameMode(const string& next = "", const string& texture = "res/images/menu/init-screen.png");
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Texture> m_babelmode;
    shared_ptr<Texture> m_colonymode;
    shared_ptr<Texture> m_planetmode;
    Button *m_babel;
    Button *m_colony;
    Button *m_planet;

    void draw_self();
    void update_self(unsigned long elapsed);
};

#endif
