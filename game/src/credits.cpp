#include "credits.h"
#include <core/font.h>
#include <core/image.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>

Credits::Credits(const string& next, const string& image)
    : Level("credits", next), m_texture(nullptr), m_logo(nullptr),
        m_credits(nullptr), m_back(nullptr)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(image);
    m_logo = env->resources_manager->get_texture("res/images/menu/babel-logo.png");
    m_credits = env->resources_manager->get_texture("res/images/menu/credits.png");

    double scale = env->canvas->scale();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22 * scale);

    const int w_button = 140 * scale;
    const int h_button = 60 * scale;
    const int x_back = (env->canvas->w() - w_button) / 2;
    const int y_back = env->canvas->h() - 123 * scale;

    m_back = new Button(this, "back", "res/images/menu/button.png",
        x_back, y_back, w_button, h_button);
    m_back->set_text("Back");

    m_back->add_observer(this);

    add_child(m_back);
}

void
Credits::draw_self()
{
    Environment *env = Environment::get_instance();
    
    double scale = env->canvas->scale();

    env->canvas->clear();
    env->canvas->draw(m_texture.get());
    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2, 25 * scale);
    env->canvas->draw(m_credits.get(), (env->canvas->w() - m_credits->w() * scale)/2, 183 * scale);

    set_position(m_back->x() + 48 * scale, m_back->y() + 15 * scale);
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
