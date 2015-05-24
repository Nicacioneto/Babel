#ifndef BUTTON_H
#define BUTTON_H

#include <core/environment.h>
#include <core/mousebuttonevent.h>
#include <core/mousebuttoneventlistener.h>
#include <core/mousemotionevent.h>
#include <core/mousemotioneventlistener.h>
#include <core/object.h>
#include <core/text.h>
#include <core/texture.h>
#include <memory>

using std::shared_ptr;
using std::string;

typedef enum { IDLE, ON_HOVER, HIDE } State;

class Button : public Object, public MouseButtonEventListener, MouseMotionEventListener
{
public:
    Button(Object *parent = nullptr, ObjectID id = "", const string& image = "",
        double x = 0, double y = 0, double w = 100, double h = 100);

    ~Button();

    bool onMouseButtonEvent(const MouseButtonEvent& event);
    bool onMouseMotionEvent(const MouseMotionEvent& event);
    void set_text(const string& str, const Color& color = Color(170, 215, 190));
    Text* text();

    static ActionID clickedID;

private:
    Text *m_text;
    shared_ptr<Texture> m_texture;
    State m_state;
    Environment *env;

    void draw_self();
};

#endif
