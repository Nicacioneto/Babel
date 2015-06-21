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
    Hospital(int slot, const string& next = "");

private:
    typedef enum { CHAT, ITEMS, RESEARCH, REVIVE } Screen;

    int m_slot;
    Screen m_screen;
    shared_ptr<Texture> m_scenario;
    map<ObjectID, Button*> m_buttons;


    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
    void change_buttons();
    void change_to_chat();
    void change_to_items();
    void change_to_research();
    void change_to_revive();
};

#endif
