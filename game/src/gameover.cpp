/*
 * GameOver class implementation.
 *
 * Author: Tiamat
 * Date: 10/05/2015
 * License: LGPL. No copyright.
 */
#include "gameover.h"
#include <core/font.h>
#include <core/keyboardevent.h>
#include <core/rect.h>

#define W 1024.0
#define H 768.0

GameOver::GameOver(const string& next)
    : Level("gameover", next)
{
    Environment *env = Environment::get_instance();

    env->events_manager->register_listener(this);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/FLATS.ttf");
    font->set_style(Font::ITALIC);
    env->canvas->set_font(font);
    font->set_size(100);
    set_position((150 / W) * env->canvas->w(), (H - 500) / H * env->canvas->h());
}

GameOver::~GameOver()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
GameOver::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    env->canvas->draw("GAME OVER", bounding_box().x(), bounding_box().y(), Color::YELLOW);
}

bool
GameOver::on_event(const KeyboardEvent& event)
{
    if (event.key() == KeyboardEvent::SPACE)
    {
        finish();
        return true;
    }

    return false;
}
