#include "button.h"
#include "menu.h"
#include "resourcesmanager.h"

#define X_START 119
#define Y_START 54
#define X_SETTINGS 119
#define Y_SETTINGS 190
#define X_CREDITS 119
#define Y_CREDITS 335
#define X_EXIT 119
#define Y_EXIT 475
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
        X_START, X_START, W_BUTTON, H_BUTTON);
    m_settings = new Button(this, "settings", "res/images/buttons/option.png",
        X_START, Y_START + SPACING, W_BUTTON, H_BUTTON);
    m_credits = new Button(this, "credits", "res/images/buttons/option.png",
        X_START, Y_START + 2*SPACING, W_BUTTON, H_BUTTON);
    m_exit = new Button(this, "exit", "res/images/buttons/quit.png",
        X_START, Y_START + 3*SPACING, W_BUTTON, H_BUTTON);

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

    m_start->set_position(scale * X_START, scale * Y_START);
    m_start->set_dimensions(scale * W_BUTTON, scale * H_BUTTON);

    m_settings->set_position(scale * X_SETTINGS, scale * Y_SETTINGS);
    m_settings->set_dimensions(scale * W_BUTTON, scale * H_BUTTON);

    m_credits->set_position(scale * X_CREDITS, scale * Y_CREDITS);
    m_credits->set_dimensions(scale * W_BUTTON, scale * H_BUTTON);

    m_exit->set_position(scale * X_EXIT, scale * Y_EXIT);
    m_exit->set_dimensions(scale * W_BUTTON, scale * H_BUTTON);
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
