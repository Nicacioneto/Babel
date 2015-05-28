#include "colony.h"
#include <core/font.h>

Colony::Colony(const string& next)
    : Level("colony", next), m_colony_cenario(nullptr), m_right_bracket(nullptr),
        m_colony(nullptr), m_tower_img(nullptr), m_planet_img(nullptr),
        m_left_bracket(nullptr), m_resources(nullptr), m_center_bracket(nullptr),
        m_tower(nullptr), m_planet(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_colony_cenario = env->resources_manager->get_texture(path + "colony_scenario.png");
    m_right_bracket = env->resources_manager->get_texture(path + "right_bracket.png");
    m_colony = env->resources_manager->get_texture(path + "colony.png");
    m_tower_img = env->resources_manager->get_texture(path + "tower.png");
    m_planet_img = env->resources_manager->get_texture(path + "planet.png");
    m_left_bracket = env->resources_manager->get_texture(path + "left_bracket.png");
    m_resources = env->resources_manager->get_texture(path + "resources.png");

    double scale = env->canvas->scale();

    m_center_bracket = new Button(this, "center_bracket", path + "center_bracket_button.png",
        193 * scale, 25 * scale, 635 * scale, 156/2 * scale);

    m_tower = new Button(this, "tower", path + "tower_button.png",
        28 * scale, 25 * scale, 140 * scale, 156/2 * scale);

    m_planet = new Button(this, "planet", path + "planet_button.png",
        855 * scale, 25 * scale, 140 * scale, 156/2 * scale);

    m_center_bracket->add_observer(this);
    m_tower->add_observer(this);
    m_planet->add_observer(this);

    add_child(m_center_bracket);
    add_child(m_tower);
    add_child(m_planet);

    create_buttons();
}

void
Colony::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();

    env->canvas->clear();

    env->canvas->draw(m_colony_cenario.get(), 275 * scale, 173 * scale);
    env->canvas->draw(m_right_bracket.get(), 275 * scale, 173 * scale);
    env->canvas->draw(m_colony.get(), 193 * scale, 25 * scale);
    env->canvas->draw(m_tower_img.get(), 28 * scale, 25 * scale);
    env->canvas->draw(m_planet_img.get(), 855 * scale, 25 * scale);
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
        finish();
    }
    else if (button->id() == "planet")
    {
        set_next("planet");
        finish();
    }
    else if (button->id() == "center_bracket")
    {
        change_to_colony();
    }
    else if (button->id() == "hospital")
    {
        change_to_hospital();
        m_buttons[0]->change_state(Button::ACTIVE);
    }
    
    return true;
}

void
Colony::create_buttons()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    string path = "res/images/colony/";

    Button *button =  new Button(this, "barracks", path + "barracks_button.png",
        28 * scale, 218 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons.push_back(button);

    button = new Button(this, "research", path + "research_button.png",
        28 * scale, 322 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons.push_back(button);

    button = new Button(this, "hospital", path + "hospital_button.png",
        28 * scale, 427 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons.push_back(button);

    button = new Button(this, "workshop", path + "workshop_button.png",
        28 * scale, 531 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons.push_back(button);

    button = new Button(this, "central", path + "central_button.png",
        28 * scale, 635 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons.push_back(button);

    for (size_t i = 0; i < m_buttons.size(); ++i)
    {
        m_buttons[i]->add_observer(this);
        add_child(m_buttons[i]);
    }
}

void
Colony::change_to_colony()
{
    string path = "res/images/colony/";

    m_buttons[0]->set_id("barracks");
    m_buttons[0]->remove_text();
    m_buttons[0]->set_texture(path + "barracks_button.png");
    m_buttons[0]->change_state(Button::IDLE);
    m_buttons[1]->set_id("research");
    m_buttons[1]->remove_text();
    m_buttons[1]->set_texture(path + "research_button.png");
    m_buttons[1]->change_state(Button::IDLE);
    m_buttons[2]->set_id("hospital");
    m_buttons[2]->remove_text();
    m_buttons[2]->set_texture(path + "hospital_button.png");
    m_buttons[2]->change_state(Button::IDLE);
    m_buttons[3]->set_id("workshop");
    m_buttons[3]->remove_text();
    m_buttons[3]->set_texture(path + "workshop_button.png");
    m_buttons[3]->change_state(Button::IDLE);
    m_buttons[4]->set_id("central");
    m_buttons[4]->remove_text();
    m_buttons[4]->set_texture(path + "central_button.png");
    m_buttons[4]->change_state(Button::IDLE);

    Environment *env = Environment::get_instance();
    m_colony_cenario = env->resources_manager->get_texture(path + "colony_scenario.png");
}

void
Colony::change_to_hospital()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    string path = "res/images/colony/";

    m_buttons[0]->set_id("hospital");
    m_buttons[0]->set_texture(path + "hospital_button.png");

    for (size_t i = 1; i < m_buttons.size(); ++i)
    {
        m_buttons[i]->set_texture(path + "colony_small_button.png");
    }

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22 * scale);

    m_buttons[1]->set_id("chat");
    m_buttons[1]->set_text("Chat");
    m_buttons[1]->change_state(Button::ACTIVE);
    m_buttons[2]->set_id("items");
    m_buttons[2]->set_text("Items");
    m_buttons[3]->set_id("research");
    m_buttons[3]->set_text("Research");
    m_buttons[4]->set_id("revive");
    m_buttons[4]->set_text("Revive");

    m_colony_cenario = env->resources_manager->get_texture(path + 
        "hospital/hospital_chat_scenario.png");
}
