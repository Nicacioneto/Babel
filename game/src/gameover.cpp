/*
 * GameOver class implementation.
 *
 * Author: Tiamat
 * Date: 10/05/2015
 * License: LGPL. No copyright.
 */
#include "gameover.h"
#include "rect.h"

GameOver::GameOver(const string& next, const string&)
    : Level("", next)
{
    env = Environment::get_instance();
    env->events_manager->register_keyboard_event_listener(this);
    env->canvas->load_font("res/fonts/FLATS.ttf", 100);

    int x = env->canvas->w()/8;
    int y = env->canvas->h()/4;
    int h = 3*env->canvas->w()/4;
    int w = env->canvas->h()/2;

    Rect rect = Rect(x, y, h, w);
    env->canvas->clear();
    env->canvas->draw_message("GAME OVER", rect);
}

bool
GameOver::onKeyboardEvent(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED and event.key() == KeyboardEvent::SPACE)
    {
        env = Environment::get_instance();
        env->events_manager->unregister_keyboard_event_listener(this);
        m_done = true;

        return true;
    }

    return false;
}
