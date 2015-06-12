#include "colony.h"
#include <core/font.h>

#define W 1024.0
#define H 768.0

Colony::Colony(Object *parent, ObjectID id)
    : Object(parent, id), m_right_bracket(nullptr), m_colony(nullptr), m_tower_img(nullptr),
        m_planet_img(nullptr), m_left_bracket(nullptr), m_resources(nullptr),
        m_center_bracket(nullptr), m_tower(nullptr), m_planet(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_right_bracket = env->resources_manager->get_texture(path + "right_bracket.png");
    m_colony = env->resources_manager->get_texture(path + "colony.png");
    m_tower_img = env->resources_manager->get_texture(path + "tower.png");
    m_planet_img = env->resources_manager->get_texture(path + "planet.png");
    m_left_bracket = env->resources_manager->get_texture(path + "left_bracket.png");
    m_resources = env->resources_manager->get_texture(path + "resources.png");

    m_center_bracket = new Button(this, "center_bracket", path + "center_bracket_button.png",
        (193 / W) * env->canvas->w(), (25 / H) * env->canvas->h(),
        (635 / W) * env->canvas->w(), (156/2 / H) * env->canvas->h());
    m_tower = new Button(this, "tower", path + "tower_button.png",
        (28 / W) * env->canvas->w(), (25 / H) * env->canvas->h(),
        (140 / W) * env->canvas->w(), (156/2 / H) * env->canvas->h());
    m_planet = new Button(this, "planet", path + "planet_button.png",
        (855 / W) * env->canvas->w(), (25 / H) * env->canvas->h(),
        (140 / W) * env->canvas->w(), (156/2 / H) * env->canvas->h());

    m_center_bracket->add_observer(this);
    m_tower->add_observer(this);
    m_planet->add_observer(this);

    add_child(m_center_bracket);
    add_child(m_tower);
    add_child(m_planet);
}

void
Colony::draw_self()
{
    Environment *env = Environment::get_instance();

    env->canvas->draw(m_right_bracket.get(), (275 / W) * env->canvas->w(),(173 / H) * env->canvas->h());
    env->canvas->draw(m_colony.get(), (193 / W) * env->canvas->w(),(25 / H) * env->canvas->h());
    env->canvas->draw(m_tower_img.get(), (28 / W) * env->canvas->w(),(25 / H) * env->canvas->h());
    env->canvas->draw(m_planet_img.get(), (855 / W) * env->canvas->w(),(25 / H) * env->canvas->h());
    env->canvas->draw(m_left_bracket.get(), (28 / W) * env->canvas->w(),(175 / H) * env->canvas->h());
    env->canvas->draw(m_resources.get(), (28 / W) * env->canvas->w(),(120 / H) * env->canvas->h());
}

bool
Colony::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "tower")
    {
        notify("dungeon", "");
    }
    else if (button->id() == "planet")
    {
        notify("planet", "");
    }
    else if (button->id() == "center_bracket")
    {
        notify("base", "");
    }

    return true;
}
