/*
 * Class that represents the Facilities
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef FACILITIES_H
#define FACILITIES_H

#include "button.h"
#include "base.h"
#include "colony.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;

class Facilities : public Level
{
public:
    Facilities(int slot, const string& next = "");

private:
    typedef enum { CHAT, MILITARY, PSIONIC, TECH } Screen;

    int m_slot, m_mcost, m_pcost, m_tcost;
    Colony *m_colony;
    Screen m_screen;
    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button *> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
    void create_buttons();
    void change_buttons();
    void change_to_chat();
    void change_to_military();
    void change_to_psionic();
    void change_to_tech();
};

#endif
