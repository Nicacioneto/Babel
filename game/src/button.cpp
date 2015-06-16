/*
 * Button class implementation.
 *
 * Author: Tiamat
 * Date: 10/05/2015
 * License: LGPL. No copyright.
 */
#include "button.h"
#include <core/color.h>
#include <core/mousebuttonevent.h>
#include <core/mousemotionevent.h>
#include <core/rect.h>
#include <core/settings.h>

#include <cstdio>

MessageID Button::clickedID = "clicked()";

Button::Button(Object *parent, ObjectID id, const string& texture,
    double x, double y, double w, double h)
    : Object(parent, id, x, y, w, h), m_text(nullptr), m_texture(nullptr),
        m_state(IDLE), m_sprites(2)
{
    Environment *env = Environment::get_instance();
    env->events_manager->register_listener(this);

    if (texture.empty())
    {
        m_state = HIDE;
        set_visible(false);
    }
    else
    {
        m_texture = env->resources_manager->get_texture(texture);
    }
}

Button::~Button()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);

    remove_text();
}

void
Button::draw_self()
{
    Environment *env = Environment::get_instance();
    env = Environment::get_instance();

    if (m_state == IDLE or m_sprites == 1)
    {
        Rect clip = Rect(0, 0, m_texture->size().first, m_texture->size().second/m_sprites);
        env->canvas->draw(m_texture.get(), clip, x(), y(), w(), h());
    }
    else if (m_state == ON_HOVER)
    {
        Rect clip = Rect(0, m_texture->size().second/m_sprites, m_texture->size().first,
            m_texture->size().second/m_sprites);
        env->canvas->draw(m_texture.get(), clip, x(), y(), w(), h());
    }
    else if (m_state == ACTIVE)
    {
        Rect clip = Rect(0, 2 * m_texture->size().second/m_sprites, m_texture->size().first,
            m_texture->size().second/m_sprites);
        env->canvas->draw(m_texture.get(), clip, x(), y(), w(), h());
    }

    if (m_text)
    {
        m_text->align_to(this, Object::CENTER, Object::MIDDLE);
        m_text->draw();
    }
}

bool
Button::on_event(const MouseButtonEvent& event)
{
    if (event.state() == MouseButtonEvent::PRESSED and
        event.button() == MouseButtonEvent::LEFT and
        bounding_box().contains(event.x(), event.y()))
    {
        char coords[64];
        sprintf(coords, "%.2f,%.2f", event.x(), event.y());
        notify(clickedID, coords);

        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);

        return true;
    }

    return false;
}

bool
Button::on_event(const MouseMotionEvent& event)
{
    if (m_state == HIDE or m_state == ACTIVE)
    {
        return false;
    }

    if (bounding_box().contains(event.x(), event.y()))
    {
        if (m_state != ON_HOVER)
        {
            Environment *env = Environment::get_instance();
            env->sfx->play("res/sfx/uiMouseOver2.ogg", 1);
        }

        m_state = ON_HOVER;
    }
    else
    {
        m_state = IDLE;
    }

    return false;
}

void
Button::set_text(const string& str, const Color& color)
{
    m_text = new Text(this, str, color);
}

void
Button::remove_text()
{
    if (m_text)
    {
        delete m_text;
        m_text = nullptr;
    }
}

Text*
Button::text()
{
    return m_text;
}

void
Button::set_sprites(const int num)
{
    m_sprites = num;
}

void
Button::change_state(State to)
{
    m_state = to;
}

void
Button::set_texture(const string& texture)
{
    Environment *env = Environment::get_instance();
    m_texture = env->resources_manager->get_texture(texture);
}
