#include "colony.h"


Colony::Colony(const string& next)
	: Level("colony", next), m_colony_screen(nullptr), m_right_bracket(nullptr),
	m_colony(nullptr), m_center_bracket(nullptr), m_tower(nullptr), m_planet(nullptr),
	m_left_bracket(nullptr), m_resources(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_colony_screen = env->resources_manager->get_texture(path + "colony_screen.png");
    m_right_bracket = env->resources_manager->get_texture(path + "right_bracket.png");
    m_colony = env->resources_manager->get_texture(path + "colony.png");
    m_center_bracket = env->resources_manager->get_texture(path + "center_bracket.png");
    m_tower = env->resources_manager->get_texture(path + "tower.png");
    m_planet = env->resources_manager->get_texture(path + "planet.png");
    m_left_bracket = env->resources_manager->get_texture(path + "left_bracket.png");
    m_resources = env->resources_manager->get_texture(path + "resources.png");
}

void
Colony::draw_self()
{
    Environment *env = Environment::get_instance();
    
    double scale = env->canvas->scale();

    env->canvas->clear();

    env->canvas->draw(m_colony_screen.get(), 275 * scale, 173 * scale);
    env->canvas->draw(m_right_bracket.get(), 275 * scale, 173 * scale);
    env->canvas->draw(m_colony.get(), 193 * scale, 25 * scale);
    env->canvas->draw(m_center_bracket.get(), 193 * scale, 25 * scale);
    env->canvas->draw(m_tower.get(), 28 * scale, 25 * scale);
    env->canvas->draw(m_planet.get(), 855 * scale, 25 * scale);
    env->canvas->draw(m_left_bracket.get(), 28 * scale, 175 * scale);
    env->canvas->draw(m_resources.get(), 28 * scale, 120 * scale);
}
