#include "button.h"
#include "font.h"
#include "play.h"
#include "rect.h"
#include "resourcesmanager.h"
#include <iostream>

#define Y_BUTTON 266
#define W_BUTTON 305
#define H_BUTTON 40
#define W_BUTTON_BACK 140
#define H_BUTTON_BACK 60
#define SPACING 65

Play::Play(const string& next, const string& image)
    : Level("", next), m_image(nullptr), m_logo(nullptr), m_slot1(nullptr),
        m_slot2(nullptr), m_slot3(nullptr)
{
    env = Environment::get_instance();

    m_image = env->resources_manager->get_image(image);
    m_logo = env->resources_manager->get_image("res/images/menu/babel-logo.png");
    m_slot_bar = env->resources_manager->get_image("res/images/menu/slot-bar.png");
    
    const int x_button = (env->canvas->w() - 305) / 2;
    m_slot1 = new Button(this, "slot1", "res/images/menu/stripe.png", x_button,
        Y_BUTTON, W_BUTTON, H_BUTTON);
    m_slot2 = new Button(this, "slot2", "res/images/menu/stripe.png", x_button,
        Y_BUTTON + SPACING * 2, W_BUTTON, H_BUTTON);
    m_slot3 = new Button(this, "slot3", "res/images/menu/stripe.png", x_button,
        Y_BUTTON + SPACING * 3, W_BUTTON, H_BUTTON);

    m_back = new Button(this, "back", "res/images/menu/button.png",
        (env->canvas->w() - W_BUTTON_BACK)/2, env->canvas->h() - 149, W_BUTTON_BACK, H_BUTTON_BACK);

    m_slot1->add_observer(this);
    m_slot2->add_observer(this);
    m_slot3->add_observer(this);
    m_back->add_observer(this);

    add_child(m_slot1);
    add_child(m_slot2);
    add_child(m_slot3);
    add_child(m_back);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
}

void
Play::update_self(unsigned long)
{
    double scale = env->canvas->scale();

    const int x_button = (env->canvas->w() - W_BUTTON * scale) / 2;

    m_slot1->set_position(x_button, scale * (Y_BUTTON + SPACING));
    m_slot2->set_position(x_button, scale * (Y_BUTTON + SPACING * 2));
    m_slot3->set_position(x_button, scale * (Y_BUTTON + SPACING * 3));
    m_back->set_position((env->canvas->w() - W_BUTTON_BACK * scale)/2,
        env->canvas->h() - 149.0 * scale);

    shared_ptr<Font> font = env->canvas->font();
    font->set_size(22 * scale);
}

void
Play::draw_self()
{
    env->canvas->clear();
    env->canvas->draw(m_image.get());

    double scale = env->canvas->scale();

    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2,
        25 * scale);

    env->canvas->draw(m_slot_bar.get(), (env->canvas->w() - m_slot_bar->w() * scale)/2,
        (Y_BUTTON + 30) * scale);

    set_position(m_slot1->x() + 32 * scale, m_slot1->y() + 5 * scale);
    env->canvas->draw("New Game", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(m_slot2->x() + 32 * scale, m_slot2->y() + 5 * scale);
    env->canvas->draw("New Game", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(m_slot3->x() + 32 * scale, m_slot3->y() + 5 * scale);
    env->canvas->draw("New Game", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(m_back->x() + 50 * scale, m_back->y() + 15 * scale);
    env->canvas->draw("Back", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    shared_ptr<Font> font = env->canvas->font();
    font->set_size(24 * scale);
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

    if (button->id() == "slot1")
    {
        m_next = "gameover";
    }
    else if (button->id() == "slot2")
    {
        m_next = "gameover";
    }
    else if (button->id() == "slot3")
    {
        m_next = "gameover";
    }
    else if (button->id() == "back")
    {
        m_next = "menu";
    }

    m_done = true;
    return true;
}
