#include "button.h"
#include "environment.h"
#include "image.h"
#include "menu.h"
#include "resourcesmanager.h"

Menu::Menu(const string& next, const string& image)
    : Level("", next), m_image(nullptr)
{
    env = Environment::get_instance();
    shared_ptr<Resource> r = env->resources_manager->get(Resource::IMAGE,
        image);
    m_image = dynamic_cast<Image *>(r.get());
}

void
Menu::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image, 0, 0);
}

void 
Menu::update_coodirnates_buttons()
{
    double scale = env->canvas->scale();

    m_x_start = scale * X_START;
    m_y_start = scale * Y_START;
    m_x_settings = scale * X_SETTINGS;
    m_y_settings = scale * Y_SETTINGS;
    m_x_exit = scale * X_EXIT;
    m_y_exit = scale * Y_EXIT;
    m_w_button = scale * W_BUTTON;
    m_h_button = scale * H_BUTTON;
}

bool
Menu::execute_action(const int x, const int y)
{
    update_coodirnates_buttons();

    Button start_button(m_x_start, m_y_start, m_w_button, m_h_button);
    Button settings_button(m_x_settings, m_y_settings, m_w_button, m_h_button);
    Button exit_button(m_x_exit, m_y_exit, m_w_button, m_h_button);

    if (start_button.is_clicked(x, y))
    {
        //TO DO
    }
    else if (settings_button.is_clicked(x, y))
    {
        m_next = "settings";
        m_done = true;
    }
    else if (exit_button.is_clicked(x, y))
    {
        return true;
    }

    return false;
}
