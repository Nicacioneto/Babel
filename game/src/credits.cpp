#include "credits.h"
#include "image.h"
#include "resourcesmanager.h"

#define X_BACK 273
#define Y_BACK 612
#define W_BACK 428
#define H_BACK 103

Credits::Credits(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_back(nullptr)
{
    env = Environment::get_instance();
    m_image = env->resources_manager->get_image(image);

    m_back = new Button(this, "back", X_BACK, Y_BACK, W_BACK, H_BACK, Color::TRANSPARENT);

    m_back->add_observer(this);

    add_child(m_back);
}

void
Credits::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    m_back->set_position(scale * X_BACK, scale * Y_BACK);
    m_back->set_dimensions(scale * W_BACK, scale * H_BACK);
}

void
Credits::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());
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