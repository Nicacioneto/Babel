#include "credits.h"
#include "font.h"
#include "image.h"
#include "rect.h"
#include "resourcesmanager.h"

#define X_BACK 300
#define Y_BACK 612
#define W_BUTTON 140
#define H_BUTTON 60

Credits::Credits(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_logo(nullptr), m_credits(nullptr), m_back(nullptr)
{
    env = Environment::get_instance();
    m_image = env->resources_manager->get_image(image);
    m_logo = env->resources_manager->get_image("res/images/menu/babel_logo.png");
    m_credits = env->resources_manager->get_image("res/images/menu/credits.png");

    m_back = new Button(this, "back", "res/images/menu/button.png", (env->canvas->w() - W_BUTTON)/2,
        env->canvas->h() - 123, W_BUTTON, H_BUTTON);

    m_back->add_observer(this);

    add_child(m_back);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
}

void
Credits::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    m_back->set_position((env->canvas->w() - W_BUTTON * scale)/2, env->canvas->h() - 123.0 * scale);

    shared_ptr<Font> font = env->canvas->font();
    font->set_size(22 * scale);
}

void
Credits::draw_self()
{
    double scale = env->canvas->scale();

    env->canvas->clear();

    env->canvas->draw(m_image.get());
    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2, 25 * scale);
    env->canvas->draw(m_credits.get(), (env->canvas->w() - m_credits->w() * scale)/2, 183 * scale);

    set_position(m_back->x() + 48 * scale, m_back->y() + 15 * scale);
    env->canvas->draw("Back", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));
}

bool
Credits::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    m_next = "menu";
    m_done = true;

    return true;
}
