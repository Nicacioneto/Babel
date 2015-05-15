#ifndef BUTTON_H
#define BUTTON_H

#include <core/mousebuttonevent.h>
#include <core/mousebuttoneventlistener.h>
#include <core/mousemotionevent.h>
#include <core/mousemotioneventlistener.h>
#include <core/object.h>
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

    static ActionID clickedID;

private:
    shared_ptr<Texture> m_texture;
    State m_state;

    void draw_self();
};

#endif
