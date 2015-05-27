#include "colony.h"


Colony::Colony(const string& next)
	: Level("colony", next), m_colony(nullptr)
{
    Environment *env = Environment::get_instance();
		
    m_colony = env->resources_manager->get_texture("res/images/colony/colony_screen.png");
}

void
Colony::draw_self()
{
    Environment *env = Environment::get_instance();
    
    double scale = env->canvas->scale();

    env->canvas->clear();

    env->canvas->draw(m_colony.get(), 275 * scale, 173 * scale);
}
