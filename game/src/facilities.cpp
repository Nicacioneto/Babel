/*
 * Facilities class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "facilities.h"
#include <core/font.h>
#include <core/line.h>
#include <core/point.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Facilities::Facilities(int slot, const string& next)
    : Level("facilities", next), m_slot(slot), m_matter_cost(10),
        m_energy_cost(10), m_screen(CHAT)
{
    m_colony = new Colony(slot, this, "facilities");
    m_colony->add_observer(this);
    add_child(m_colony);

    create_buttons();
}

void
Facilities::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    switch (m_screen)
    {
        case CHAT:
            change_to_chat();
            break;

        case MILITARY:
            change_to_military();
            break;

        case PSIONIC:
            change_to_psionic();
            break;

        case TECH:
            change_to_tech();
            break;
    }
}

bool
Facilities::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        Colony *colony = dynamic_cast<Colony *>(sender);
        if (not colony)
        {
            return false;
        }

        set_next(id);
        finish();
    }
    else if (button->id() == "mwake")
    {
        int matter = m_colony->matter();
        int energy = m_colony->energy();
        int mwaked = m_colony->mwaked();

        if (matter < m_matter_cost || energy < m_energy_cost)
        {
            return true;
        }

        if (++mwaked > 100)
        {
            mwaked = 100;
        }
        else
        {
            matter -= m_matter_cost;
            energy -= m_energy_cost;
        }

        m_colony->set_matter(matter);
        m_colony->set_energy(energy);
        m_colony->set_mwaked(mwaked);
    }
    else if (button->id() == "pwake")
    {
        int matter = m_colony->matter();
        int energy = m_colony->energy();
        int pwaked = m_colony->pwaked();

        if (matter < m_matter_cost || energy < m_energy_cost)
        {
            return true;
        }

        if (++pwaked > 100)
        {
            pwaked = 100;
        }
        else
        {
            matter -= m_matter_cost;
            energy -= m_energy_cost;
        }

        m_colony->set_matter(matter);
        m_colony->set_energy(energy);
        m_colony->set_pwaked(pwaked);
    }
    else if (button->id() == "twake")
    {
        int matter = m_colony->matter();
        int energy = m_colony->energy();
        int twaked = m_colony->twaked();

        if (matter < m_matter_cost || energy < m_energy_cost)
        {
            return true;
        }

        if (++twaked > 100)
        {
            twaked = 100;
        }
        else
        {
            matter -= m_matter_cost;
            energy -= m_energy_cost;
        }

        m_colony->set_matter(matter);
        m_colony->set_energy(energy);
        m_colony->set_twaked(twaked);
    }
    else if (button->id() != "facilities")
    {
        change_buttons();

        if (button->id() == "chat")
        {
            m_screen = CHAT;
        }
        else if (button->id() == "military")
        {
            m_screen = MILITARY;
        }
        else if (button->id() == "psionic")
        {
            m_screen = PSIONIC;
        }
        else if (button->id() == "tech")
        {
            m_screen = TECH;
        }

        button->change_state(Button::ACTIVE);
    }

    return true;
}

void
Facilities::create_buttons()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/";

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(24);

    const int x = (28 / W) * env->canvas->w();
    const int w = (190 / W) * env->canvas->w();
    const int h = (180/3 / H) * env->canvas->h();

    Button *button =  new Button(this, "facilities", path + "facilities_button.png",
        x, (218 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "chat", path + "colony_small_button.png",
        x, (322 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Chat");
    button->change_state(Button::ACTIVE);

    m_buttons[button->id()] = button;

    button = new Button(this, "military", path + "colony_small_button.png",
        x, (427 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Military");

    m_buttons[button->id()] = button;

    button = new Button(this, "psionic", path + "colony_small_button.png",
        x, (531 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Psionic");

    m_buttons[button->id()] = button;

    button = new Button(this, "tech", path + "colony_small_button.png",
        x, (635 / H) * env->canvas->h(), w, h);
    button->set_sprites(3);
    button->set_text("Technologic");

    m_buttons[button->id()] = button;

    button = new Button(this, "mwake", "", (315/W) * env->canvas->w(), (636 / H) * env->canvas->h(),
        60, 60);
    button->set_active(false);

    m_buttons[button->id()] = button;

    button = new Button(this, "pwake", "", (315/W) * env->canvas->w(), (636 / H) * env->canvas->h(),
        60, 60);
    button->set_active(false);

    m_buttons[button->id()] = button;

    button = new Button(this, "twake", "", (315/W) * env->canvas->w(), (636 / H) * env->canvas->h(),
        60, 60);
    button->set_active(false);

    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}

void
Facilities::change_buttons()
{
    for (auto b : m_buttons)
    {
        if (b.first != "facilities" and
            b.first != "mwake" and
            b.first != "pwake" and
            b.first != "twake")
        {
            b.second->change_state(Button::IDLE);
        }
    }
}

void
Facilities::change_to_chat()
{
    Environment *env = Environment::get_instance();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(18);
    Color color(170, 215, 190);
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");
    map< string, map<string, string> > sections = settings->sections();
    string text = sections["Facilities"]["welcome"];
    env->canvas->draw(text, ((305+5) / W) * env->canvas->w(), (605 / H) * env->canvas->h(), color);
    env->canvas->draw(Rect((305 / W) * env->canvas->w(), (605 / H) * env->canvas->h(),
        (670 / W) * env->canvas->w(), (116 / H) * env->canvas->h()), color);
}

void
Facilities::change_to_military()
{
    m_buttons["mwake"]->set_active(true);
    m_buttons["pwake"]->set_active(false);
    m_buttons["twake"]->set_active(false);

    Environment *env = Environment::get_instance();
    string path = "res/images/colony/facilities/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    shared_ptr<Texture> texture;

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    int mwaked = m_colony->mwaked();

    for (int i = 5, y = 235; i > 0; --i, y += 77)
    {
        string red = "red_";
        if (mwaked / 19 >= i)
        {
            red = "";
        }

        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), 384 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), 462 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "pistol.png");
        env->canvas->draw(texture.get(), 540 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "sniper.png");
        env->canvas->draw(texture.get(), 618 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), 696 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), 774 * scale_w, (y+25) * scale_h);

        Point a(360 * scale_w, (y-14+25) * scale_h);
        Point b(851 * scale_w, (y-14+25) * scale_h);
        env->canvas->draw(Line(a, b), color);
    }

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");

    Rect rect(333 * scale_w, 222 * scale_h,
        25 * scale_w, 416 * scale_h);
    env->canvas->draw(rect, color);

    texture = env->resources_manager->get_texture(path + "wake-m.png");
    env->canvas->draw(texture.get(), 308 * scale_w, 628 * scale_h);

    int x = 340;
    int y = (629 - 4 * mwaked);

    rect = Rect(x * scale_w, y * scale_h,
        10 * scale_w, 4 * (mwaked+2) * scale_h);
    env->canvas->fill(rect, Color(206, 178, 46));

    y -= 13;
    texture = env->resources_manager->get_texture(path + "meter-m.png");
    env->canvas->draw(texture.get(), 290 * scale_w, y * scale_h);

    env->canvas->draw(std::to_string(mwaked), 294 * scale_w, (y + 1) * scale_h, color);

    path = "res/images/colony/icons/";

    if (m_colony->matter() >= m_matter_cost)
    {
        y = 0;
        color = Color(170, 215, 190);
    }
    else
    {
        y = 20;
        color = Color(145, 6, 6);
    }

    Rect clip = Rect(0, y, 24, 20);
    texture = env->resources_manager->get_texture(path + "matter.png");
    env->canvas->draw(texture.get(), clip, 385 * scale_w, 670 * scale_h, 24, 20);

    if (m_colony->energy() >= m_energy_cost)
    {
        y = 0;
        color = Color(170, 215, 190);
    }
    else
    {
        y = 18;
        color = Color(145, 6, 6);
    }

    clip = Rect(0, y, 11, 18);
    texture = env->resources_manager->get_texture(path + "energy.png");
    env->canvas->draw(texture.get(), clip, 478 * scale_w, 670 * scale_h, 11, 18);

    env->canvas->draw(std::to_string(m_matter_cost), 425 * scale_w, 670 * scale_h, color);
    env->canvas->draw(std::to_string(m_energy_cost), 510 * scale_w, 670 * scale_h, color);
}

void
Facilities::change_to_psionic()
{
    m_buttons["mwake"]->set_active(false);
    m_buttons["pwake"]->set_active(true);
    m_buttons["twake"]->set_active(false);

    Environment *env = Environment::get_instance();
    string path = "res/images/colony/facilities/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    shared_ptr<Texture> texture;

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    int pwaked = m_colony->pwaked();

    for (int i = 5, y = 235; i > 0; --i, y += 77)
    {
        string red = "red_";
        if (pwaked / 19 >= i)
        {
            red = "";
        }

        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), 384 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), 462 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "pistol.png");
        env->canvas->draw(texture.get(), 540 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "sniper.png");
        env->canvas->draw(texture.get(), 618 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), 696 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), 774 * scale_w, (y+25) * scale_h);

        Point a(360 * scale_w, (y-14+25) * scale_h);
        Point b(851 * scale_w, (y-14+25) * scale_h);
        env->canvas->draw(Line(a, b), color);
    }

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");

    Rect rect(333 * scale_w, 222 * scale_h, 25 * scale_w, 416 * scale_h);
    env->canvas->draw(rect, color);

    texture = env->resources_manager->get_texture(path + "wake-p.png");
    env->canvas->draw(texture.get(), 308 * scale_w, 628 * scale_h);

    int x = 340;
    int y = (629 - 4 * pwaked);

    rect = Rect(x * scale_w, y * scale_h, 10 * scale_w, 4*(pwaked+2) * scale_h);
    env->canvas->fill(rect, Color(146, 61, 133));

    y -= 13;
    texture = env->resources_manager->get_texture(path + "meter-p.png");
    env->canvas->draw(texture.get(), 290 * scale_w, y * scale_h);

    env->canvas->draw(std::to_string(pwaked), (290+4) * scale_w,
        (y + 1) * scale_h, color);

    path = "res/images/colony/icons/";

    if (m_colony->matter() >= m_matter_cost)
    {
        y = 0;
        color = Color(170, 215, 190);
    }
    else
    {
        y = 20;
        color = Color(145, 6, 6);
    }

    Rect clip = Rect(0, y, 24, 20);
    texture = env->resources_manager->get_texture(path + "matter.png");
    env->canvas->draw(texture.get(), clip, 385 * scale_w, 670 * scale_h, 24, 20);

    if (m_colony->energy() >= m_energy_cost)
    {
        y = 0;
        color = Color(170, 215, 190);
    }
    else
    {
        y = 18;
        color = Color(145, 6, 6);
    }

    clip = Rect(0, y, 11, 18);
    texture = env->resources_manager->get_texture(path + "energy.png");
    env->canvas->draw(texture.get(), clip, 478 * scale_w, 670 * scale_h, 11, 18);

    env->canvas->draw(std::to_string(m_matter_cost), 425 * scale_w, 670 * scale_h, color);
    env->canvas->draw(std::to_string(m_energy_cost), 510 * scale_w, 670 * scale_h, color);
}

void
Facilities::change_to_tech()
{
    m_buttons["mwake"]->set_active(false);
    m_buttons["pwake"]->set_active(false);
    m_buttons["twake"]->set_active(true);

    Environment *env = Environment::get_instance();
    string path = "res/images/colony/facilities/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    shared_ptr<Texture> texture;

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    int twaked = m_colony->twaked();

    for (int i = 5, y = 235; i > 0; --i, y += 77)
    {
        string red = "red_";
        if (twaked / 19 >= i)
        {
            red = "";
        }

        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), 384 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), 462 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "pistol.png");
        env->canvas->draw(texture.get(), 540 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "sniper.png");
        env->canvas->draw(texture.get(), 618 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), 696 * scale_w, (y+25) * scale_h);
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), 774 * scale_w, (y+25) * scale_h);

        Point a(360 * scale_w, (y-14+25) * scale_h);
        Point b(851 * scale_w, (y-14+25) * scale_h);
        env->canvas->draw(Line(a, b), color);
    }

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");

    Rect rect(333 * scale_w, (222 / H) * env->canvas->h(), 25 * scale_w,
        (416 / H) * env->canvas->h());
    env->canvas->draw(rect, color);

    texture = env->resources_manager->get_texture(path + "wake-t.png");
    env->canvas->draw(texture.get(), 308 * scale_w, (628 / H) * env->canvas->h());

    int x = 340;
    int y = (629 - 4 * twaked);

    rect = Rect(x * scale_w, y * scale_h, 10 * scale_w, 4 * (twaked+2) * scale_h);
    env->canvas->fill(rect, Color(79, 194, 193));

    y -= 13;
    texture = env->resources_manager->get_texture(path + "meter-t.png");
    env->canvas->draw(texture.get(), 290 * scale_w, y * scale_h);

    env->canvas->draw(std::to_string(twaked), (290+4) * scale_w, (y + 1) * scale_h, color);

    path = "res/images/colony/icons/";

    if (m_colony->matter() >= m_matter_cost)
    {
        y = 0;
        color = Color(170, 215, 190);
    }
    else
    {
        y = 20;
        color = Color(145, 6, 6);
    }

    Rect clip = Rect(0, y, 24, 20);
    texture = env->resources_manager->get_texture(path + "matter.png");
    env->canvas->draw(texture.get(), clip, 385 * scale_w, 670 * scale_h, 24, 20);

    if (m_colony->energy() >= m_energy_cost)
    {
        y = 0;
        color = Color(170, 215, 190);
    }
    else
    {
        y = 18;
        color = Color(145, 6, 6);
    }

    clip = Rect(0, y, 11, 18);
    texture = env->resources_manager->get_texture(path + "energy.png");
    env->canvas->draw(texture.get(), clip, 478 * scale_w, 670 * scale_h, 11, 18);

    env->canvas->draw(std::to_string(m_matter_cost), 425 * scale_w,
        670 * scale_h, color);
    env->canvas->draw(std::to_string(m_energy_cost), 510 * scale_w,
        670 * scale_h, color);
}
