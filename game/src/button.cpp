/*
 * Button class implementation.
 *
 * Author: Tiamat
 * Date: 10/05/2015
 * License: LGPL. No copyright.
 */
#include "button.h"
#include "settings.h"
#include <core/color.h>
#include <core/rect.h>

#include <cstdio>

MessageID Button::clickedID = "clicked()";

Button::Button(Object *parent, ObjectID id, const string& texture,
    double x, double y, double w, double h)
    : Object(parent, id, x, y, w, h), m_text(nullptr), m_texture(nullptr), m_state(HIDE)
{
    Environment *env = Environment::get_instance();
    env->events_manager->register_mouse_button_event_listener(this);
    env->events_manager->register_mouse_motion_event_listener(this);

    if (texture != "")
    {
        m_texture = env->resources_manager->get_texture(texture);
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
    env = Environment::get_instance();

    if (m_state != HIDE)
    {
        if (m_state == IDLE)
        {
            Rect clip = Rect(0, 0, m_texture->w(), m_texture->h()/2);
            env->canvas->draw(m_texture.get(), clip, x(), y());
        }
        else if (m_state == ON_HOVER)
        {
            Rect clip = Rect(0, m_texture->h()/2, m_texture->w(), m_texture->h()/2);
            env->canvas->draw(m_texture.get(), clip, x(), y());
        }

        if (m_text)
        {
            m_text->align_to(this, Object::CENTER, Object::MIDDLE);
            m_text->draw();
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

        AudioManagerSfx *sfx = env->sfx;
        sfx->set_volume(Settings::volume());
        sfx->play("res/sfx/uiConfirm1.ogg", 1);

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
        if (m_state != ON_HOVER)
        {
            AudioManagerSfx *sfx = env->sfx;
            sfx->set_volume(Settings::volume());
            sfx->play("res/sfx/uiMouseOver2.ogg", 1);
        }
        m_state = ON_HOVER;
        return true;
    }

    m_state = IDLE;

    return false;
}

void
Button::set_text(const string& str, const Color& color)
{
    m_text = new Text(this, str, color);
}

Text*
Button::text()
{
    return m_text;
}
