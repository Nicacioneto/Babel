#include "button.h"
#include "environment.h"
#include "image.h"
#include "settings.h"
#include "resourcesmanager.h"
#include <iostream>


Settings::Settings(const string& next, const string& image)
    : Level("", next), m_image(nullptr)
{
    m_env = Environment::get_instance();
    shared_ptr<Resource> r = m_env->resources_manager->get(Resource::IMAGE,
        image);
    m_image = dynamic_cast<Image *>(r.get());
}

void
Settings::draw_self()
{
    m_env->canvas->clear();
    m_env->canvas->draw(m_image, 0, 0);
}

bool
Settings::execute_action(const int x, const int y)
{
    Button back_button(X_BACK, Y_BACK, W_BACK_BUTTON, H_BACK_BUTTON);
    Button up_resolution_button(X_UP_RESOLUTION, Y_UP_RESOLUTION, W_RESOLUTION_BUTTON, H_RESOLUTION_BUTTON);
    Button down_resolution_button(X_DOWN_RESOLUTION, Y_DOWN_RESOLUTION, W_RESOLUTION_BUTTON, H_RESOLUTION_BUTTON);

    if (back_button.is_clicked(x, y))
    {
        m_next = "menu";
        m_done = true;
    }
    else if (up_resolution_button.is_clicked(x, y))
    {
        int w, h;
        w = m_env->video->resolution().first;
        h = m_env->video->resolution().second;
        
        if(w < 1310)
        {
            w *= 1.28;
            h *= 1.28;   
            m_env->video->set_resolution(w, h);
        }

    }
    else if (down_resolution_button.is_clicked(x, y))
    {
        int w, h;
        w = m_env->video->resolution().first;
        h = m_env->video->resolution().second;
        
        if(w > 625)
        {
            w /= 1.28;
            h /= 1.28;   
            m_env->video->set_resolution(w, h);
        }
    }

    return false;
}
