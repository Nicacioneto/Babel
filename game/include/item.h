/*
 * Class that represents the Item
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef ITEM_H
#define ITEM_H

#include <core/object.h>
#include <map>

using std::map;

class Button;
class Texture;
class Colony;
class Font;

class Item : public Object
{
public:
    Item(int slot, Colony *colony, Object *parent = nullptr);

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
    void draw_items(double scale_w, double scale_h, Color color);
    bool on_message(Object *sender, MessageID id, Parameters p);
    void create_buttons();
    void change_buttons(bool visible = false, bool active = false);
    void change_button_state(Button *button, bool state, int y = 0);
    void buy_item(const ObjectID id);
};

#endif
