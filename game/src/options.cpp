#include "file.h"
#include "options.h"
#include <algorithm>
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

Options::Options(const string& next, const string& texture)
    : Level("options", next), m_texture(nullptr), m_logo(nullptr), m_soundvideo(nullptr),
        m_volume(nullptr), m_arrow(nullptr), m_up_volume(nullptr), m_down_volume(nullptr),
        m_up_resolution(nullptr), m_down_resolution(nullptr), m_back(nullptr)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(texture);
    m_logo = env->resources_manager->get_texture("res/images/menu/babel-logo.png");
    m_soundvideo = env->resources_manager->get_texture("res/images/menu/sound-video.png");
    m_volume = env->resources_manager->get_texture("res/images/menu/volume.png");
    m_arrow = env->resources_manager->get_texture("res/images/menu/arrow.png");

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22);

    int x = (W - 140)/(W * 2) * env->canvas->w();
    int y = (H - 140)/H * env->canvas->h();
    int w = (140/W) * env->canvas->w();
    int h = (60/H) * env->canvas->h();

    m_back = new Button(this, "back", "res/images/menu/button.png", x, y, w, h);
    m_back->set_text("Back");

    m_up_volume = new Button(this, "up_volume", "",
        300, (env->canvas->h() - 25)/2, 12, 12);

    m_down_volume = new Button(this, "down_volume", "",
        300, env->canvas->h()/2, 12, 12);

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
}

void
Options::update_coordinates()
{
    Environment *env = Environment::get_instance();

    int x = (W - 140)/(W * 2) * env->canvas->w();
    int y = (H - 149)/H * env->canvas->h();
    int w = (140/W) * env->canvas->w();
    int h = (60/H) * env->canvas->h();
    m_back->set_position(x, y);
    m_back->set_dimensions(w, h);
    m_back->set_text("Back");

    x = (300)/W * env->canvas->w();
    y = (H - 25)/(H * 2) * env->canvas->h();
    w = (12/W) * env->canvas->w();
    h = (12/H) * env->canvas->h();
    m_up_volume->set_position(x, y);
    m_up_volume->set_dimensions(w, h);

    y = env->canvas->h()/2;
    m_down_volume->set_position(x, y);
    m_down_volume->set_dimensions(w, h);

    x = (W/2 + 140)/W * env->canvas->w();
    y = (H - 25)/(H * 2) * env->canvas->h();
    w = (12/W) * env->canvas->w();
    h = (12/H) * env->canvas->h();
    m_up_resolution->set_position(x, y);
    m_up_resolution->set_dimensions(w, h);

    y = (env->canvas->h())/2;
    m_down_resolution->set_position(x, y);
    m_down_resolution->set_dimensions(w, h);
}

void
Options::draw_self()
{
    update_coordinates(); // TO FIX

    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->canvas->font();
    shared_ptr<Settings> settings = env->resources_manager->get_settings(env->m_settings_path);

    env->canvas->clear();
    env->canvas->draw(m_texture.get());

    int x = (env->canvas->w() - m_logo->w())/2;
    int y = 25/H * env->canvas->h();
    int w, h;
    env->canvas->draw(m_logo.get(), x, y);

    x = 189/W * env->canvas->w();
    y = 321/H * env->canvas->h();
    env->canvas->draw(m_soundvideo.get(), x, y);

    x = 300/W * env->canvas->w();
    y = (H - 20)/(H * 2) * env->canvas->h();
    env->canvas->draw(m_arrow.get(), x, y);
    
    x = (W/2 + 140)/W * env->canvas->w();
    env->canvas->draw(m_arrow.get(), x, y);

    int i, volume = settings->read<int>("Game", "volume", 50)/10;

    for (i = 0; i < (10 - volume)*17; i+=17)
    {
        x = (318 + i)/W * env->canvas->w();
        y = (H - 15)/(H*2) * env->canvas->h();
        h = 15/W * env->canvas->w();
        w = h;
        env->canvas->draw(m_volume.get(), Rect(0, 15, 15, 15), x, y, h, w);
    }
    for (int j = i; j < i + volume*17; j+=17)
    {
        x = (318 + j)/W * env->canvas->w();
        y = (H - 15)/(H*2) * env->canvas->h();
        h = 15/W * env->canvas->w();
        w = h;
        env->canvas->draw(m_volume.get(), Rect(0, 0, 15, 15), x, y, w, h);
    }

    x = 465/W * env->canvas->w();
    y = 190/H * env->canvas->h();

    font->set_size(24);
    set_position(x, y);
    env->canvas->draw("OPTIONS", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    x = 425/W * env->canvas->w();
    y = (H - 65)/(H*2) * env->canvas->h();

    font->set_size(18);
    set_position(x, y);
    env->canvas->draw("Volume", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    x = (W/2 + 20)/W * env->canvas->w();
    set_position(x, y);
    env->canvas->draw("Resolution", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    x += 5;
    y = 372/H * env->canvas->h();

    w = env->canvas->w();
    h = env->canvas->h();
    string text = std::to_string(w) + " x " + std::to_string(h) + " px";
    set_position(x, y);
    env->canvas->draw(text, bounding_box().x(), bounding_box().y(), Color(170, 215, 190));
}

bool
Options::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings(env->m_settings_path);

    if (button->id() == "back")
    {
        finish();
    }
    else if (button->id() == "up_resolution" or button->id() == "down_resolution")
    {
        int w = env->canvas->w();
        int h;
        int position = std::find(m_resolutions.begin(), m_resolutions.end(), w) -
            m_resolutions.begin();

        if (button->id() == "up_resolution")
        {
            if (++position == (int) m_resolutions.size())
            {
                position = m_resolutions.size() - 1;
            }
        }
        else
        {
            if (--position < 0)
            {
                position = 0;
            }
        }

        w = m_resolutions[position];
        h = w * 3 / 4;

        double scale = (double) w / m_resolutions[m_resolutions.size() - 1];
        env->video->set_resolution(w, h, scale);
        update_coordinates();

        settings->write<int>("Game", "w", w);
        settings->write<int>("Game", "h", h);
        settings->write<double>("Game", "scale", scale);
        settings->save(env->m_settings_path);
    }
    else if (button->id() == "up_volume" or button->id() == "down_volume")
    {
        int volume = settings->read<int>("Game", "volume", 50)/10;

        if (button->id() == "up_volume")
        {
            if (++volume > 10)
            {
                volume = 10;
            }
        }
        else
        {
            if (--volume < 0)
            {
                volume = 0;
            }
        }

        env->music->set_volume(volume*10);
        env->sfx->set_volume(volume*10);

        settings->write<int>("Game", "volume", volume*10);
        settings->save(env->m_settings_path);
    }

    return true;
}
