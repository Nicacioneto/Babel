/*
 * Button class implementation.
 *
 * Author: Tiamat
 * Date: 10/05/2015
 * License: LGPL. No copyright.
 */
#include "color.h"
#include "rect.h"
#include "button.h"
#include "environment.h"
#include "mousebuttonevent.h"
#include "mousemotionevent.h"

#include <cstdio>

MessageID Button::clickedID = "clicked()";

Button::Button(Object *parent, ObjectID id, const string& image,
    double x, double y, double w, double h)
    : Object(parent, id, x, y, w, h), m_image(nullptr), m_state(HIDE)
{
    Environment *env = Environment::get_instance();
    env->events_manager->register_mouse_button_event_listener(this);
    env->events_manager->register_mouse_motion_event_listener(this);

    if (image != "")
    {
        m_image = env->resources_manager->get_image(image);
        m_state = IDLE;
    }

}

Button::~Button()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_mouse_button_event_listener(this);
    env->events_manager->unregister_mouse_motion_event_listener(this);
}

void
Button::draw_self()
{
    Environment *env = Environment::get_instance();

    if (m_state != HIDE)
    {
        if (m_state == IDLE)
        {
            Rect clip = Rect(0, 0, w(), h());
            env->canvas->draw(m_image.get(), clip, x(), y());
        }
        else if (m_state == ON_HOVER)
        {
            Rect clip = Rect(0, h(), w(), h());
            env->canvas->draw(m_image.get(), clip, x(), y());
        }
    }
}

bool
Button::onMouseButtonEvent(const MouseButtonEvent& event)
{
    if (event.state() == MouseButtonEvent::PRESSED and
        event.button() == MouseButtonEvent::LEFT and
        bounding_box().contains(event.x(), event.y()))
    {
        char coords[64];
        sprintf(coords, "%.2f,%.2f", event.x(), event.y());

        notify(clickedID, coords);
        return true;
    }

    return false;
}

bool
Button::onMouseMotionEvent(const MouseMotionEvent& event)
{
    if (m_state == HIDE)
    {
        return false;
    }

    if (bounding_box().contains(event.x(), event.y()))
    {
        m_state = ON_HOVER;
        return true;
    }

    m_state = IDLE;

    return false;
}
