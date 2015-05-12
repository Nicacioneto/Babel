#include "font.h"
#include "image.h"
#include "rect.h"
#include "resourcesmanager.h"
#include "settings.h"
#include <algorithm>

#define W_BUTTON 140
#define H_BUTTON 60

Settings::Settings(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_logo(nullptr), m_soundvideo(nullptr),
        m_volume(nullptr), m_arrow(nullptr), m_up_volume(nullptr), m_down_volume(nullptr),
        m_up_resolution(nullptr), m_down_resolution(nullptr), m_back(nullptr)
{
    env = Environment::get_instance();
    m_image = env->resources_manager->get_image(image);
    m_logo = env->resources_manager->get_image("res/images/babel_logo.png");
    m_soundvideo = env->resources_manager->get_image("res/images/settings/sound-video.png");
    m_volume = env->resources_manager->get_image("res/images/settings/volume.png");
    m_arrow = env->resources_manager->get_image("res/images/settings/arrow.png");

    m_back = new Button(this, "back", "res/images/menu/button.png",
        (env->canvas->w() - W_BUTTON)/2, env->canvas->h() - 149, W_BUTTON, H_BUTTON);

    m_up_volume = new Button(this, "up_volume", "",
        296, (env->canvas->h() - 25)/2, 12, 12);

    m_down_volume = new Button(this, "down_volume", "",
        296, env->canvas->h()/2, 12, 12);

    m_up_resolution = new Button(this, "up_resolution", "",
        env->canvas->w()/2 + 140, (env->canvas->h() - 25)/2, 12, 12);

    m_down_resolution = new Button(this, "down_resolution", "",
        env->canvas->w()/2 + 140, (env->canvas->h())/2, 12, 12);

    m_back->add_observer(this);
    m_up_volume->add_observer(this);
    m_down_volume->add_observer(this);
    m_up_resolution->add_observer(this);
    m_down_resolution->add_observer(this);

    add_child(m_back);
    add_child(m_up_volume);
    add_child(m_down_volume);
    add_child(m_up_resolution);
    add_child(m_down_resolution);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
}

void
Settings::update_self(unsigned long)
{
    double scale = env->canvas->scale();
    m_back->set_position((env->canvas->w() - W_BUTTON * scale)/2, env->canvas->h() - 149.0 * scale);
    m_up_volume->set_position(296 * scale, (env->canvas->h() - 25 * scale)/2);
    m_down_volume->set_position(296 * scale, env->canvas->h()/2);
    m_up_resolution->set_position(env->canvas->w()/2 + 140 * scale, (env->canvas->h() - 25*scale)/2);
    m_down_resolution->set_position(env->canvas->w()/2 + 140 * scale, (env->canvas->h())/2);
}

void
Settings::draw_self()
{
    double scale = env->canvas->scale();
    shared_ptr<Font> font = env->canvas->font();

    env->canvas->clear();
    env->canvas->draw(m_image.get());
    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2, 25 * scale);
    env->canvas->draw(m_soundvideo.get(), 189 * scale, 321 * scale);
    env->canvas->draw(m_arrow.get(), 296 * scale, (env->canvas->h() - 20 * scale)/2);
    env->canvas->draw(m_arrow.get(), env->canvas->w()/2 + 140 * scale, (env->canvas->h() - 20 * scale)/2);

    for (int i = 0; i < 5*17; i+=17)
    {
        env->canvas->draw(m_volume.get(), Rect(0, 15, 15, 15), (313+i) * scale, (env->canvas->h() - 15 * scale)/2);
    }
    for (int i = 85; i < 85 + 5*17; i+=17)
    {
        env->canvas->draw(m_volume.get(), Rect(0, 0, 15, 15), (313+i) * scale, (env->canvas->h() - 15 * scale)/2);
    }

    font->set_size(24 * scale);
    set_position((env->canvas->w() - W_BUTTON)/2 + 23 * scale, 167 * scale);
    env->canvas->draw("OPTIONS", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    font->set_size(18 * scale);
    set_position((bounding_box().y() + 250) * scale, env->canvas->h()/2 - 37 * scale);
    env->canvas->draw("Volume", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(env->canvas->w()/2 + 20 * scale, env->canvas->h()/2 - 37 * scale);
    env->canvas->draw("Resolution", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    int w = env->canvas->w();
    int h = env->canvas->h();
    string text = std::to_string(w) + " x " + std::to_string(h) + " px";
    set_position(env->canvas->w()/2 + 25 * scale, (env->canvas->h() - 25 * scale)/2);
    env->canvas->draw(text, bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    font->set_size(22 * scale);
    set_position(m_back->x() + 48 * scale, m_back->y() + 15 * scale);
    env->canvas->draw("Back", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));
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
