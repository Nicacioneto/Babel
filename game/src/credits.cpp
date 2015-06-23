/*
 * Credits class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "credits.h"
#include <core/font.h>
#include <core/image.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>

#define W 1024.0
#define H 768.0

Credits::Credits(const string& next, const string& image)
    : Level("credits", next), m_texture(nullptr), m_logo(nullptr),
        m_credits(nullptr), m_back(nullptr)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(image);
    m_logo = env->resources_manager->get_texture("res/images/menu/babel-logo.png");
    m_credits = env->resources_manager->get_texture("res/images/menu/credits.png");

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int x = (W - 140)/2 * scale_w;
    int y = (H - 140) * scale_h;
    int w = 140 * scale_w;
    int h = 60 * scale_h;

    m_back = new Button(this, "back", "res/images/menu/button.png", x, y, w, h);
    m_back->set_text("Back");

    m_back->add_observer(this);

    add_child(m_back);
}

void
Credits::draw_self()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int x = 360 * scale_w;
    int y = 193 * scale_h;
    
    env->canvas->clear();
    env->canvas->draw(m_texture.get());
    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w())/2, 25);
    env->canvas->draw(m_credits.get(), x, y);

    set_position(m_back->x() + 48, m_back->y() + 15);
}

bool
Credits::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    finish();

    return true;
}
