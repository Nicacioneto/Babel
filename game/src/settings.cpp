#include "button.h"
#include "environment.h"
#include "image.h"
#include "settings.h"
#include "resourcesmanager.h"


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
    Button back_button(X_BACK, Y_BACK, W_BUTTON, H_BUTTON);

    if (back_button.is_clicked(x, y))
    {
        m_next = "menu";
        m_done = true;
    }
    return false;
}
