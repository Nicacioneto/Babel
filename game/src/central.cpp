#include "central.h"

Central::Central(const string& next)
    : Level("central", next), m_scenario(nullptr), m_right_bracket(nullptr),
        m_colony(nullptr), m_tower_img(nullptr), m_planet_img(nullptr),
        m_left_bracket(nullptr), m_resources(nullptr), m_center_bracket(nullptr),
        m_tower(nullptr), m_planet(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_scenario = env->resources_manager->get_texture(path + "central/central_chat_scenario.png");
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
Central::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();

    env->canvas->clear();

    env->canvas->draw(m_scenario.get(), 275 * scale, 173 * scale);
    env->canvas->draw(m_right_bracket.get(), 275 * scale, 173 * scale);
    env->canvas->draw(m_colony.get(), 193 * scale, 25 * scale);
    env->canvas->draw(m_tower_img.get(), 28 * scale, 25 * scale);
    env->canvas->draw(m_planet_img.get(), 855 * scale, 25 * scale);
    env->canvas->draw(m_left_bracket.get(), 28 * scale, 175 * scale);
    env->canvas->draw(m_resources.get(), 28 * scale, 120 * scale);
}

bool
Central::on_message(Object *sender, MessageID id, Parameters)
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
        set_next("colony");
        finish();
    }
    else
    {
        if (button->id() == "chat")
        {
            // change_to_chat();
        }
        else if (button->id() == "quests")
        {
            // change_to_quests();
        }
        else if (button->id() == "bestiary")
        {
            // change_to_bestiary();
        }
        else if (button->id() == "timers")
        {
            // change_to_timers();
        }

        if (button->id() != "chat")
        {
            m_buttons["chat"]->change_state(Button::IDLE);
        }
        if (button->id() != "quests")
        {
            m_buttons["quests"]->change_state(Button::IDLE);
        }
        if (button->id() != "bestiary")
        {
            m_buttons["bestiary"]->change_state(Button::IDLE);
        }
        if (button->id() != "timers")
        {
            m_buttons["timers"]->change_state(Button::IDLE);
        }
        
        return false;
    }

    return true;
}

void
Central::create_buttons()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    string path = "res/images/colony/";

    Button *button =  new Button(this, "central", path + "central_button.png",
        28 * scale, 218 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "chat", path + "colony_small_button.png",
        28 * scale, 322 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Chat");
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "quests", path + "colony_small_button.png",
        28 * scale, 427 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Quests");

    m_buttons[button->id()] = button;

    button = new Button(this, "bestiary", path + "colony_small_button.png",
        28 * scale, 531 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Bestiary");

    m_buttons[button->id()] = button;

    button = new Button(this, "timers", path + "colony_small_button.png",
        28 * scale, 635 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Timers");

    m_buttons[button->id()] = button;

    for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
    {
        auto id = it->first;
        m_buttons[id]->add_observer(this);
        add_child(m_buttons[id]);
    }
}
