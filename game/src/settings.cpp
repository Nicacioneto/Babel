#include "button.h"
#include "image.h"
#include "rect.h"
#include "resourcesmanager.h"
#include "settings.h"
#include <algorithm>

#define X_BACK 49
#define Y_BACK 480
#define W_BACK_BUTTON 428
#define H_BACK_BUTTON 103
#define X_UP_RESOLUTION 449
#define Y_UP_RESOLUTION 169
#define X_DOWN_RESOLUTION 449
#define Y_DOWN_RESOLUTION 187
#define W_RESOLUTION_BUTTON 21
#define H_RESOLUTION_BUTTON 13
#define MAX_RESOLUTION 1310
#define MIN_RESOLUTION 800
#define SCALE 1.28 // Scale to keep display standart

Settings::Settings(const string& next, const string& image)
    : Level("", next), m_image(nullptr)
{
    env = Environment::get_instance();

    m_image = env->resources_manager->get_image(image);
    m_resolution = env->resources_manager->get_image("res/images/resolutions.png");

    env->canvas->load_font("res/fonts/FLATS.ttf", 14);

    Button *back_button = new Button(this, "back", X_BACK, Y_BACK, W_BACK_BUTTON, H_BACK_BUTTON, Color::TRANSPARENT);
    Button *up_resolution_button = new Button(this, "up_resolution", X_UP_RESOLUTION, Y_UP_RESOLUTION,
        W_RESOLUTION_BUTTON, H_RESOLUTION_BUTTON, Color::TRANSPARENT);
    Button *down_resolution_button = new Button(this, "down_resolution", X_DOWN_RESOLUTION, Y_DOWN_RESOLUTION,
        W_RESOLUTION_BUTTON, H_RESOLUTION_BUTTON, Color::TRANSPARENT);

    back_button->add_observer(this);
    up_resolution_button->add_observer(this);
    down_resolution_button->add_observer(this);

    add_child(back_button);
    add_child(up_resolution_button);
    add_child(down_resolution_button);
}

void
Settings::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());
    int w = env->canvas->w();
    int h = env->canvas->h();

    if (w == 800)
    {
        m_resolution_position = 0;
    }
    else if (w == 1024)
    {
        m_resolution_position = 150;
    }

    string text = std::to_string(w) + "x" + std::to_string(h);
    Color color = {0, 0, 255, 255};
    Rect rect(200, 163, 150, 40);
    env->canvas->draw_message(text, rect, color);

    // double scale = env->canvas->scale();
    // Rect rect(m_resolution_position, 0, 150, m_resolution->h());
    // env->canvas->draw(m_resolution.get(), rect, scale*215, scale*170);
}

// void
// Settings::update_coordinates_buttons()
// {
//     double scale = env->canvas->scale();

//     m_x_back = scale * X_BACK;
//     m_y_back = scale * Y_BACK;
//     m_w_back_button = scale * W_BACK_BUTTON;
//     m_h_back_button = scale * H_BACK_BUTTON;
//     m_x_up_resolution = scale * X_UP_RESOLUTION;
//     m_y_up_resolution = scale * Y_UP_RESOLUTION;
//     m_x_down_resolution = scale * X_DOWN_RESOLUTION;
//     m_y_down_resolution = scale * Y_DOWN_RESOLUTION;
//     m_w_resolution_button = scale * W_RESOLUTION_BUTTON;
//     m_h_resolution_button = scale * H_RESOLUTION_BUTTON;
// }

bool
Settings::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "back")
    {
        m_next = "menu";
        m_done = true;
    }
    else if (button->id() == "up_resolution" or button->id() == "down_resolution")
    {
        int w = env->canvas->w();
        int h;
        int position = std::find(m_resolutions_size.begin(), m_resolutions_size.end(), w) - 
            m_resolutions_size.begin();

        if (button->id() == "up_resolution")
        {
            if (position + 1 < (int) m_resolutions_size.size())
            {
                position++;
                env->canvas->set_scale(SCALE);
            }
        }
        else
        {
            if (position - 1 >= 0)
            {
                position--;
                env->canvas->set_scale(1/SCALE);
            }
        }

        w = m_resolutions_size[position];
        h = w * 3 / 4;
        env->video->set_resolution(w, h);
    }

    return true;
}
