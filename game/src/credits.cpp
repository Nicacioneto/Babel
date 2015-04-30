#include "button.h"
#include "credits.h"
#include "image.h"
#include "resourcesmanager.h"

Credits::Credits(const string& next, const string& image)
    : Level("", next), m_image(nullptr)
{
    env = Environment::get_instance();
    shared_ptr<Resource> r = env->resources_manager->get(Resource::IMAGE, image);
    m_image = dynamic_cast<Image *>(r.get());
}

void
Credits::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image);
}


void
Credits::update_coordinates_buttons()
{
    double scale = env->canvas->scale();
    m_x_back = scale * X_BACK_CREDITS;
    m_y_back = scale * Y_BACK_CREDITS;
    m_w_back = scale * W_BACK_CREDITS;
    m_h_back = scale * H_BACK_CREDITS;
}

bool
Credits::execute_action(const int x, const int y)
{
    update_coordinates_buttons();
    
    Button back_button(m_x_back, m_y_back, m_w_back, m_h_back);

    if (back_button.is_clicked(x, y))
    {
        m_next = "menu";
        m_done = true;
    }

    return false;
}
