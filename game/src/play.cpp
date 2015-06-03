#include "play.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/settings.h>
#include <core/texture.h>

#define Y_BUTTON 266
#define W_BUTTON 305
#define H_BUTTON 40
#define W_BUTTON_BACK 140
#define H_BUTTON_BACK 60
#define SPACING 65

Play::Play(const string& next, const string& texture)
    : Level("play", next), m_texture(nullptr), m_logo(nullptr)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(texture);
    m_logo = env->resources_manager->get_texture("res/images/menu/babel-logo.png");
    m_slot_bar = env->resources_manager->get_texture("res/images/menu/slot-bar.png");
    
    double scale = env->canvas->scale();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22 * scale);
    
    slots();
}

void
Play::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    
    env->canvas->draw(m_texture.get());

    double scale = env->canvas->scale();

    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2,
        25 * scale);

    env->canvas->draw(m_slot_bar.get(), (env->canvas->w() - m_slot_bar->w() * scale)/2,
        (Y_BUTTON + 30) * scale);

    set_position((env->canvas->w() - 115 * scale) / 2,  192 * scale);
    env->canvas->draw("PLAY GAME", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));
}

bool
Play::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/settings.ini");

    int slot1 = settings->read<int>("Slots", "slot1", 0);
    int slot2 = settings->read<int>("Slots", "slot2", 0);
    int slot3 = settings->read<int>("Slots", "slot3", 0);

    if (button->id() == "slot1")
    {
        set_next("base");
        settings->write<int>("Slots", "slot1", 1);
        settings->write<int>("Slots", "slot2", slot2);
        settings->write<int>("Slots", "slot3", slot3);
    }
    else if (button->id() == "slot2")
    {
        set_next("base");
        settings->write<int>("Slots", "slot1", slot1);
        settings->write<int>("Slots", "slot2", 1);
        settings->write<int>("Slots", "slot3", slot3);
    }
    else if (button->id() == "slot3")
    {
        set_next("base");
        settings->write<int>("Slots", "slot1", slot1);
        settings->write<int>("Slots", "slot2", slot2);
        settings->write<int>("Slots", "slot3", 1);
    }
    else if (button->id() == "slot1_x")
    {
        set_next("play");
        settings->write<int>("Slots", "slot1", 0);
        settings->write<int>("Slots", "slot2", slot2);
        settings->write<int>("Slots", "slot3", slot3);
    }
    else if (button->id() == "slot2_x")
    {
        set_next("play");
        settings->write<int>("Slots", "slot1", slot1);
        settings->write<int>("Slots", "slot2", 0);
        settings->write<int>("Slots", "slot3", slot3);
    }
    else if (button->id() == "slot3_x")
    {
        set_next("play");
        settings->write<int>("Slots", "slot1", slot1);
        settings->write<int>("Slots", "slot2", slot2);
        settings->write<int>("Slots", "slot3", 0);
    }
    else if (button->id() == "back")
    {
        set_next("menu");
    }

    settings->save("res/settings.ini");
    finish();

    return true;
}

void
Play::slots()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    const int x_button = (env->canvas->w() - W_BUTTON * scale) / 2;

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/settings.ini");
    string text;

    int saved = settings->read<int>("Slots", "slot1", 0);
    if (saved)
    {
        text = "Slot 1";
        m_slots[0][1] = new Button(this, "slot1_x", "res/images/menu/x.png", x_button + 280 * scale,
            scale * (Y_BUTTON + SPACING + 12), 13 * scale, 18 * scale);
        m_slots[0][1]->set_sprites(1);
        m_slots[0][1]->add_observer(this);
        add_child(m_slots[0][1]);
    }
    else
    {
        text = "New Game";
    }

    m_slots[0][0] = new Button(this, "slot1", "res/images/menu/stripe.png", x_button,
        scale * (Y_BUTTON + SPACING), W_BUTTON * scale, H_BUTTON * scale);
    m_slots[0][0]->set_text(text);

    saved = settings->read<int>("Slots", "slot2", 0);
    if (saved)
    {
        text = "Slot 2";
        m_slots[1][1] = new Button(this, "slot2_x", "res/images/menu/x.png", x_button + 280 * scale,
            scale * (Y_BUTTON + SPACING * 2 + 12), 13 * scale, 18 * scale);
        m_slots[1][1]->set_sprites(1);
        m_slots[1][1]->add_observer(this);
        add_child(m_slots[1][1]);
    }
    else
    {
        text = "New Game";
    }

    m_slots[1][0] = new Button(this, "slot2", "res/images/menu/stripe.png", x_button,
        scale * (Y_BUTTON + SPACING * 2), W_BUTTON * scale, H_BUTTON * scale);
    m_slots[1][0]->set_text(text);

    saved = settings->read<int>("Slots", "slot3", 0);
    if (saved)
    {
        text = "Slot 3";
        m_slots[2][1] = new Button(this, "slot3_x", "res/images/menu/x.png", x_button + 280 * scale,
            scale * (Y_BUTTON + SPACING * 3 + 12), 13 * scale, 18 * scale);
        m_slots[2][1]->set_sprites(1);
        m_slots[2][1]->add_observer(this);
        add_child(m_slots[2][1]);
    }
    else
    {
        text = "New Game";
    }

    m_slots[2][0] = new Button(this, "slot3", "res/images/menu/stripe.png", x_button,
        scale * (Y_BUTTON + SPACING * 3), W_BUTTON * scale, H_BUTTON * scale);
    m_slots[2][0]->set_text(text);

    m_back = new Button(this, "back", "res/images/menu/button.png",
        (env->canvas->w() - W_BUTTON_BACK * scale)/2, env->canvas->h() - 149 * scale,
        W_BUTTON_BACK * scale, H_BUTTON_BACK * scale);
    m_back->set_text("Back");

    for (auto slot : m_slots)
    {
        slot[0]->add_observer(this);
        add_child(slot[0]);
    }

    m_back->add_observer(this);
    add_child(m_back);
}
