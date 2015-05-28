#include "base.h"
#include "colony.h"
#include <core/font.h>

Base::Base(const string& next)
    : Level("base", next), m_scenario(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_scenario = env->resources_manager->get_texture(path + "colony_scenario.png");

    Colony *colony = new Colony(this, "base");
    colony->add_observer(this);
    add_child(colony);

    create_buttons();
}

void
Base::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    double scale = env->canvas->scale();
    env->canvas->draw(m_scenario.get(), 275 * scale, 173 * scale);
}

bool
Base::on_message(Object *sender, MessageID id, Parameters)
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
    }
    else if (button->id() == "hospital")
    {
        set_next("hospital");
    }
    else if (button->id() == "central")
    {
        set_next("central");
    }
    
    finish();
    return true;
}

void
Base::create_buttons()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    string path = "res/images/colony/";

    Button *button =  new Button(this, "barracks", path + "barracks_button.png",
        28 * scale, 218 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "research", path + "research_button.png",
        28 * scale, 322 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "hospital", path + "hospital_button.png",
        28 * scale, 427 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "workshop", path + "workshop_button.png",
        28 * scale, 531 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "central", path + "central_button.png",
        28 * scale, 635 * scale, 190 * scale, 180/3* scale);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    for (auto it : m_buttons)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}
