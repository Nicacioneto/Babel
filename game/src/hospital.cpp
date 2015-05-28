#include "colony.h"
#include "hospital.h"

Hospital::Hospital(const string& next)
    : Level("hospital", next), m_scenario(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_scenario = env->resources_manager->get_texture(path + "hospital/hospital_chat_scenario.png");

    Colony *colony = new Colony(this, "hospital");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Hospital::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    double scale = env->canvas->scale();
    env->canvas->draw(m_scenario.get(), 275 * scale, 173 * scale);
}

bool
Hospital::on_message(Object *sender, MessageID id, Parameters)
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
    else if (button->id() != "hospital")
    {
        change_buttons();
        
        if (button->id() == "chat")
        {
            // change_to_chat();
            button->change_state(Button::ACTIVE);
        }
        else if (button->id() == "items")
        {
            // change_to_items();
            button->change_state(Button::ACTIVE);
        }
        else if (button->id() == "research")
        {
            // change_to_research();
            button->change_state(Button::ACTIVE);
        }
        else if (button->id() == "revive")
        {
            // change_to_revive();
            button->change_state(Button::ACTIVE);
        }
    }

    return true;
}

void
Hospital::create_buttons()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    string path = "res/images/colony/";

    Button *button =  new Button(this, "hospital", path + "hospital_button.png",
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

    button = new Button(this, "items", path + "colony_small_button.png",
        28 * scale, 427 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Items");

    m_buttons[button->id()] = button;

    button = new Button(this, "research", path + "colony_small_button.png",
        28 * scale, 531 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Research");

    m_buttons[button->id()] = button;

    button = new Button(this, "revive", path + "colony_small_button.png",
        28 * scale, 635 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);
    button->set_text("Revive");

    m_buttons[button->id()] = button;

    for (auto it : m_buttons)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}

void
Hospital::change_buttons()
{
    for (auto it : m_buttons)
    {
        if (it.first != "hospital")
        {
            it.second->change_state(Button::IDLE);
        }
    }
}