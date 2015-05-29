#ifndef CENTRAL_H
#define CENTRAL_H

#include "button.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;

class Central : public Level
{
public:
    Central(const string& next = "");

private:
    typedef enum { CHAT, QUESTS, BESTIARY, TIMERS } Screen;
    shared_ptr<Texture> m_scenario;
    
    map<ObjectID, Button*> m_buttons;

    Screen m_screen;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self(double x0 = 0, double y0 = 0);
    void create_buttons();
    void change_buttons();
    void change_to_chat();
    void change_to_quests();
    void change_to_bestiary();
    void change_to_timers();
};

#endif
