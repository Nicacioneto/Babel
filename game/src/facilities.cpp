#include "facilities.h"
#include <core/font.h>
#include <core/line.h>
#include <core/point.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Facilities::Facilities(int slot,const string& next)
    : Level("facilities", next), m_slot(slot),
        m_mwaked(0), m_pwaked(0), m_twaked(0),
        m_matter_cost(10), m_energy_cost(10), m_screen(CHAT)
{
    m_colony = new Colony(m_slot, this, "facilities");
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

        if (matter < m_matter_cost || energy < m_energy_cost)
        {
            return true;
        }

        if (++m_mwaked > 100)
        {
            m_mwaked = 100;
        }
        else
        {
            matter -= m_matter_cost;
            energy -= m_energy_cost;
        }

        m_colony->set_matter(matter);
        m_colony->set_energy(energy);

        Environment *env = Environment::get_instance();
        auto settings = env->resources_manager->get_settings("res/datas/slot" +
            to_string(m_slot) + "/colony.sav");
        settings->write<int>("Facilities", "military", m_mwaked);
        settings->save("res/datas/slot" + to_string(m_slot) + "/colony.sav");
    }
    else if (button->id() == "pwake")
    {
        int matter = m_colony->matter();
        int energy = m_colony->energy();

        if (matter < m_matter_cost || energy < m_energy_cost)
        {
            return true;
        }

        if (++m_pwaked > 100)
        {
            m_pwaked = 100;
        }
        else
        {
            matter -= m_matter_cost;
            energy -= m_energy_cost;
        }

        m_colony->set_matter(matter);
        m_colony->set_energy(energy);

        Environment *env = Environment::get_instance();
        auto settings = env->resources_manager->get_settings("res/datas/slot" +
            to_string(m_slot) + "/colony.sav");
        settings->write<int>("Facilities", "psionic", m_pwaked);
        settings->save("res/datas/slot" + to_string(m_slot) + "/colony.sav");
    }
    else if (button->id() == "twake")
    {
        int matter = m_colony->matter();
        int energy = m_colony->energy();

        if (matter < m_matter_cost || energy < m_energy_cost)
        {
            return true;
        }

        if (++m_twaked > 100)
        {
            m_twaked = 100;
        }
        else
        {
            matter -= m_matter_cost;
            energy -= m_energy_cost;
        }

        m_colony->set_matter(matter);
        m_colony->set_energy(energy);

        Environment *env = Environment::get_instance();
        auto settings = env->resources_manager->get_settings("res/datas/slot" +
            to_string(m_slot) + "/colony.sav");
        settings->write<int>("Facilities", "tech", m_twaked);
        settings->save("res/datas/slot" + to_string(m_slot) + "/colony.sav");
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

    for (int i = 5, y = 235; i > 0; --i, y += 77)
    {
        string red = "red_";
        if (m_mwaked / 19 >= i)
        {
            red = "";
        }

        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), (384 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), (462 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "pistol.png");
        env->canvas->draw(texture.get(), (540 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "sniper.png");
        env->canvas->draw(texture.get(), (618 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), (696 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), (774 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());

        Point a((360 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        Point b((851 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        env->canvas->draw(Line(a, b), color);
    }

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");
    m_mwaked = settings->read<int>("Facilities", "military", 0);

    Rect rect((333 / W) * env->canvas->w(), (222 / H) * env->canvas->h(),
        (25 / W) * env->canvas->w(), (416 / H) * env->canvas->h());
    env->canvas->draw(rect, color);

    texture = env->resources_manager->get_texture(path + "wake-m.png");
    env->canvas->draw(texture.get(), (308 / W) * env->canvas->w(), (628 / H) * env->canvas->h());

    int x = 340;
    int y = (629 - 4 * m_mwaked);

    rect = Rect((x / W) * env->canvas->w(), (y / H) * env->canvas->h(),
        (10 / W) * env->canvas->w(), (4*(m_mwaked+2) / H) * env->canvas->h());
    env->canvas->fill(rect, Color(206, 178, 46));

    y -= 13;
    texture = env->resources_manager->get_texture(path + "meter-m.png");
    env->canvas->draw(texture.get(), (290 / W) * env->canvas->w(),
        (y / H) * env->canvas->h());

    env->canvas->draw(std::to_string(m_mwaked), ((290+4) / W) * env->canvas->w(),
        ((y + 1) / H) * env->canvas->h(), color);

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
    env->canvas->draw(texture.get(), clip, 385/W * env->canvas->w(), 670/H * env->canvas->h(), 24, 20);

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
    env->canvas->draw(texture.get(), clip, 478/W * env->canvas->w(), 670/H * env->canvas->h(), 11, 18);

    env->canvas->draw(std::to_string(m_matter_cost), 425/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
    env->canvas->draw(std::to_string(m_energy_cost), 510/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
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

    for (int i = 5, y = 235; i > 0; --i, y += 77)
    {
        string red = "red_";
        if (m_pwaked / 19 >= i)
        {
            red = "";
        }

        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), (384 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), (462 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "pistol.png");
        env->canvas->draw(texture.get(), (540 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "sniper.png");
        env->canvas->draw(texture.get(), (618 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), (696 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), (774 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());

        Point a((360 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        Point b((851 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        env->canvas->draw(Line(a, b), color);
    }

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");
    m_pwaked = settings->read<int>("Facilities", "psionic", 0);

    Rect rect((333 / W) * env->canvas->w(), (222 / H) * env->canvas->h(),
        (25 / W) * env->canvas->w(), (416 / H) * env->canvas->h());
    env->canvas->draw(rect, color);

    texture = env->resources_manager->get_texture(path + "wake-p.png");
    env->canvas->draw(texture.get(), (308 / W) * env->canvas->w(), (628 / H) * env->canvas->h());

    int x = 340;
    int y = (629 - 4 * m_pwaked);

    rect = Rect((x / W) * env->canvas->w(), (y / H) * env->canvas->h(),
        (10 / W) * env->canvas->w(), (4*(m_pwaked+2) / H) * env->canvas->h());
    env->canvas->fill(rect, Color(146, 61, 133));

    y -= 13;
    texture = env->resources_manager->get_texture(path + "meter-p.png");
    env->canvas->draw(texture.get(), (290 / W) * env->canvas->w(),
        (y / H) * env->canvas->h());

    env->canvas->draw(std::to_string(m_pwaked), ((290+4) / W) * env->canvas->w(),
        ((y + 1) / H) * env->canvas->h(), color);

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
    env->canvas->draw(texture.get(), clip, 385/W * env->canvas->w(), 670/H * env->canvas->h(), 24, 20);

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
    env->canvas->draw(texture.get(), clip, 478/W * env->canvas->w(), 670/H * env->canvas->h(), 11, 18);

    env->canvas->draw(std::to_string(m_matter_cost), 425/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
    env->canvas->draw(std::to_string(m_energy_cost), 510/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
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

    for (int i = 5, y = 235; i > 0; --i, y += 77)
    {
        string red = "red_";
        if (m_twaked / 19 >= i)
        {
            red = "";
        }

        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), (384 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), (462 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "pistol.png");
        env->canvas->draw(texture.get(), (540 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "sniper.png");
        env->canvas->draw(texture.get(), (618 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "rifle.png");
        env->canvas->draw(texture.get(), (696 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());
        texture = env->resources_manager->get_texture(path + red + "shotgun.png");
        env->canvas->draw(texture.get(), (774 / W) * env->canvas->w(), ((y+25) / H) * env->canvas->h());

        Point a((360 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        Point b((851 / W) * env->canvas->w(), ((y-14+25) / H) * env->canvas->h());
        env->canvas->draw(Line(a, b), color);
    }

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/colony.sav");
    m_twaked = settings->read<int>("Facilities", "tech", 0);

    Rect rect((333 / W) * env->canvas->w(), (222 / H) * env->canvas->h(),
        (25 / W) * env->canvas->w(), (416 / H) * env->canvas->h());
    env->canvas->draw(rect, color);

    texture = env->resources_manager->get_texture(path + "wake-t.png");
    env->canvas->draw(texture.get(), (308 / W) * env->canvas->w(), (628 / H) * env->canvas->h());

    int x = 340;
    int y = (629 - 4 * m_twaked);

    rect = Rect((x / W) * env->canvas->w(), (y / H) * env->canvas->h(),
        (10 / W) * env->canvas->w(), (4*(m_twaked+2) / H) * env->canvas->h());
    env->canvas->fill(rect, Color(79, 194, 193));

    y -= 13;
    texture = env->resources_manager->get_texture(path + "meter-t.png");
    env->canvas->draw(texture.get(), (290 / W) * env->canvas->w(),
        (y / H) * env->canvas->h());

    env->canvas->draw(std::to_string(m_twaked), ((290+4) / W) * env->canvas->w(),
        ((y + 1) / H) * env->canvas->h(), color);

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
    env->canvas->draw(texture.get(), clip, 385/W * env->canvas->w(), 670/H * env->canvas->h(), 24, 20);

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
    env->canvas->draw(texture.get(), clip, 478/W * env->canvas->w(), 670/H * env->canvas->h(), 11, 18);

    env->canvas->draw(std::to_string(m_matter_cost), 425/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
    env->canvas->draw(std::to_string(m_energy_cost), 510/W * env->canvas->w(),
        670/H * env->canvas->h(), color);
}
