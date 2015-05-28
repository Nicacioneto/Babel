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
    void update_self(unsigned long elapsed);
    void draw_self(double x0 = 0, double y0 = 0);
    bool on_event(const KeyboardEvent& event);
};

#endif
