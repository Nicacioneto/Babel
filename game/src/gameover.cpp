/*
 * GameOver class implementation.
 *
 * Author: Tiamat
 * Date: 10/05/2015
 * License: LGPL. No copyright.
 */
#include "gameover.h"
#include "rect.h"

GameOver::GameOver(const string& next)
    : Level("", next)
{
    env = Environment::get_instance();

    env->events_manager->register_keyboard_event_listener(this);
    env->canvas->load_font("res/fonts/FLATS.ttf", 100);
}

GameOver::~GameOver()
{
    env->events_manager->unregister_keyboard_event_listener(this);
}

void
GameOver::update_self(unsigned long)
{
    set_position(env->canvas->w()/8, env->canvas->h()/4);
    set_dimensions(3*env->canvas->w()/4, env->canvas->h()/2);
}

void
GameOver::draw_self()
{
    env->canvas->clear();
    env->canvas->draw_message("GAME OVER", bounding_box());
}

bool
GameOver::onKeyboardEvent(const KeyboardEvent& event)
{
    if (event.key() == KeyboardEvent::SPACE)
    {
        m_done = true;
        return true;
    }

    return false;
}
