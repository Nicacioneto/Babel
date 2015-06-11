#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <core/level.h>
#include <core/listener.h>
#include <memory>

class Image;
class KeyboardEvent;

class GameOver : public Level, Listener
{
public:
    GameOver(const string& next = "menu");
    ~GameOver();
    
private:
    void draw_self();
    bool on_event(const KeyboardEvent& event);
};

#endif
