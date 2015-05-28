#include "central.h"
#include "colony.h"

Central::Central(const string& next)
    : Level("central", next), m_scenario(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_scenario = env->resources_manager->get_texture(path + "central/central_chat_scenario.png");

    Colony *colony = new Colony(this, "central");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Central::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    double scale = env->canvas->scale();
    env->canvas->draw(m_scenario.get(), 275 * scale, 173 * scale);
}

bool
Central::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        Colony *colony = dynamic_cast<Colony *>(sender);
        if (not colony)
        {
            return false;
        }

        set_next(id);
        finish();
    }
    else if (button->id() != "central")
    {
        change_buttons();

        if (button->id() == "chat")
        {
            // change_to_chat();
            button->change_state(Button::ACTIVE);
        }
        else if (button->id() == "quests")
        {
            // change_to_quests();
            button->change_state(Button::ACTIVE);
        }
        else if (button->id() == "bestiary")
        {
            // change_to_bestiary();
            button->change_state(Button::ACTIVE);
        }
        else if (button->id() == "timers")
        {
            // change_to_timers();
            button->change_state(Button::ACTIVE);
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

void
Central::change_buttons()
{
    for (auto it : m_buttons)
    {
        if (it.first != "central")
        {
            it.second->change_state(Button::IDLE);
        }
    }
}
