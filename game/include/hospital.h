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

#define BIG_LIST 7

class Button;
class Colony;
class Texture;
class Text;

class Hospital : public Level
{
public:
    Hospital(int slot, const string& next = "");

private:
    int m_slot;
    Colony *m_colony;
    string m_screen;
    shared_ptr<Texture> m_scenario;
    int m_page;
    int m_max_pages;
    Text *m_text;
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
    void change_buttons();
    void chat_screen();
    void revive_screen();
    void update_max_pages(int sections);
    void set_pages_text();
};

#endif
