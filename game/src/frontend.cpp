/*
 * FrontEnd class implementation.
 *
 * Author: Tiamat
 * Date: 20/04/2015
 * License: LGPL. No copyright.
 */
#include "frontend.h"
#include "environment.h"
#include "image.h"
#include "resourcesmanager.h"

FrontEnd::FrontEnd(const string& next, const string& image,
    unsigned long duration)
    : Level("", next), m_image(nullptr), m_start(0), m_duration(duration)
{
    env = Environment::get_instance();
    env->events_manager->register_keyboard_event_listener(this);

    m_image = env->resources_manager->get_image(image);

    m_x = (env->canvas->w() - m_image->w())/2;
    m_y = (env->canvas->h() - m_image->h())/2;
}

void
FrontEnd::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get(), m_x, m_y);
}

void
FrontEnd::update_self(unsigned long elapsed)
{
    if (not m_start)
    {
        m_start = elapsed;
    }

    unsigned long now = elapsed - m_start;
    unsigned long in = m_duration / 6;
    unsigned long out = m_duration - in;
    unsigned char alpha = SDL_ALPHA_OPAQUE;

    if (now < in)
    {
        alpha *= 1.0*now/in;
    }
    else if (now > out)
    {
        alpha *= 1 - 1.0*(now - out)/in;
    }

    if (now > m_duration)
    {
        m_done = true;
    }
    else
    {
        SDL_SetTextureAlphaMod(m_image->texture(), alpha);
    }
}

bool
FrontEnd::onKeyboardEvent(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED and event.key() == KeyboardEvent::SPACE)
    {
        m_done = true;
        m_next = "menu";

        env->events_manager->unregister_keyboard_event_listener(this);

        return true;
    }

    return false;
}
