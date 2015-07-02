/*
 * Class that represents the Action
 *
 * Author: Tiamat
 * Date: 01/07/2015
 * License: LGPL. No copyright.
 */
#ifndef ACTION_H
#define ACTION_H

#include <core/object.h>
#include <map>

using std::map;
using std::shared_ptr;

class Button;
class Texture;

class Action : public Object
{
public:
    Action(int slot, Object *parent = nullptr);

private:
    typedef enum { MILITARY, PSIONIC, TECH } SkillState;
    typedef enum { ATTACK, SKILL, ITEM, DEFENSE, REST, RUN, NONE } ActionState;

    int m_slot;

    map<ObjectID, Button*> m_buttons;
    map<ObjectID, shared_ptr<Texture>> m_textures;
    SkillState m_mpt;
    ActionState m_state;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void create_buttons();
    void change_buttons();
    void draw_attack();
    void draw_skill();
    void draw_confirm_box(string icon);
};

#endif
