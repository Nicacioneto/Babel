/*
 * GameOver class implementation.
 *
 * Author: Tiamat
 * Date: 10/05/2015
 * License: LGPL. No copyright.
 */
#include "font.h"
#include "gameover.h"
#include "rect.h"

GameOver::GameOver(const string& next)
    : Level("", next)
{
    env = Environment::get_instance();

    env->events_manager->register_keyboard_event_listener(this);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/FLATS.ttf");
    font->set_style(Font::ITALIC);
    env->canvas->set_font(font);
}

GameOver::~GameOver()
{
    env->events_manager->unregister_keyboard_event_listener(this);
}

void
GameOver::update_self(unsigned long)
{
    double scale = env->canvas->scale();
    set_position(env->canvas->w() / (8 * scale), env->canvas->h() / (4 * scale));

    shared_ptr<Font> font = env->canvas->font();
    font->set_size(100 * scale);
}

void
GameOver::draw_self()
{
    env->canvas->clear();
    env->canvas->draw("GAME OVER", bounding_box().x(), bounding_box().y(), Color::YELLOW);
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
