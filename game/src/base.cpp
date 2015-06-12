#include "base.h"
#include "colony.h"
#include <core/font.h>

#define W 1024.0
#define H 768.0

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
Base::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_scenario.get(), (275 / W) * env->canvas->w(), (173 / H) * env->canvas->h());
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
    else
    {
        set_next(button->id());
    }
    
    finish();
    return true;
}

void
Base::create_buttons()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";

    const int x = (28 / W) * env->canvas->w();
    const int w = (190 / W) * env->canvas->w();
    const int h = (180/3 / H) * env->canvas->h();

    Button *button =  new Button(this, "barracks", path + "barracks_button.png",
        x, (218 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "facilities", path + "facilities_button.png",
        x, (322 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "hospital", path + "hospital_button.png",
        x, (427 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "workshop", path + "workshop_button.png",
        x, (531 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    button = new Button(this, "central", path + "central_button.png",
        x, (635 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);

    m_buttons[button->id()] = button;

    for (auto it : m_buttons)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}
