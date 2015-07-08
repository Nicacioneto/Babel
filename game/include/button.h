/*
 * Class that represents the Button
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <core/environment.h>
#include <core/listener.h>
#include <core/object.h>
#include <core/text.h>
#include <core/texture.h>
#include <memory>

using std::shared_ptr;
using std::string;

class Color;
class MouseButtonEvent;
class MouseMotionEvent;

class Button : public Object, public Listener
{
public:
    typedef enum { IDLE, ON_HOVER, ACTIVE, INACTIVE } State;
    
    Button(Object *parent = nullptr, ObjectID id = "", const string& texture = "",
        double x = 0, double y = 0, double w = 1, double h = 1);
    Button(Object *parent, ObjectID id = "",
        double x = 0, double y = 0, double w = 1, double h = 1, Color color = Color::TRANSPARENT);

    ~Button();

    bool on_event(const MouseButtonEvent& event);
    bool on_event(const MouseMotionEvent& event);
    void set_text(const string& str, const Color& color = Color(170, 215, 190));
    void remove_text();
    State state();
    Text* text();

    void set_sprites(const int num);
    void change_state(State to);

    void set_texture(const string& texture);

    static ActionID clickedID;
    static ActionID hoverID;

private:
    Text *m_text;
    shared_ptr<Texture> m_texture;
    State m_state;
    int m_sprites;
    Color m_color;

    void draw_self();
};

#endif
