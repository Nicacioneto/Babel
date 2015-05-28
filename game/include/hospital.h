#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "button.h"
#include "base.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;

class Hospital : public Level
{
public:
    Hospital(const string& next = "");

private:
    typedef enum { CHAT, ITEMS, RESEARCH, REVIVE } Screen;
    shared_ptr<Texture> m_scenario;
    
    Button *m_reset;
    Button *m_buy;
    map<ObjectID, Button*> m_buttons;

    Screen m_screen;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self(double x0 = 0, double y0 = 0);
    void create_buttons();
    void change_buttons();
    void change_to_chat();
    void change_to_items();
    void change_to_research();
    void change_to_revive();
};

#endif
