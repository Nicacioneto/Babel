/*
 * Class that represents the Central
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
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
    Central(int slot, const string& next = "");

private:
    typedef enum { CHAT, QUESTS, BESTIARY, TIMERS } Screen;

    int m_slot;
    Screen m_screen;

    unsigned long m_last;
    shared_ptr<Texture> m_scenario;
    int m_page;
    int m_max_pages;
    Text *m_text;
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
    void change_buttons();
    void change_to_chat();
    void change_to_quests();
    void change_to_bestiary();
    void change_to_timers();
    void update_self(unsigned long elapsed);
    void update_max_pages(int sections);
    void set_pages_text();
};

#endif
