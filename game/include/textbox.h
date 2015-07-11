/*
 * Class that represents a text box.
 *
 * Author: Tiamat
 * Date: 10/07/2015
 * License: LGPL. No copyright.
 */
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <core/object.h>
#include <core/color.h>

class TextBox : public Object
{
public:
    typedef enum { LEFT, RIGHT, JUSTIFY, CENTER } Alignment;

    TextBox(Object *parent, const Rect& area, const string& text = "",
        const Color& text_color = Color::WHITE, Alignment alignment = LEFT);
    
    void set_text(const string& text);
    void set_colors(const Color& border, const Color& background = Color::TRANSPARENT);
    void set_text_color(const Color& text_color);

private:
    class Impl;
    Impl *m_impl;

    void update_self(unsigned long elapsed);
    void draw_self();
};

#endif
