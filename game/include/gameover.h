#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "level.h"
#include "keyboardevent.h"
#include "keyboardeventlistener.h"
#include <memory>

class Image;

class GameOver : public Level, public KeyboardEventListener
{
public:
    GameOver(const string& next = "menu", const string& image = "");
    
    bool onKeyboardEvent(const KeyboardEvent& event);
};

#endif
