#include "planet.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>

#define W 1024.0
#define H 768.0

Planet::Planet(const string& next, const string& texture)
    : Level("planet", next), m_texture(nullptr), m_state(IDLE)
{
    Environment *env = Environment::get_instance();
    m_texture = env->resources_manager->get_texture(texture);
    
    string path = "res/images/planet/";
    m_popup = env->resources_manager->get_texture(path + "popup.png");

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22);

    load_buttons(path);
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

        Rect r {0, 0, (double)env->canvas->w(), (double)env->canvas->h()};
        
        env->canvas->set_blend_mode(Canvas::BLEND);
        env->canvas->fill(r, Color(0, 0, 0, 128));
        env->canvas->set_blend_mode(Canvas::NONE);
        
        env->canvas->draw(m_popup.get(), x, y);
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
    else if (button->id() == "jungle")
    {
        enable_popup(true);
    }
    else if (button->id() == "mountain")
    {
        enable_popup(true);
    }
    else if (button->id() == "sea")
    {
        enable_popup(true);
    }
    else if (button->id() == "dunes")
    {
        enable_popup(true);
    }
    else if (button->id() == "x")
    {
        enable_popup(false);
    }

    return true;
}

void
Planet::enable_popup(bool popup)
{
    for (auto button : m_buttons)
    {
        if (button.first == "x")
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
Planet::load_buttons(string path)
{
    Environment *env = Environment::get_instance();

    const int w_button = (140 / W) * env->canvas->w();
    const int h_button = (60 / H) * env->canvas->h();

    Button *button = new Button(this, "jungle", path + "button.png",
        (200/W) * env->canvas->w(), (100/H) * env->canvas->h(), w_button, h_button);
    button->set_sprites(2);
    button->set_text("Jungle");
    m_buttons[button->id()] = button;

    button = new Button(this, "mountain", path + "button.png",
        (50/W) * env->canvas->w(), (100/H) * env->canvas->h(), w_button, h_button);
    button->set_sprites(2);
    button->set_text("Mountain");
    m_buttons[button->id()] = button;

    button = new Button(this, "sea", path + "button.png",
        (400/W) * env->canvas->w(), (100/H) * env->canvas->h(), w_button, h_button);
    button->set_sprites(2);
    button->set_text("Sea");
    m_buttons[button->id()] = button;

    button = new Button(this, "dunes", path + "button.png",
        (800/W) * env->canvas->w(), (600/H) * env->canvas->h(), w_button, h_button);
    button->set_sprites(2);
    button->set_text("Dunes");
    m_buttons[button->id()] = button;

    button = new Button(this, "colony", path + "button.png",
        (env->canvas->w() - w_button) / 2, (env->canvas->h() - h_button) / 2, w_button, h_button);
    button->set_sprites(2);
    button->set_text("Colony");
    m_buttons[button->id()] = button;

    const int x_x = ((W / 2 - 25 ) / W) * env->canvas->w() + m_popup->w() / 2;
    const int y_x = ((H / 2 + 12 ) / H) * env->canvas->h() - m_popup->h() / 2;
    button = new Button(this, "x", path + "x.png",
        x_x, y_x, 13/W * env->canvas->w(), 18 / H  * env->canvas->w());
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    for (auto it : m_buttons)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}
