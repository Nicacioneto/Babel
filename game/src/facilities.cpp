#include "facilities.h"
#include <core/font.h>
#include <core/line.h>
#include <core/point.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

Facilities::Facilities(const string& next)
    : Level("facilities", next), m_screen(CHAT), m_waked(0),
        m_matter_price(10), m_energy_price(10)
{
    m_colony = new Colony(this, "facilities");
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
    else if (button->id() == "wake")
    {
        int matter = m_colony->matter();
        int energy = m_colony->energy();

        if (matter < m_matter_price || energy < m_energy_price)
        {
            return true;
        }

        if (++m_waked > 100)
        {
            m_waked = 100;
        }
        else
        {
            matter -= m_matter_price;
            energy -= m_energy_price;
        }

        m_colony->set_matter(matter);
        m_colony->set_energy(energy);

        Environment *env = Environment::get_instance();
        auto settings = env->resources_manager->get_settings("res/datas/facilities.sav");
        settings->write<int>("Military", "waked", m_waked);
        settings->save("res/datas/facilities.sav");
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
        if (b.first != "facilities")
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
    
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/chat.sav");
    map< string, map<string, string> > sections = settings->sections();
    string text = sections["Facilities"]["welcome"];
    env->canvas->draw(text, ((305+5) / W) * env->canvas->w(), (605 / H) * env->canvas->h(), color);
    env->canvas->draw(Rect((305 / W) * env->canvas->w(), (605 / H) * env->canvas->h(),
        (670 / W) * env->canvas->w(), (116 / H) * env->canvas->h()), color);
}

void
Facilities::change_to_military()
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/facilities/";
    Color color(170, 215, 190);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    shared_ptr<Texture> texture;

    for (int i = 5, y = 235; i > 0; --i, y += 77)
    {
        string red = "_red";
        if (m_waked / 19 >= i)
        {
            red = "";
        }

        texture = env->resources_manager->get_texture(path + "rifle" + red + ".png");
        env->canvas->draw(texture.get(), (384 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "shotgun" + red + ".png");
        env->canvas->draw(texture.get(), (462 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "pistol" + red + ".png");
        env->canvas->draw(texture.get(), (540 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "sniper" + red + ".png");
        env->canvas->draw(texture.get(), (618 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "rifle" + red + ".png");
        env->canvas->draw(texture.get(), (696 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + "shotgun" + red + ".png");
        env->canvas->draw(texture.get(), (774 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());

        Point a((360 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        Point b((851 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        env->canvas->draw(Line(a, b), color);
    }

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/facilities.sav");
    m_waked = settings->read<int>("Military", "waked", 0);

    Rect rect((333 / W) * env->canvas->w(), (222 / H) * env->canvas->h(),
        (25 / W) * env->canvas->w(), (416 / H) * env->canvas->h());
    env->canvas->draw(rect, color);

    texture = env->resources_manager->get_texture(path + "wake.png");
    env->canvas->draw(texture.get(), (308 / W) * env->canvas->w(), (628 / H) * env->canvas->h());

    int x = 340;
    int y = (629 - 4 * m_waked);

    rect = Rect((x / W) * env->canvas->w(), (y / H) * env->canvas->h(),
        (10 / W) * env->canvas->w(), (4*(m_waked+2) / H) * env->canvas->h());
    env->canvas->fill(rect, Color(206, 178, 46));

    y -= 13;
    texture = env->resources_manager->get_texture(path + "meter.png");
    env->canvas->draw(texture.get(), (290 / W) * env->canvas->w(),
        (y / H) * env->canvas->h());

    env->canvas->draw(std::to_string(m_waked), ((290+4) / W) * env->canvas->w(),
        ((y + 1) / H) * env->canvas->h(), color);

    button = new Button(this, "wake", "", (315/W) * env->canvas->w(), (636 / H) * env->canvas->h(),
        60, 60);
    button->add_observer(this);
    add_child(button);

    path = "res/images/colony/icons/";

    if (m_colony->matter() >= m_matter_price)
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
    env->canvas->draw(texture.get(), clip, 385/W * env->canvas->w(), 670/H * env->canvas->h(), 24, 20);

    if (m_colony->energy() >= m_energy_price)
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
    env->canvas->draw(texture.get(), clip, 478/W * env->canvas->w(), 670/H * env->canvas->h(), 11, 18);

    env->canvas->draw(std::to_string(m_matter_price), 425/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
    env->canvas->draw(std::to_string(m_energy_price), 510/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
}

void
Facilities::change_to_psionic()
{
}

void
Facilities::change_to_tech()
{
}
