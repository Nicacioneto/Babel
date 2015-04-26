#include "button.h"
#include "environment.h"
#include "image.h"
#include "settings.h"
#include "resourcesmanager.h"


Settings::Settings(const string& next, const string& image)
    : Level("", next), m_image(nullptr)
{
    env = Environment::get_instance();
    shared_ptr<Resource> r = env->resources_manager->get(Resource::IMAGE,
        image);
    m_image = dynamic_cast<Image *>(r.get());
}

void
Settings::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image, 0, 0);
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
        w = env->video->resolution().first;
        h = env->video->resolution().second;
        
        if(w < MAX_RESOLUTION)
        {
            w *= SCALE;
            h *= SCALE;   
            env->video->set_resolution(w, h);
        }

    }
    else if (down_resolution_button.is_clicked(x, y))
    {
        int w, h;
        w = env->video->resolution().first;
        h = env->video->resolution().second;
        
        if(w > MIN_RESOLUTION)
        {
            w /= SCALE;
            h /= SCALE;
            env->video->set_resolution(w, h);
        }
    }

    return false;
}
