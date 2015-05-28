#include "colony.h"

Colony::Colony(const string& next)
    : Level("colony", next), m_colony_screen(nullptr), m_right_bracket(nullptr),
        m_colony(nullptr), m_center_bracket(nullptr), m_tower(nullptr), m_planet(nullptr),
        m_left_bracket(nullptr), m_resources(nullptr), m_tower_button(nullptr),
        m_planet_button(nullptr)
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

    double scale = env->canvas->scale();

    m_tower_button = new Button(this, "tower", path + "tower_button.png",
        28 * scale, 25 * scale, 140 * scale, 156/2 * scale);

    m_planet_button = new Button(this, "planet", path + "planet_button.png",
        855 * scale, 25 * scale, 140 * scale, 156/2 * scale);

    m_barracks_button = new Button(this, "barracks", path + "barracks_button.png",
        28 * scale, 218 * scale, 190 * scale, 180/3* scale);
    m_barracks_button->set_sprites(3);

    m_research_button = new Button(this, "research", path + "research_button.png",
        28 * scale, 322 * scale, 190 * scale, 180/3* scale);
    m_research_button->set_sprites(3);

    m_hospital_button = new Button(this, "hospital", path + "hospital_button.png",
        28 * scale, 427 * scale, 190 * scale, 180/3* scale);
    m_hospital_button->set_sprites(3);

    m_workshop_button = new Button(this, "workshop", path + "workshop_button.png",
        28 * scale, 531 * scale, 190 * scale, 180/3* scale);
    m_workshop_button->set_sprites(3);

    m_central_button = new Button(this, "central", path + "central_button.png",
        28 * scale, 635 * scale, 190 * scale, 180/3* scale);
    m_central_button->set_sprites(3);

    m_tower_button->add_observer(this);
    m_planet_button->add_observer(this);
    m_barracks_button->add_observer(this);
    m_research_button->add_observer(this);
    m_hospital_button->add_observer(this);
    m_workshop_button->add_observer(this);
    m_central_button->add_observer(this);

    add_child(m_tower_button);
    add_child(m_planet_button);
    add_child(m_barracks_button);
    add_child(m_research_button);
    add_child(m_hospital_button);
    add_child(m_workshop_button);
    add_child(m_central_button);
}

void
Colony::draw_self(double, double)
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
        set_next("dungeon");
    }
    else if (button->id() == "planet")
    {
        set_next("planet");
    }
    else
    {
        if (button->id() != "barracks")
        {
            m_barracks_button->change_state(State::IDLE);
        }
        if (button->id() != "research")
        {
            m_research_button->change_state(State::IDLE);
        }
        if (button->id() != "hospital")
        {
            m_hospital_button->change_state(State::IDLE);
        }
        if (button->id() != "workshop")
        {
            m_workshop_button->change_state(State::IDLE);
        }
        if (button->id() != "central")
        {
            m_central_button->change_state(State::IDLE);
        }

        return false;
    }
    
    finish();

    return true;
}
