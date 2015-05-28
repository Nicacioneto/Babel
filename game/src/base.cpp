#include "base.h"
#include "colony.h"
#include <core/font.h>

Base::Base(const string& next)
    : Level("base", next)
{
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
        finish();
        return true;
    }

    if (button->id() == "hospital")
    {
        set_next("hospital");
        finish();
    }
    else if (button->id() == "central")
    {
    }

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
