/*
 * Options class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
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

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button = new Button(this, "up_volume",
        300 * scale_w, 371 * scale_h, 12 * scale_w, 12 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "down_volume",
        300 * scale_w, 384 * scale_h, 12 * scale_w, 12 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "up_resolution",
        652 * scale_w, 371 * scale_h, 12 * scale_w, 12 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "down_resolution",
        652 * scale_w, 384 * scale_h, 12 * scale_w, 12 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "back", "res/images/menu/button.png",
        442 * scale_w, 628 * scale_h,
        140 * scale_w, 60 * scale_h);
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

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["background"].get());

    int x = (env->canvas->w() - m_textures["logo"]->w())/2;
    int y = 25 * scale_h;
    int w, h;
    env->canvas->draw(m_textures["logo"].get(), x, y);
    env->canvas->draw(m_textures["soundvideo"].get(), 189 * scale_w, 321 * scale_h);
    env->canvas->draw(m_textures["arrow"].get(), 300 * scale_w, (H - 20)/2 * scale_h);
    env->canvas->draw(m_textures["arrow"].get(), (W/2 + 140) * scale_w, (H - 20)/2 * scale_h);

    int i, volume = settings->read<int>("Game", "volume", 50)/10;

    for (i = 0; i < (10 - volume)*17; i+=17)
    {
        x = (318 + i) * scale_w;
        y = (H - 15)/2 * scale_h;
        w = 15 * scale_w;
        h = 15 * scale_h;
        env->canvas->draw(m_textures["volume"].get(), Rect(0, 15, 15, 15), x, y, h, w);
    }
    for (int j = i; j < i + volume*17; j+=17)
    {
        x = (318 + j) * scale_w;
        y = (H - 15)/2 * scale_h;
        w = 15 * scale_w;
        h = 15 * scale_h;
        env->canvas->draw(m_textures["volume"].get(), Rect(0, 0, 15, 15), x, y, w, h);
    }

    font->set_size(24);
    set_position(465 * scale_w, 190 * scale_h);
    env->canvas->draw("OPTIONS", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    font->set_size(18);
    set_position(425 * scale_w, (H - 65)/2 * scale_h);
    env->canvas->draw("Volume", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position((W/2 + 20) * scale_w, (H - 65)/2 * scale_h);
    env->canvas->draw("Resolution", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    w = env->canvas->w();
    h = env->canvas->h();
    string text = std::to_string(w) + " x " + std::to_string(h) + " px";
    set_position((W/2 + 25) * scale_w, 372 * scale_h);
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
