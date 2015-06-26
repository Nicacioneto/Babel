/*
 * Class that represents the Hospital
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "base.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Button;
class Colony;
class Texture;

class Hospital : public Level
{
public:
    Hospital(int slot, const string& next = "");

private:
    typedef enum { CHAT, ITEMS, RESEARCH, REVIVE } Screen;

    int m_slot;
    Colony *m_colony;
    Screen m_screen;
    shared_ptr<Texture> m_scenario;
    int m_page;
    int m_max_pages;
    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Button*> m_items;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
    void create_items();
    void change_buttons();
    void change_items();
    void buy_item(const ObjectID id);
    void chat_screen();
    void items_screen();
    void research_screen();
    void revive_screen();
};

#endif
