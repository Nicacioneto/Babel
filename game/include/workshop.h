#ifndef WORKSHOP_H
#define WORKSHOP_H

#include "button.h"
#include "base.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;

class Workshop : public Level
{
public:
    Workshop(const string& next = "");

private:
    typedef enum { CHAT, DRONE, VEHICLE, ARSENAL } Screen;
    shared_ptr<Texture> m_scenario;
    
    map<ObjectID, Button*> m_buttons;
    Screen m_screen;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
    void change_buttons();
    void change_to_chat();
    void change_to_drone();
    void change_to_vehicle();
    void change_to_arsenal();
};

#endif
