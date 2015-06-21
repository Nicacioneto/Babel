#include "file.h"
#include "options.h"
#include <algorithm>
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

Options::Options(const string& next)
    : Level("options", next)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/menu/";
    m_textures["background"] = env->resources_manager->get_texture(path + "init-screen.png");
    m_textures["logo"] = env->resources_manager->get_texture(path + "babel-logo.png");
    m_textures["soundvideo"] = env->resources_manager->get_texture(path + "sound-video.png");
    m_textures["volume"] = env->resources_manager->get_texture(path + "volume.png");
    m_textures["arrow"] = env->resources_manager->get_texture(path + "arrow.png");

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22);

    Button *button = new Button(this, "up_volume", "",
        (300/W) * env->canvas->w(), (371/H) * env->canvas->h(), 12, 12);
    m_buttons[button->id()] = button;

    button = new Button(this, "down_volume", "",
        (300/W) * env->canvas->w(), (384/H) * env->canvas->h(), 12, 12);
    m_buttons[button->id()] = button;

    button = new Button(this, "up_resolution", "",
        (652/W) * env->canvas->w(), (371/H) * env->canvas->h(), 12, 12);
    m_buttons[button->id()] = button;

    button = new Button(this, "down_resolution", "",
        (652/W) * env->canvas->w(), (384/H) * env->canvas->h(), 12, 12);
    m_buttons[button->id()] = button;

    button = new Button(this, "back", "res/images/menu/button.png",
        (442/W) * env->canvas->w(), (628/H) * env->canvas->h(),
        (140/W) * env->canvas->w(), (60/H) * env->canvas->h());
    button->set_text("Back");
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}

void
Options::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    shared_ptr<Font> font = env->canvas->font();
    shared_ptr<Settings> settings = env->resources_manager->get_settings(env->m_settings_path);

    env->canvas->draw(m_textures["background"].get());

    int x = (env->canvas->w() - m_textures["logo"]->w())/2;
    int y = 25/H * env->canvas->h();
    int w, h;
    env->canvas->draw(m_textures["logo"].get(), x, y);

    x = 189/W * env->canvas->w();
    y = 321/H * env->canvas->h();
    env->canvas->draw(m_textures["soundvideo"].get(), x, y);

    x = 300/W * env->canvas->w();
    y = (H - 20)/(H * 2) * env->canvas->h();
    env->canvas->draw(m_textures["arrow"].get(), x, y);
    
    x = (W/2 + 140)/W * env->canvas->w();
    env->canvas->draw(m_textures["arrow"].get(), x, y);

    int i, volume = settings->read<int>("Game", "volume", 50)/10;

    for (i = 0; i < (10 - volume)*17; i+=17)
    {
        x = (318 + i)/W * env->canvas->w();
        y = (H - 15)/(H*2) * env->canvas->h();
        h = 15/W * env->canvas->w();
        w = h;
        env->canvas->draw(m_textures["volume"].get(), Rect(0, 15, 15, 15), x, y, h, w);
    }
    for (int j = i; j < i + volume*17; j+=17)
    {
        x = (318 + j)/W * env->canvas->w();
        y = (H - 15)/(H*2) * env->canvas->h();
        h = 15/W * env->canvas->w();
        w = h;
        env->canvas->draw(m_textures["volume"].get(), Rect(0, 0, 15, 15), x, y, w, h);
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

        settings->write<int>("Game", "w", w);
        settings->write<int>("Game", "h", h);
        settings->write<double>("Game", "scale", scale);
        settings->save(env->m_settings_path);

        set_next("options");
        finish();
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
