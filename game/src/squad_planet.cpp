/*
 * SquadPlanet class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "character.h"
#include "colony.h"
#include "drone.h"
#include "squad_planet.h"
#include "team_planet.h"
#include "timer.h"
#include "mission.h"
#include <algorithm>
#include <core/font.h>
#include <core/keyboardevent.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

SquadPlanet::SquadPlanet(int slot, Screen screen, const string& next)
    : Level("SquadPlanet", next), m_slot(slot), m_screen(screen), m_settings(nullptr),
        m_bracket(nullptr), m_level(nullptr), m_time(nullptr), m_team(nullptr), m_drone(nullptr)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/tower/squad/";

    m_settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/planet.sav");

    env->events_manager->register_listener(this);

    m_bracket = env->resources_manager->get_texture("res/images/colony/barracks/bracket.png");

    m_level = env->resources_manager->get_texture("res/images/tower/squad/level.png");

    m_time = env->resources_manager->get_texture("res/images/tower/squad/timer.png");

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button = new Button(this, "confirm_choice", path + "confirm.png",
        470 * scale_w, 622 * scale_h, 25 * scale_w, 25 * scale_h);
    m_buttons[button->id()] = button;

    button = new Button(this, "reset_choice", path + "change.png",
        530 * scale_w, 622 * scale_h, 25 * scale_w, 25 * scale_h);
    button->set_sprites(2);
    m_buttons[button->id()] = button;

    button = new Button(this, "select_squad", path + "select_squad.png",
        300 * scale_w, 675 * scale_h, 100 * scale_w, 18 * scale_h);
    button->set_sprites(4);
    if (m_screen == TEAM)
    {
        button->change_state(Button::ACTIVE);
    }
    m_buttons[button->id()] = button;

    button = new Button(this, "select_drone", path + "select_drone.png",
        465 * scale_w, 675 * scale_h, 100 * scale_w, 18 * scale_h);
    button->set_sprites(4);
    if (m_screen == DRONE)
    {
        button->change_state(Button::ACTIVE);
    }
    m_buttons[button->id()] = button;

    button = new Button(this, "select_vehicle", path + "select_vehicle.png",
        130 * scale_w, 675 * scale_h, 100 * scale_w, 18 * scale_h);
    button->set_sprites(4);
    if (m_screen == DRONE)
    {
        button->change_state(Button::ACTIVE);
    }
    m_buttons[button->id()] = button;

    button = new Button(this, "confirm", path + "select_confirm.png",
        645 * scale_w, 675 * scale_h, 60 * scale_w, 18 * scale_h);
    button->set_sprites(4);
    m_buttons[button->id()] = button;

    button = new Button(this, "back", path + "back.png",
        912 * scale_w, 55 * scale_h, 67 * scale_w, 26 * scale_h);
    button->set_sprites(1);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    m_team = new TeamPlanet(m_slot, this);
    m_team->add_observer(this);
    add_child(m_team);

    confirm_state();

    m_drone = new Drone(m_slot, this);
    m_drone->add_observer(this);
    add_child(m_drone);

    change_screen();

    calculate_level();
}

SquadPlanet::~SquadPlanet()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
SquadPlanet::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    Color color(70, 89, 79);

    env->canvas->draw(m_bracket.get(), 30 * scale_w, 25 * scale_h);
    env->canvas->draw(m_level.get(), 400 * scale_w, 60 * scale_h);
    env->canvas->draw(m_time.get(), 660 * scale_w, 85 * scale_h);
    env->canvas->draw("Click to add/remove", 60 * scale_w, 80 * scale_h, color);
    env->canvas->draw("Confirm Choice", 345 * scale_w, 630 * scale_h, color);
    env->canvas->draw("Reset Choice", 565 * scale_w, 630 * scale_h, color);

    env->canvas->draw(m_level_m, 418 * scale_w, 120 * scale_h, color);
    env->canvas->draw(m_level_p, 495 * scale_w, 120 * scale_h, color);
    env->canvas->draw(m_level_t, 580 * scale_w, 120 * scale_h, color);

    env->canvas->draw(m_timer, 685 * scale_w, 95 * scale_h, color);
}

bool
SquadPlanet::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "select_squad")
    {
        m_screen = TEAM;
        change_screen();

        m_buttons["select_drone"]->change_state(Button::IDLE);
        m_buttons["select_vehicle"]->change_state(Button::IDLE);
        button->change_state(Button::ACTIVE);
    }
    else if (button->id() == "select_drone")
    {
        m_screen = DRONE;
        change_screen();

        m_buttons["select_squad"]->change_state(Button::IDLE);
        m_buttons["select_vehicle"]->change_state(Button::IDLE);
        button->change_state(Button::ACTIVE);
    }
    else if (button->id() == "select_vehicle")
    {
        m_screen = DRONE;
        change_screen();

        m_buttons["select_squad"]->change_state(Button::IDLE);
        m_buttons["select_drone"]->change_state(Button::IDLE);
        button->change_state(Button::ACTIVE);
    }
    else if (button->id() == "confirm_choice")
    {
        m_screen == TEAM ? m_team->confirm() : m_drone->confirm();
        confirm_state();
    }
    else if (button->id() == "reset_choice")
    {
        m_screen == TEAM ? m_team->reset() : m_drone->load_drones();
    }
    else if (button->id() == "confirm")
    {
        Environment *env = Environment::get_instance();
        string path = "res/datas/slot" + to_string(m_slot) + "/squad_planet.sav";
        shared_ptr<Settings> settings = env->resources_manager->get_settings(path);

        string hero1 = settings->read<string>("Squad Planet", "hero1", "");
        if (hero1 != "")
        {
            start_mission();
            set_next("planet");
            finish();
        }
    }
    else if (button->id() == "back")
    {
        set_next("planet");
        finish();
    }

    return true;
}

void
SquadPlanet::change_screen()
{
    m_team->set_visible(m_screen == TEAM);
    m_team->change_buttons(m_screen == TEAM);

    m_drone->set_visible(m_screen == DRONE);
    m_drone->change_buttons(m_screen == DRONE);
}

bool
SquadPlanet::on_event(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED and
        event.key() == KeyboardEvent::ESCAPE)
    {
        set_next("base");
        finish();

        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);

        return true;
    }

    return false;
}

void
SquadPlanet::confirm_state()
{
    if (not m_team->size())
    {
        m_buttons["confirm"]->change_state(Button::INACTIVE);
    }
    else
    {
        m_buttons["confirm"]->change_state(Button::IDLE);
    }
}

void
SquadPlanet::start_mission()
{
    string current_mission = m_settings->read<string>("Mission", "current", "");
    string time = m_settings->read<string>(current_mission, "time", "00:00");
    int matter = m_settings->read<int>(current_mission, "matter", 0);
    int energy = m_settings->read<int>(current_mission, "energy", 0);
    int m = m_settings->read<int>(current_mission, "military", 0);
    int p = m_settings->read<int>(current_mission, "psionic", 0);
    int t = m_settings->read<int>(current_mission, "tech", 0);

    unsigned long min = atol(time.substr(0, 2).c_str()) * 60;
    unsigned long seg = atol(time.substr(3).c_str());

    Mission *mission = new Mission(current_mission, min + seg, "workshop",
        energy, matter, "colony", m, p, t, m_team->m_team, m_slot);

    mission->available_character(false);
    update_squad();
    start_time(mission);
}

void
SquadPlanet::calculate_level()
{
    string mission = m_settings->read<string>("Mission", "current", "");
    int m = m_settings->read<int>(mission, "military", 0);
    int p = m_settings->read<int>(mission, "psionic", 0);
    int t = m_settings->read<int>(mission, "tech", 0);

    m_level_m = m < 1 ? "None" : (m < 3 ? "Low" : "High");
    m_level_p = p < 1 ? "None" : (p < 3 ? "Low" : "High");
    m_level_t = t < 1 ? "None" : (t < 3 ? "Low" : "High");

    m_timer = m_settings->read<string>(mission, "time", "00:00");
}

void
SquadPlanet::update_squad()
{
    Environment *env = Environment::get_instance();

    string path = "res/datas/slot" + to_string(m_slot) + "/squad.sav";
    shared_ptr<Settings> squad_tower = env->resources_manager->get_settings(path);

    for (int i = 1; i <= 4; ++i)
    {
        squad_tower->write<string>("Squad", "hero" + to_string(i), "");
    }
    squad_tower->save(path);

    path = "res/datas/slot" + to_string(m_slot) + "/squad_planet.sav";
    shared_ptr<Settings> squad_planet = env->resources_manager->get_settings(path);

    for (int i = 1; i <= 4; ++i)
    {
        squad_planet->write<string>("Squad Planet", "hero" + to_string(i), "");
    }

    squad_planet->save(path);
}
