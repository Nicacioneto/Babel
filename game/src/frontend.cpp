/*
 * Implementação da class FrontEnd.
 *
 * Autor: Edson Alves
 * Data: 20/04/2015
 * Licença: LGPL. Sem copyright.
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
    Uint8 alpha = m_image->alpha();

    if (not m_start)
    {
        m_start = elapsed;
    }

    if (elapsed - m_start > 0 and elapsed - m_start < 400)
    {
        alpha < SDL_ALPHA_OPAQUE ? alpha++ : alpha = SDL_ALPHA_OPAQUE;
    }
    else if (elapsed - m_start > (m_duration - 400) and elapsed - m_start < m_duration)
    {
        alpha > SDL_ALPHA_TRANSPARENT ? alpha-- : alpha = SDL_ALPHA_TRANSPARENT;
    }
    else
    {
        m_image->set_alpha(SDL_ALPHA_OPAQUE);
    }

    m_image->set_alpha(alpha);
    SDL_SetTextureAlphaMod(m_image->texture(), alpha);

    if (elapsed - m_start > m_duration)
    {
        m_done = true;
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
