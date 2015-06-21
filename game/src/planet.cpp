#include "planet.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Planet::Planet(int slot, const string& next)
    : Level("planet", next), m_slot(slot), m_state(IDLE)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/planet/";

    m_texture = env->resources_manager->get_texture(path + "planet.png");
    m_popup = env->resources_manager->get_texture(path + "popup.png");

    load_buttons();
}

void
Planet::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_texture.get());

    if (m_state == POPUP)
    {
        int x = (env->canvas->w() - m_popup->w())/2;
        int y = (env->canvas->h() - m_popup->h())/2;

        Rect rect { 0, 0, (double)env->canvas->w(), (double)env->canvas->h() };
        
        env->canvas->set_blend_mode(Canvas::BLEND);
        env->canvas->fill(rect, Color(0, 0, 0, 128));
        env->canvas->set_blend_mode(Canvas::NONE);
        
        env->canvas->draw(m_popup.get(), x, y);

        env->canvas->draw("Start misson in " + m_text, x + (10/W) * env->canvas->w(), y + m_popup->h()/2);
    }
}

bool
Planet::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "colony")
    {
        set_next("base");
        finish();
    }
    else if (button->id() == "trunda")
    {
        m_text = "Trunda";
        enable_popup(true);
    }
    else if (button->id() == "jungle")
    {
        m_text = "Jungle";
        enable_popup(true);
    }
    else if (button->id() == "sea")
    {
        m_text = "Sea";
        enable_popup(true);
    }
    else if (button->id() == "dunes")
    {
        m_text = "Dunes";
        enable_popup(true);
    }

    else if (button->id() == "taiga")
    {
        m_text = "Taiga";
        enable_popup(true);
    }
    else if (button->id() == "lake")
    {
        m_text = "Lake";
        enable_popup(true);
    }
    else if (button->id() == "swamp")
    {
        m_text = "Swamp";
        enable_popup(true);
    }
    else if (button->id() == "x")
    {
        enable_popup(false);
    }
    else if (button->id() == "confirm")
    {
        start_mission();
        enable_popup(false);
    }

    return true;
}

void
Planet::enable_popup(bool popup)
{
    for (auto button : m_buttons)
    {
        if (button.first == "x" || button.first == "confirm")
        {
            button.second->set_active(popup);
            button.second->set_visible(popup);
        }
        else
        {
            button.second->set_active(not popup);
            button.second->set_visible(not popup);
        }
    }

    m_state = popup ? POPUP : IDLE;
}

void
Planet::load_buttons()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22);
    string path = "res/images/planet/";

    Button *button = new Button(this, "trunda", "", 0, 0,
        (300/W) * env->canvas->w(), (290/H) * env->canvas->h());
    m_buttons[button->id()] = button;

    button = new Button(this, "jungle", "", (390/W) * env->canvas->w(), 0,
        (360/W) * env->canvas->w(), (235/H) * env->canvas->h());
    m_buttons[button->id()] = button;


    button = new Button(this, "sea", "", (755/W) * env->canvas->w(), 0,
        (265/W) * env->canvas->w(), (235/H) * env->canvas->h());
    m_buttons[button->id()] = button;

    button = new Button(this, "dunes", "", (630/W) * env->canvas->w(), (270/H) * env->canvas->h(),
        (394/W) * env->canvas->w(), (185/H) * env->canvas->h());
    m_buttons[button->id()] = button;

    button = new Button(this, "taiga", "", (0/W) * env->canvas->w(), (410/H) * env->canvas->h(),
        (310/W) * env->canvas->w(), (358/H) * env->canvas->h());
    m_buttons[button->id()] = button;

    button = new Button(this, "lake", "", (300/W) * env->canvas->w(), (500/H) * env->canvas->h(),
        (300/W) * env->canvas->w(), (185/H) * env->canvas->h());
    m_buttons[button->id()] = button;

    button = new Button(this, "swamp", "", (600/W) * env->canvas->w(), (485/H) * env->canvas->h(),
        (424/W) * env->canvas->w(), (283/H) * env->canvas->h());
    m_buttons[button->id()] = button;

    button = new Button(this, "colony", "",(405/W) * env->canvas->w(), (410/H) * env->canvas->h(),
        (110/W) * env->canvas->w(), (80/H) * env->canvas->h());
    m_buttons[button->id()] = button;

    int x = ((W / 2 - 25 ) / W) * env->canvas->w() + m_popup->w() / 2;
    int y = ((H / 2 + 12 ) / H) * env->canvas->h() - m_popup->h() / 2;

    button = new Button(this, "x", path + "x.png", x, y,
        (13/W) * env->canvas->w(), (18/H) * env->canvas->w());
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    const int w = (140 / W) * env->canvas->w();
    const int h = (60 / H) * env->canvas->h();

    x = (env->canvas->w() - w) / 2;
    y = (env->canvas->h() + m_popup->h()) / 2 - (h + (10 / H) * env->canvas->h() );

    button = new Button(this, "confirm", path + "button.png", x , y, w, h);
    button->set_text("Confirm");
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }
}

void
Planet::start_mission()
{
    Environment *env = Environment::get_instance();

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/timers.sav");

    settings->write<unsigned long>(m_text, "final_time", 60000);
    settings->write<unsigned long>(m_text, "time", 0);
    settings->write<string>(m_text, "icon", "workshop");

    settings->save("res/datas/slot" + to_string(m_slot) + "/timers.sav");
}