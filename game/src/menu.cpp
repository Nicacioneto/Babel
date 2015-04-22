#include "image.h"
#include "environment.h"
#include "menu.h"
#include "resourcesmanager.h"

Menu::Menu(const string& next, const string& image)
    : Level("", next), m_image(nullptr)
{
    Environment *env = Environment::get_instance();
    shared_ptr<Resource> r = env->resources_manager->get(Resource::IMAGE,
        image);
    m_image = dynamic_cast<Image *>(r.get());

    // cout << m_image.w() << endl;

    m_x = 0;
    m_y = 0;
}

void
Menu::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    env->canvas->draw(m_image, m_x, m_y);
}
