/*
 * Colony class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "colony.h"
#include <core/font.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Colony::Colony(int slot, Object *parent, ObjectID id)
    : Object(parent, id), m_slot(slot), m_data(0), m_matter(0), m_energy(0), m_settings(nullptr)
{
    Environment *env = Environment::get_instance();

    string path = "res/images/colony/";
    m_textures["right_bracket"] = env->resources_manager->get_texture(path + "right_bracket.png");
    m_textures["colony"] = env->resources_manager->get_texture(path + "colony.png");
    m_textures["tower"] = env->resources_manager->get_texture(path + "tower.png");
    m_textures["planet"] = env->resources_manager->get_texture(path + "planet.png");
    m_textures["left_bracket"] = env->resources_manager->get_texture(path + "left_bracket.png");
    m_textures["resources"] = env->resources_manager->get_texture(path + "resources.png");

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button = new Button(this, "tower", path + "tower_button.png",
        28 * scale_w, 25 * scale_h, 220 * scale_w, 154/2 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "center_bracket", path + "center_bracket_button.png",
        277 * scale_w, 25 * scale_h, 467 * scale_w, 154/2 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "planet", path + "planet_button.png",
        770 * scale_w, 25 * scale_h, 223 * scale_w, 154/2 * scale_h);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    path = "res/datas/slot" + to_string(m_slot) + "/colony.sav";
    m_settings = env->resources_manager->get_settings(path);
    m_data = m_settings->read<int>("Colony", "data", 0);
    m_matter = m_settings->read<int>("Colony", "matter", 0);
    m_energy = m_settings->read<int>("Colony", "energy", 0);
    m_mwaked = m_settings->read<int>("Colony", "military", 0);
    m_pwaked = m_settings->read<int>("Colony", "psionic", 0);
    m_twaked = m_settings->read<int>("Colony", "tech", 0);
}

void
Colony::draw_self()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["right_bracket"].get(), 275 * scale_w, 173 * scale_h);
    env->canvas->draw(m_textures["colony"].get(), 277 * scale_w, 25 * scale_h);
    env->canvas->draw(m_textures["tower"].get(), 28 * scale_w, 25 * scale_h);
    env->canvas->draw(m_textures["planet"].get(), 770 * scale_w, 25 * scale_h);
    env->canvas->draw(m_textures["left_bracket"].get(), 28 * scale_w, 175 * scale_h);
    env->canvas->draw(m_textures["resources"].get(), 28 * scale_w, 120 * scale_h);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    font->set_size(16);

    Color color(170, 215, 190);
    int x = 120;
    int y = 123 * scale_h;

    env->canvas->draw(to_string(m_data), x * scale_w, y, color);
    env->canvas->draw(to_string(m_matter), (x + 170) * scale_w, y, color);
    env->canvas->draw(to_string(m_energy), (x + 160*2) * scale_w, y, color);

    y -= 3;
    env->canvas->draw(to_string(m_mwaked), (x + 646) * scale_w, y, color);
    env->canvas->draw(to_string(m_pwaked), (x + 703) * scale_w, y, color);
    env->canvas->draw(to_string(m_twaked), (x + 753) * scale_w, y, color);
}

bool
Colony::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "tower")
    {
        notify("tower", "");
    }
    else if (button->id() == "planet")
    {
        notify("planet", "");
    }
    else if (button->id() == "center_bracket")
    {
        notify("base", "");
    }

    return true;
}

int
Colony::data() const
{
    return m_data;
}

int
Colony::matter() const
{
    return m_matter;
}

int
Colony::energy() const
{
    return m_energy;
}

int
Colony::mwaked() const
{
    return m_mwaked;
}

int
Colony::pwaked() const
{
    return m_pwaked;
}

int
Colony::twaked() const
{
    return m_twaked;
}

void
Colony::set_data(int data)
{
    m_data = data;
    write<int>("data", data);
}

void
Colony::set_matter(int matter)
{
    m_matter = matter;
    write<int>("matter", matter);
}

void
Colony::set_energy(int energy)
{
    m_energy = energy;
    write<int>("energy", energy);
}

void
Colony::set_mwaked(int mwaked)
{
    m_mwaked = mwaked;
    write<int>("military", mwaked);
}

void
Colony::set_pwaked(int pwaked)
{
    m_pwaked = pwaked;
    write<int>("psionic", pwaked);
}

void
Colony::set_twaked(int twaked)
{
    m_twaked = twaked;
    write<int>("tech", twaked);
}

template<typename T> void
Colony::write(const string& attr, const T& value)
{
    m_settings->write<int>("Colony", attr, value);
    m_settings->save("res/datas/slot" + to_string(m_slot) + "/colony.sav");
}
