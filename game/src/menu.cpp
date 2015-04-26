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

bool
Menu::execute_action(const int x, const int y)
{
    Button start_button(X_START, Y_START, W_BUTTON, H_BUTTON);
    Button settings_button(X_SETTINGS, Y_SETTINGS, W_BUTTON, H_BUTTON);
    Button exit_button(X_EXIT, Y_EXIT, W_BUTTON, H_BUTTON);

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
