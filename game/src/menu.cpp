#include "button.h"
#include "menu.h"
#include "resourcesmanager.h"

#define X_BUTTON 119
#define Y_BUTTON 54
#define W_BUTTON 304
#define H_BUTTON 93
#define SPACING 140

Menu::Menu(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_start(nullptr), m_settings(nullptr),
        m_credits(nullptr), m_exit(nullptr)
{
    env = Environment::get_instance();
    m_image = env->resources_manager->get_image(image);

    m_start = new Button(this, "start", "res/images/buttons/start.png",
        X_BUTTON, X_BUTTON, W_BUTTON, H_BUTTON);
    m_settings = new Button(this, "settings", "res/images/buttons/option.png",
        X_BUTTON, Y_BUTTON + SPACING, W_BUTTON, H_BUTTON);
    m_credits = new Button(this, "credits", "res/images/buttons/option.png",
        X_BUTTON, Y_BUTTON + 2*SPACING, W_BUTTON, H_BUTTON);
    m_exit = new Button(this, "exit", "res/images/buttons/quit.png",
        X_BUTTON, Y_BUTTON + 3*SPACING, W_BUTTON, H_BUTTON);

    m_start->add_observer(this);
    m_settings->add_observer(this);
    m_credits->add_observer(this);
    m_exit->add_observer(this);

    add_child(m_start);
    add_child(m_settings);
    add_child(m_credits);
    add_child(m_exit);
}

void
Menu::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    m_start->set_position(scale * X_BUTTON, scale * Y_BUTTON);
    m_settings->set_position(scale * X_BUTTON, scale * (Y_BUTTON + SPACING));
    m_credits->set_position(scale * X_BUTTON, scale * (Y_BUTTON + 2*SPACING));
    m_exit->set_position(scale * X_BUTTON, scale * (Y_BUTTON + 3*SPACING));
}

void
Menu::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());
}

bool
Menu::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "start")
    {
        m_next = "gameover";
    }
    else if (button->id() == "settings")
    {
        m_next = "settings";
    }
    else if (button->id() == "credits")
    {
        m_next = "credits";
    }

    m_done = true;
    return true;
}
