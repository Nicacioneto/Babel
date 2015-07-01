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
    int m_slot;
    // shared_ptr<Font> m_font;

    map<ObjectID, Button*> m_buttons;
    map<ObjectID, shared_ptr<Texture>> m_textures;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void create_buttons();
    // void change_buttons(bool visible = false, bool active = false);
    // void change_button_state(Button *button, bool state, int y = 0);
};

#endif
