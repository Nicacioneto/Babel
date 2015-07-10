/*
 * Character class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "character.h"
#include <core/environment.h>
#include <core/font.h>
#include <core/mousebuttonevent.h>
#include <core/rect.h>
#include <core/settings.h>
#include <core/text.h>
#include <core/texture.h>
#include <cmath>
#include <ctime>

using std::to_string;

#define W 1024.0
#define H 768.0

Character::Character(int slot, Object *parent, ObjectID id, const string& texture,
    double x, double y, double w, double h, Type type)
    : Object(parent, id, x, y, w, h), m_slot(slot), m_texture(nullptr), 
        m_bracket_small(nullptr), m_bracket_big(nullptr), m_settings(nullptr),
        m_name(id), m_type(type), m_style(SMALL), m_attacks_quantity(0)
{
    Environment *env = Environment::get_instance();
    env->events_manager->register_listener(this);

    string path = "res/images/characters/";
    m_texture = env->resources_manager->get_texture(path + texture);
    m_bracket_small = env->resources_manager->get_texture("res/images/characters/card_small.png");
    m_bracket_big = env->resources_manager->get_texture("res/images/characters/card_big.png");

    if (not w and not h)
    {
        set_dimensions(m_texture->w(), m_texture->h());
    }

    if (m_type != HERO)
    {
        m_name = "Default";
    }

    m_settings = env->resources_manager->get_settings("res/datas/slot" + to_string(m_slot)
        + "/characters.sav");

    if (texture.find("small") == string::npos)
    {
        m_style = BIG;
    }
    init();

    if (m_type == HERO)
    {
        load_texts();
    }
}

Character::~Character()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
Character::init()
{
    m_military = m_settings->read<int>(m_name, "military", 0);
    m_psionic = m_settings->read<int>(m_name, "psionic", 0);
    m_tech = m_settings->read<int>(m_name, "tech", 0);
    m_levelup_m = m_settings->read<int>(m_name, "levelup_m", 0);
    m_levelup_p = m_settings->read<int>(m_name, "levelup_p", 0);
    m_levelup_t = m_settings->read<int>(m_name, "levelup_t", 0);
    m_shield = m_settings->read<int>(m_name, "shield", 0);
    m_max_shield = m_settings->read<int>(m_name, "max_shield", 0);
    m_life = m_settings->read<int>(m_name, "life", 0);
    m_max_life = m_settings->read<int>(m_name, "max_life", 0);
    m_mp = m_settings->read<int>(m_name, "mp", 0);
    m_max_mp = m_settings->read<int>(m_name, "max_mp", 0);
    m_might = m_settings->read<int>(m_name, "might", 0);
    m_mind = m_settings->read<int>(m_name, "mind", 0);
    m_resilience = m_settings->read<int>(m_name, "resilience", 0);
    m_willpower = m_settings->read<int>(m_name, "willpower", 0);
    m_agility = m_settings->read<int>(m_name, "agility", 0);
    m_perception = m_settings->read<int>(m_name, "perception", 0);
    // m_might_attack = m_settings->read<int>(m_name, "might_attack", 0);
    // m_mind_attack = m_settings->read<int>(m_name, "mind_attack", 0);
    // m_defense = m_settings->read<int>(m_name, "defense", 0);
    // m_cooldown = m_settings->read<int>(m_name, "cooldown", 0);
    m_might_armor = m_settings->read<int>(m_name, "might_armor", 0);
    m_mind_armor = m_settings->read<int>(m_name, "mind_armor", 0);
    // m_critical = m_settings->read<int>(m_name, "critical", 0);
    set_might_attack(m_might*10 + range((-1)*m_might, m_might));
    set_mind_attack(m_mind*10 + range((-1)*m_mind, m_mind));
    set_defense(m_resilience);
    set_cooldown((m_agility + m_perception) / 2);
    set_critical(m_agility/4);
    m_defense_mode = false;
}

int
Character::range(int min, int max) const
{
    srand(time(NULL));
    return rand() % (max - min) + min;
}

void
Character::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->draw(m_texture.get(), x(), y(), w(), h());

    if (m_type == HERO)
    {
        if (m_style == SMALL)
        {
            env->canvas->draw(m_bracket_small.get(), x(), y());
        }
        else
        {
            env->canvas->draw(m_bracket_big.get(), x(), y());
        }
        
        draw_attributes();
    }
}

void
Character::draw_attributes()
{
    set_attributes_positions();

    m_texts[m_name]->draw();

    m_texts[m_name + "_shield"]->draw();
    m_texts[m_name + "_max_shield"]->draw();

    m_texts[m_name + "_life"]->draw();
    m_texts[m_name + "_max_life"]->draw();

    m_texts[m_name + "_mp"]->draw();
    m_texts[m_name + "_max_mp"]->draw();

    if (m_style == BIG)
    {
        m_texts[m_name + "_military"]->draw();
        m_texts[m_name + "_psionic"]->draw();
        m_texts[m_name + "_tech"]->draw();
    }
}

void
Character::load_texts()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    
    string military = to_string(m_military);
    string psionic = to_string(m_psionic);
    string tech = to_string(m_tech);
    string shield = to_string(m_shield);
    string max_shield = to_string(m_max_shield);
    string life = to_string(m_life);
    string max_life = to_string(m_max_life);
    string mp = to_string(m_mp);
    string max_mp = to_string(m_max_mp);

    font->set_style(Font::BOLD);
    font->set_size(18);
    m_texts[m_name + "_military"] = new Text(this, military, Color(208, 179, 43));
    m_texts[m_name + "_psionic"] = new Text(this, psionic, Color(166, 69, 151));
    m_texts[m_name + "_tech"] = new Text(this, tech, Color(78, 191, 190));

    font->set_style(Font::NORMAL);
    font->set_size(m_style == SMALL ? 16 : 18);
    m_texts[m_name] = new Text(this, m_name, Color(170, 215, 190));

    font->set_size(m_style == SMALL ? 10 : 14);
    m_texts[m_name + "_shield"] = new Text(this, shield + "/", Color(170, 215, 190));
    m_texts[m_name + "_life"] = new Text(this, life + "/", Color(170, 215, 190));
    m_texts[m_name + "_mp"] = new Text(this, mp + "/", Color(170, 215, 190));

    font->set_size(m_style == SMALL ? 7 : 10);
    m_texts[m_name + "_max_shield"] = new Text(this, max_shield, Color(170, 215, 190));
    m_texts[m_name + "_max_life"] = new Text(this, max_life, Color(170, 215, 190));
    m_texts[m_name + "_max_mp"] = new Text(this, max_mp, Color(170, 215, 190));
}

void
Character::delete_texts()
{
    for (auto text : m_texts)
    {
        if (text.second != nullptr)
        {
            delete text.second;
            text.second = nullptr;
        }
    }
}

void
Character::set_attributes_positions()
{
    Environment *env = Environment::get_instance();
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    m_texts[m_name]->set_position(x() + 131 * scale_w, y() + 3 * scale_h);

    Rect box;
    if (m_style == SMALL)
    {
        box.set_position(x() + 172 * scale_w, y() + 32 * scale_h);
        box.set_dimensions(40 * scale_w, 21 * scale_h);
    }
    else
    {
        box.set_position(x() + 144 * scale_w, y() + 160 * scale_h);
        box.set_dimensions(65 * scale_w, 20 * scale_h);
    }

    double w_shield = m_texts[m_name + "_shield"]->w() + m_texts[m_name + "_max_shield"]->w();
    double x_shield = (box.w() - w_shield)/2 + box.x();
    double y_shield = (box.h() - m_texts[m_name + "_shield"]->h())/2 + box.y();
    double x_max_shield = x_shield + m_texts[m_name + "_shield"]->w();
    double y_max_shield = y_shield + m_texts[m_name + "_shield"]->h() -
        m_texts[m_name + "_max_shield"]->h();

    m_texts[m_name + "_shield"]->set_position(x_shield, y_shield);
    m_texts[m_name + "_max_shield"]->set_position(x_max_shield, y_max_shield);

    (m_style == SMALL ? box.set_y(y() + 59 * scale_h) : box.set_y(y() + 190 * scale_h));
    double w_life = m_texts[m_name + "_life"]->w() + m_texts[m_name + "_max_life"]->w();
    double x_life = (box.w() - w_life)/2 + box.x();
    double y_life = (box.h() - m_texts[m_name + "_life"]->h())/2 + box.y();
    double x_max_life = x_life + m_texts[m_name + "_life"]->w();
    double y_max_life = y_life + m_texts[m_name + "_life"]->h() -
        m_texts[m_name + "_max_life"]->h();

    m_texts[m_name + "_life"]->set_position(x_life, y_life);
    m_texts[m_name + "_max_life"]->set_position(x_max_life, y_max_life);

    (m_style == SMALL ? box.set_y(y() + 86 * scale_h) : box.set_y(y() + 220 * scale_h));
    double w_mp = m_texts[m_name + "_mp"]->w() + m_texts[m_name + "_max_mp"]->w();
    double x_mp = (box.w() - w_mp)/2 + box.x();
    double y_mp = (box.h() - m_texts[m_name + "_mp"]->h())/2 + box.y();
    double x_max_mp = x_mp + m_texts[m_name + "_mp"]->w();
    double y_max_mp = y_mp + m_texts[m_name + "_mp"]->h() -
        m_texts[m_name + "_max_mp"]->h();

    m_texts[m_name + "_mp"]->set_position(x_mp, y_mp);
    m_texts[m_name + "_max_mp"]->set_position(x_max_mp, y_max_mp);

    if (m_style == BIG)
    {
        box.set_position(x() + 170 * scale_w, y() + 37 * scale_h);
        box.set_dimensions(40 * scale_w, 22 * scale_h);

        double x_military = (box.w() - m_texts[m_name + "_military"]->w())/2 + box.x();
        double y_military = (box.h() - m_texts[m_name + "_military"]->h())/2 + box.y();

        m_texts[m_name + "_military"]->set_position(x_military, y_military);
        
        box.set_y(y() + 69 * scale_h);
        double x_psionic = (box.w() - m_texts[m_name + "_psionic"]->w())/2 + box.x();
        double y_psionic = (box.h() - m_texts[m_name + "_psionic"]->h())/2 + box.y();

        m_texts[m_name + "_psionic"]->set_position(x_psionic, y_psionic);

        box.set_y(y() + 100 * scale_h);
        double x_tech = (box.w() - m_texts[m_name + "_tech"]->w())/2 + box.x();
        double y_tech = (box.h() - m_texts[m_name + "_tech"]->h())/2 + box.y();

        m_texts[m_name + "_tech"]->set_position(x_tech, y_tech);
    }
}

void
Character::update_from_levelup(string class_)
{
    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/levelup.sav");

    set_life(m_life + settings->read<int>(class_, "life", 0));
    set_max_life(m_max_life + settings->read<int>(class_, "life", 0));
    set_mp(m_mp + settings->read<int>(class_, "mp", 0));
    set_max_mp(m_max_mp + settings->read<int>(class_, "mp", 0));
    set_might(m_might + settings->read<int>(class_, "might", 0));
    set_mind(m_mind + settings->read<int>(class_, "mind", 0));
    set_resilience(m_resilience + settings->read<int>(class_, "resilience", 0));
    set_willpower(m_willpower + settings->read<int>(class_, "willpower", 0));
    set_agility(m_agility + settings->read<int>(class_, "agility", 0));
    set_perception(m_perception + settings->read<int>(class_, "perception", 0));

    delete_texts();
    load_texts();
}

bool
Character::on_event(const MouseButtonEvent& event)
{
    Environment *env = Environment::get_instance();

    if (event.state() == MouseButtonEvent::PRESSED and
        event.button() == MouseButtonEvent::LEFT and
        bounding_box().contains(event.x(), event.y()))
    {
        notify(id(), "");

        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);

        return true;
    }

    return false;
}

int
Character::receive_damage(Character *attacker)
{
    double damage = 0;

    if (attacker->might_attack() > m_defense * (1 + m_defense_mode))
    {
        damage = attacker->might_attack() - m_defense * (1 + m_defense_mode);
    }
    else
    {
        damage = attacker->might_attack() * 0.1;
    }

    // TODO Balance attacks

    int d = round(damage);
    m_life -= d;

    return d;
}

Character::Type
Character::type() const
{
    return m_type;
}

int
Character::attacks_quantity() const
{
    return m_attacks_quantity;
}

int
Character::military() const
{
    return m_military;
}

int
Character::psionic() const
{
    return m_psionic;
}

int
Character::tech() const
{
    return m_tech;
}

int
Character::might() const
{
    return m_might;
}

int
Character::levelup_m() const
{
    return m_levelup_m;
}

int
Character::levelup_p() const
{
    return m_levelup_p;
}

int
Character::levelup_t() const
{
    return m_levelup_t;
}

int
Character::shield() const
{
    return m_shield;
}

int
Character::max_shield() const
{
    return m_max_shield;
}

int
Character::life() const
{
    return m_life;
}

int
Character::max_life() const
{
    return m_max_life;
}

int
Character::mp() const
{
    return m_mp;
}

int
Character::max_mp() const
{
    return m_max_mp;
}

int
Character::mind() const
{
    return m_mind;
}

int
Character::resilience() const
{
    return m_resilience;
}

int
Character::willpower() const
{
    return m_willpower;
}

int
Character::agility() const
{
    return m_agility;
}

int
Character::perception() const
{
    return m_perception;
}

int
Character::might_attack() const
{
    return m_might_attack;
}

int
Character::mind_attack() const
{
    return m_mind_attack;
}

int
Character::cooldown() const
{
    return m_cooldown;
}

int
Character::defense() const
{
    return m_defense;
}

int
Character::might_armor() const
{
    return m_might_armor;
}

int
Character::mind_armor() const
{
    return m_mind_armor;
}

int
Character::critical() const
{
    return m_critical;
}

void
Character::set_texture(const string& id)
{
    Environment *env = Environment::get_instance();
    string texture = "res/images/characters/" + id;
    m_texture = env->resources_manager->get_texture(texture);
    m_style = texture.find("small") != string::npos ? SMALL : BIG;
}

void
Character::set_attacks_quantity(int attacks_quantity)
{
    m_attacks_quantity = attacks_quantity;
}

void
Character::set_military(int military)
{
    m_military = military;
    write<int>("military", military);
}

void
Character::set_psionic(int psionic)
{
    m_psionic = psionic;
    write<int>("psionic", psionic);
}

void
Character::set_tech(int tech)
{
    m_tech = tech;
    write<int>("tech", tech);
}

void
Character::set_levelup_m(int levelup_m)
{
    m_levelup_m = levelup_m;
    write<int>("levelup_m", levelup_m);
}

void
Character::set_levelup_p(int levelup_p)
{
    m_levelup_p = levelup_p;
    write<int>("levelup_p", levelup_p);
}

void
Character::set_levelup_t(int levelup_t)
{
    m_levelup_t = levelup_t;
    write<int>("levelup_t", levelup_t);
}

void
Character::set_shield(int shield)
{
    m_shield = shield;
    write<int>("shield", shield);
}

void
Character::set_max_shield(int max_shield)
{
    m_max_shield = max_shield;
    write<int>("max_shield", max_shield);
}

void
Character::set_life(int life)
{
    m_life = life;
    write<int>("life", life);
}

void
Character::set_max_life(int max_life)
{
    m_max_life = max_life;
    write<int>("max_life", max_life);
}

void
Character::set_mp(int mp)
{
    m_mp = mp;
    write<int>("mp", mp);
}

void
Character::set_max_mp(int max_mp)
{
    m_max_mp = max_mp;
    write<int>("max_mp", max_mp);
}

void
Character::set_might(int might)
{
    m_might = might;
    write<int>("might", might);
}

void
Character::set_mind(int mind)
{
    m_mind = mind;
    write<int>("mind", mind);
}

void
Character::set_resilience(int resilience)
{
    m_resilience = resilience;
    write<int>("resilience", resilience);
}

void
Character::set_willpower(int willpower)
{
    m_willpower = willpower;
    write<int>("willpower", willpower);
}

void
Character::set_agility(int agility)
{
    m_agility = agility;
    write<int>("agility", agility);
}

void
Character::set_perception(int perception)
{
    m_perception = perception;
    write<int>("perception", perception);
}

void
Character::set_might_attack(int might_attack)
{
    m_might_attack = might_attack;
    write<int>("might_attack", might_attack);
}

void
Character::set_mind_attack(int mind_attack)
{
    m_mind_attack = mind_attack;
    write<int>("mind_attack", mind_attack);
}

void
Character::set_cooldown(int cooldown)
{
    m_cooldown = cooldown;
    write<int>("cooldown", cooldown);
}

void
Character::set_defense(int defense)
{
    m_defense = defense;
    write<int>("defense", defense);
}

void
Character::set_might_armor(int might_armor)
{
    m_might_armor = might_armor;
    write<int>("might_armor", might_armor);
}

void
Character::set_mind_armor(int mind_armor)
{
    m_mind_armor = mind_armor;
    write<int>("mind_armor", mind_armor);
}

void
Character::set_critical(int critical)
{
    m_critical = critical;
    write<int>("critical", critical);
}

void
Character::set_defense_mode(bool defense_mode)
{
    m_defense_mode = defense_mode;
}

template<typename T> void
Character::write(const string& attr, const T& value)
{
    if (m_name != "Default")
    {
        m_settings->write<int>(m_name, attr, value);
        m_settings->save("res/datas/slot" + to_string(m_slot) + "/characters.sav");
    }
}
