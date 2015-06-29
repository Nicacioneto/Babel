/*
 * Combat class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "combat.h"
#include "character.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/text.h>
#include <core/keyboardevent.h>

using std::to_string;

#define W 1024.0
#define H 768.0
#define DELAY 1000

Combat::Combat(int slot, const string& next)
    : Level("combat", next), m_slot(slot), m_attacker(""), m_state(ENEMY_ATTACK),
        m_enemy_turn(nullptr), m_text(nullptr), m_last(0)
{
    Environment *env = Environment::get_instance();

    env->events_manager->register_listener(this);
    m_texture = env->resources_manager->get_texture("res/images/combat/arena.png");

    env->sfx->play("res/sfx/uiBattle_Turn1.ogg", 1);

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(30);

    load_characters();
    load_enemies();
}

Combat::~Combat()
{
    if (m_text)
    {
        delete m_text;
        m_text = nullptr;
    }

    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
Combat::update_self(unsigned long elapsed)
{
    Environment *env = Environment::get_instance();

    if (m_state == FINISHED_COMBAT)
    {
        return;
    }
    else if (m_characters.empty())
    {
        set_next("base");
        m_result = env->resources_manager->get_texture("res/images/combat/you-lose.png");
        m_state = FINISHED_COMBAT;

        for (auto it : m_enemies)
        {
            it.second->set_active(false);
            it.second->set_visible(false);
        }

        return;
    }
    else if (m_enemies.empty())
    {
        set_next("dungeon");
        m_result = env->resources_manager->get_texture("res/images/combat/you-win.png");
        m_state = FINISHED_COMBAT;

        for (auto it : m_characters)
        {
            it.second->set_visible(false);
        }

        return;
    }

    if (not m_last or m_state == CHARACTER_ATTACK)
    {
        m_last = elapsed;
        return;
    }
    else if (elapsed - m_last < DELAY)
    {
        return;
    }
    else if (elapsed - m_last < DELAY * 2)
    {
        m_state = ENEMY_ATTACK;
        return;
    }

    m_last = elapsed;
    m_attacker = m_attackers.begin()->second;
    
    Character *enemy = (m_enemies.find(m_attacker) != m_enemies.end() ?
        m_enemies[m_attacker] : nullptr);
    Character *character = (m_characters.find(m_attacker) != m_characters.end() ?
        m_characters[m_attacker] : nullptr);

    if (enemy)
    {
        enemy_attack(enemy);
        m_enemy_turn = enemy;
    }
    else if (character)
    {
        m_state = CHARACTER_ATTACK;
        m_enemy_turn = nullptr;
    }

    m_attackers.erase(m_attackers.begin());
}

void
Combat::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    env->canvas->draw(m_texture.get());

    env->canvas->set_blend_mode(Canvas::BLEND);
    env->canvas->fill(Rect(0, 0, env->canvas->w(),  env->canvas->h()), Color(0, 0, 0, 128));
    env->canvas->set_blend_mode(Canvas::NONE);

    if (m_state == SHOW_DAMAGE)
    {
        m_text->draw();
    }
    else if (m_state == FINISHED_COMBAT)
    {
        int x = ((env->canvas->w() - m_result->w()) / 2 / W) * env->canvas->w();
        int y = ((env->canvas->h() - m_result->h()) / 2 / H) * env->canvas->h();

        env->canvas->draw(m_result.get(), x, y);
    }
    
    if (m_enemy_turn)
    {
        double x = m_enemy_turn->x() - (5 * W / env->canvas->w());
        double y = m_enemy_turn->y() - (5 * H / env->canvas->h());
        double w = m_enemy_turn->w() + (10 * W / env->canvas->w());
        double h = m_enemy_turn->h() + (10 * H / env->canvas->h());
        
        Rect rect { x, y, w, h };
        env->canvas->fill(rect, Color::BLUE);
    }

    double x = 30 * env->canvas->w() / W;
    auto it = m_attackers.begin();
    for (int i = 1; i <= 12; ++i)
    {
        string attacker = it->second;

        m_attacker_icon = env->resources_manager->get_texture("res/images/combat/" +
            attacker + ".png");

        env->canvas->draw(m_attacker_icon.get(), x, 25 * env->canvas->h() / H);

        x += 82 * env->canvas->w() / W;
        ++it;
    }
}

bool
Combat::on_message(Object *sender, MessageID id, Parameters)
{
    Character *character = dynamic_cast<Character *>(sender);

    if (not character or m_state != CHARACTER_ATTACK)
    {
        return false;
    }

    Character *attacker = m_characters[m_attacker];
    Character *enemy = m_enemies[id];

    Environment *env = Environment::get_instance();

    int damage = enemy->receive_damage(attacker);
    set_text("-" + to_string(damage), Color::RED);
    
    m_text->set_position(enemy->x() + enemy->w() / 2 - m_text->w() / 2,
        enemy->y() + enemy->h() + ((10 / H) * env->canvas->h()));

    env->sfx->play("res/sfx/uiTavern_Ghost2.ogg", 1);

    if (enemy->life() <= 0)
    {
        enemy->remove_observer(this);
        remove_child(enemy);

        m_enemies.erase(id);
        delete enemy;
    }

    m_state = SHOW_DAMAGE;
    update_attackers(attacker);

    return true;
}

void
Combat::load_characters()
{
    Environment *env = Environment::get_instance();
    
    int y = (620 / H) * env->canvas->h();

    double scale_w = env->canvas->w() / W;

    Character *character = new Character(m_slot, this, "albert", "albert.png", 29 * scale_w, y);
    m_characters[character->id()] = character;

    character = new Character(m_slot, this, "booker", "booker.png", 276 * scale_w, y);
    m_characters[character->id()] = character;

    character = new Character(m_slot, this, "isaac", "isaac.png", 525 * scale_w, y);
    m_characters[character->id()] = character;

    character = new Character(m_slot, this, "newton", "newton.png", 773 * scale_w, y);
    m_characters[character->id()] = character;

    for (auto it : m_characters)
    {
        it.second->add_observer(this);
        it.second->set_active(false);
        add_child(it.second);

        for (int i = 1; i <= 12; ++i)
        {
            it.second->set_attacks_quantity(i);

            int new_cooldown = it.second->cooldown() * i;

            m_attackers.insert(pair<int, string>(new_cooldown, it.second->id()));
        }
    }
}

void
Combat::load_enemies()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int w = 170 * scale_w;
    int h = 265 * scale_h;
    int i = 0;

    Character *enemy = new Character(m_slot, this, "technopus" + to_string(++i), "technopus.png",
        58 * scale_w, 275 * scale_h, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(m_slot, this, "technopus" + to_string(++i), "technopus.png",
        194 * scale_w, 322 * scale_h, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(m_slot, this, "technopus" + to_string(++i), "technopus.png",
        330 * scale_w, 275 * scale_h, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(m_slot, this, "technopus" + to_string(++i), "technopus.png",
        530 * scale_w, 275 * scale_h, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(m_slot, this, "technopus" + to_string(++i), "technopus.png",
        665 * scale_w, 322 * scale_h, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(m_slot, this, "technopus" + to_string(++i), "technopus.png",
        800 * scale_w, 275 * scale_h, w, h);
    m_enemies[enemy->id()] = enemy;

    for (auto it : m_enemies)
    {
        it.second->add_observer(this);
        add_child(it.second);

        for (int i = 1; i <= 12; ++i)
        {
            it.second->set_attacks_quantity(i);

            int new_cooldown = it.second->cooldown() * i;

            m_attackers.insert(pair<int, string>(new_cooldown, it.second->id()));
        }
    }
}

void
Combat::enemy_attack(Character* enemy)
{
    Character *character = m_characters.begin()->second;

    Environment *env = Environment::get_instance();
    
    int damage = character->receive_damage(enemy);
    set_text("-" + to_string(damage), Color::RED);
    m_text->set_position(character->x() + character->w() / 2 - m_text->w() / 2,
        character->y() - m_text->h() - ((10 / H) * env->canvas->h()));

    env->sfx->play("res/sfx/uiTavern_Enforcer.ogg", 1);


    if (character->life() <= 0)
    {
        character->remove_observer(this);
        remove_child(character);

        m_characters.erase(character->id());
        delete character;
    }

    update_attackers(enemy);
    m_state = SHOW_DAMAGE;
}

void
Combat::update_attackers(Character* character)
{
    int attacks_quantity = character->attacks_quantity() + 1;
    character->set_attacks_quantity(attacks_quantity);

    int new_cooldown = character->cooldown() * attacks_quantity;

    m_attackers.insert(pair<int, string>(new_cooldown, character->id()));
}

bool
Combat::on_event(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED and m_state == FINISHED_COMBAT)
    {
        finish();

        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiBattle_Escape.ogg", 1);

        return true;
    }

    return false;
}

void
Combat::set_text(const string& str, const Color& color)
{
    if (m_text)
    {
        delete m_text;
        m_text = nullptr;
    }

    m_text = new Text(this, str, color);
}
