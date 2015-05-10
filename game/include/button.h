#ifndef BUTTON_H
#define BUTTON_H

#include "image.h"
#include "mousebuttoneventlistener.h"
#include "mousemotioneventlistener.h"
#include "object.h"
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
    shared_ptr<Image> m_image;
    State m_state;

    void draw_self();
};

#endif
