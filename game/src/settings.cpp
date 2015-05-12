#include "font.h"
#include "image.h"
#include "rect.h"
#include "resourcesmanager.h"
#include "settings.h"
#include <algorithm>

#define X_BACK 150
#define Y_BACK 480
#define W_BACK 304
#define H_BACK 93
#define X_UP_RESOLUTION 449
#define Y_UP_RESOLUTION 169
#define X_DOWN_RESOLUTION 449
#define Y_DOWN_RESOLUTION 187
#define W_RESOLUTION 21
#define H_RESOLUTION 13
#define MAX_RESOLUTION 1310
#define MIN_RESOLUTION 800

Settings::Settings(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_back(nullptr), m_up_resolution(nullptr),
        m_down_resolution(nullptr)
{
    env = Environment::get_instance();

    m_image = env->resources_manager->get_image(image);

    m_back = new Button(this, "back", "res/images/buttons/back.png",
        X_BACK, Y_BACK, W_BACK, H_BACK);
    m_up_resolution = new Button(this, "up_resolution", "",
        X_UP_RESOLUTION, Y_UP_RESOLUTION, W_RESOLUTION, H_RESOLUTION);
    m_down_resolution = new Button(this, "down_resolution", "",
        X_DOWN_RESOLUTION, Y_DOWN_RESOLUTION, W_RESOLUTION, H_RESOLUTION);

    m_back->add_observer(this);
    m_up_resolution->add_observer(this);
    m_down_resolution->add_observer(this);

    add_child(m_back);
    add_child(m_up_resolution);
    add_child(m_down_resolution);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/FLATS.ttf");
    env->canvas->set_font(font);
}

void
Settings::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    m_back->set_position(scale * X_BACK, scale * Y_BACK);

    m_up_resolution->set_position(scale * X_UP_RESOLUTION, scale * Y_UP_RESOLUTION);
    m_up_resolution->set_dimensions(scale * W_RESOLUTION, scale * H_RESOLUTION);

    m_down_resolution->set_position(scale * X_DOWN_RESOLUTION, scale * Y_DOWN_RESOLUTION);
    m_down_resolution->set_dimensions(scale * W_RESOLUTION, scale * H_RESOLUTION);

    shared_ptr<Font> font = env->canvas->font();
    font->set_size(30 * scale);
    set_position(210 * scale, 170 * scale);
}

void
Settings::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());

    int w = env->canvas->w();
    int h = env->canvas->h();
    string text = std::to_string(w) + "x" + std::to_string(h);

    env->canvas->draw(text, bounding_box().x(), bounding_box().y(), Color::BLUE);
}

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
        int position = std::find(m_resolutions.begin(), m_resolutions.end(), w) -
            m_resolutions.begin();

        if (button->id() == "up_resolution")
        {
            if (position + 1 < (int) m_resolutions.size())
            {
                position++;
            }
        }
        else
        {
            if (position - 1 >= 0)
            {
                position--;
            }
        }

        w = m_resolutions[position];
        h = w * 3 / 4;
        env->video->set_resolution(w, h);

        double scale = (double) env->canvas->w() / m_resolutions[m_resolutions.size() - 1];
        env->canvas->set_scale(scale);
    }

    return true;
}
