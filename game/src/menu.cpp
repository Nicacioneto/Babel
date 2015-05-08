#include "button.h"
#include "image.h"
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
#define W_BUTTON 428
#define H_BUTTON 103

Menu::Menu(const string& next, const string& image)
    : Level("", next), m_image(nullptr)
{
    env = Environment::get_instance();
    m_image = env->resources_manager->get_image(image);

    Button *start_button = new Button(this, "start", X_START, Y_START, W_BUTTON, H_BUTTON, Color::TRANSPARENT);
    Button *settings_button = new Button(this, "settings", X_SETTINGS, Y_SETTINGS, W_BUTTON, H_BUTTON, Color::TRANSPARENT);
    Button *credits_button = new Button(this, "credits", X_CREDITS, Y_CREDITS, W_BUTTON, H_BUTTON, Color::TRANSPARENT);
    Button *exit_button = new Button(this, "exit", X_EXIT, Y_EXIT, W_BUTTON, H_BUTTON, Color::TRANSPARENT);

    start_button->add_observer(this);
    settings_button->add_observer(this);
    credits_button->add_observer(this);
    exit_button->add_observer(this);

    add_child(start_button);
    add_child(settings_button);
    add_child(credits_button);
    add_child(exit_button);
}

void
Menu::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());
}

// void
// Menu::update_coordinates_buttons()
// {
//     double scale = env->canvas->scale();

//     m_x_start = scale * X_START;
//     m_y_start = scale * Y_START;
//     m_x_settings = scale * X_SETTINGS;
//     m_y_settings = scale * Y_SETTINGS;
//     m_x_credits = scale * X_CREDITS;
//     m_y_credits = scale * Y_CREDITS;
//     m_x_exit = scale * X_EXIT;
//     m_y_exit = scale * Y_EXIT;
//     m_w_button = scale * W_BUTTON;
//     m_h_button = scale * H_BUTTON;
// }

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
        // TODO
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