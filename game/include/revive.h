/*
 * Class that represents the Revive
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef REVIVE_H
#define REVIVE_H

#include <core/object.h>
#include <map>

using std::map;

class Texture;
class Font;

class Revive : public Object
{
public:
    Revive(int slot, Colony *colony, Object *parent = nullptr);

private:
    int m_slot;
    Colony *m_colony;
    shared_ptr<Settings> m_settings;
    int m_page;
    int m_max_pages;
    shared_ptr<Font> m_font;

    map<ObjectID, Button*> m_buttons;
    map<ObjectID, shared_ptr<Texture>> m_textures;

    void draw_self();
    void draw_character(double scale_w, double scale_h, Color color);
    bool on_message(Object *sender, MessageID id, Parameters p);
    void create_buttons();
    void change_buttons(bool visible = false, bool active = false);
    void change_button_state(Button *button, bool state, int y = 0);
    void revive_character(const ObjectID id);
};

#endif
