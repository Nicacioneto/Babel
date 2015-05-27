#include "planet.h"
#include <core/font.h>
#include <core/resourcesmanager.h>

#define X_MISSION_1 0
#define Y_MISSION_1 0
#define X_MISSION_2 100
#define Y_MISSION_2 100
#define X_MISSION_3 200
#define Y_MISSION_3 200
#define SIZE_MISSION 80
#define W_BUTTON_COLONY 140
#define H_BUTTON_COLONY 60

Planet::Planet(const string& next, const string& texture)
    : Level("planet", next), m_texture(nullptr), m_misson1(nullptr), m_misson2(nullptr),
        m_misson3(nullptr), m_colony(nullptr)
{
    Environment *env = Environment::get_instance();
    m_texture = env->resources_manager->get_texture(texture);

    double scale = env->canvas->scale();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22 * scale);

    m_colony = new Button(this, "colony", "res/images/menu/button.png",
        (env->canvas->w() - W_BUTTON_COLONY * scale)/2, env->canvas->h() - 149 * scale,
        W_BUTTON_COLONY * scale, H_BUTTON_COLONY * scale);
    m_colony->set_text("colony");

    m_misson1 = new Button(this, "mission1", "",
        X_MISSION_1 * scale, Y_MISSION_1 * scale, SIZE_MISSION * scale,
        SIZE_MISSION * scale);

    m_misson2 = new Button(this, "mission2", "",
        X_MISSION_2 * scale, Y_MISSION_2 * scale, SIZE_MISSION * scale,
        SIZE_MISSION * scale);

    m_misson3 = new Button(this, "mission3", "",
        X_MISSION_3 * scale, Y_MISSION_3 * scale, SIZE_MISSION * scale,
        SIZE_MISSION * scale);

    m_colony->add_observer(this);
    m_misson1->add_observer(this);
    m_misson2->add_observer(this);
    m_misson3->add_observer(this);

    add_child(m_colony);
    add_child(m_misson1);
    add_child(m_misson2);
    add_child(m_misson3);
}

void
Planet::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_texture.get());
    env->canvas->draw(m_misson1->bounding_box(), Color::RED);
    env->canvas->draw(m_misson2->bounding_box(), Color::BLUE);
    env->canvas->draw(m_misson3->bounding_box(), Color::YELLOW);
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
        set_next("colony");
        finish();
    }
    else if (button->id() == "mission1")
    {
    }
    else if (button->id() == "mission2")
    {
    }
    else if (button->id() == "mission3")
    {
    }

    return true;
}
