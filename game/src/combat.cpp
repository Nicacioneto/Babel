#include "combat.h"
#include "character.h"
#include <core/font.h>

using std::make_pair;
using std::to_string;

#define W 1024.0
#define H 768.0
#define DELAY 1000

Combat::Combat(const string& next, const string& image)
    : Level("combat", next), m_texture(nullptr), m_attacker(""), m_state(ENEMY_ATTACK), m_last(0),
    m_damage(0)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(image);

    load_characters();
    load_enemies();
}

void
Combat::update_self(unsigned long elapsed)
{
    if (m_characters.empty())
    {
        set_next("base");
        finish();
    }
    else if (m_enemies.empty())
    {
        set_next("dungeon");
        finish();
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
    }
    else if (character)
    {
        m_state = CHARACTER_ATTACK;
    }

    m_attackers.erase(m_attackers.begin());
}

void
Combat::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    env->canvas->draw(m_texture.get());

    if (m_state == SHOW_DAMAGE)
    {
        shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
        env->canvas->set_font(font);
        font->set_size(25);
        env->canvas->draw("-" + to_string(m_damage), m_receiver.first, m_receiver.second, Color::RED);
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

    m_damage = enemy->receive_damage(attacker);

    Environment *env = Environment::get_instance();
    receiver(enemy->x() + enemy->w() / 2, ((enemy->y() + enemy->h() + 10) / H * env->canvas->h()));

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
    
    int y = ((env->canvas->h() - 300) / H) * env->canvas->h();
    int w = (200 / W) * env->canvas->w();
    int h = (300 / H) * env->canvas->h();

    Character *character = new Character(this, "kenny1", "kenny.png", 0, y, w, h);
    m_characters[character->id()] = character;

    character = new Character(this, "kenny2", "kenny.png", (250 / W) * env->canvas->w(), y, w, h);
    m_characters[character->id()] = character;

    character = new Character(this, "kenny3", "kenny.png", (550 / W) * env->canvas->w(), y, w, h);
    m_characters[character->id()] = character;

    character = new Character(this, "kenny4", "kenny.png", (800 / W) * env->canvas->w(), y, w, h);
    m_characters[character->id()] = character;

    for (auto it : m_characters)
    {
        it.second->add_observer(this);
        add_child(it.second);
        it.second->set_active(false);

        m_attackers.insert(pair<int, string>(it.second->cooldown(), it.second->id()));
    }
}

void
Combat::load_enemies()
{
    Environment *env = Environment::get_instance();

    int w = (200 / W) * env->canvas->w();
    int h = (300 / H) * env->canvas->h();

    Character *enemy = new Character(this, "timber1", "timber.png", 0, 0, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(this, "timber2", "timber.png", (250 / W) * env->canvas->w(), 0, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(this, "timber3", "timber.png", (550 / W) * env->canvas->w(), 0, w, h);
    m_enemies[enemy->id()] = enemy;

    enemy = new Character(this, "timber4", "timber.png", (800 / W) * env->canvas->w(), 0, w, h);
    m_enemies[enemy->id()] = enemy;

    for (auto it : m_enemies)
    {
        it.second->add_observer(this);
        add_child(it.second);

        m_attackers.insert(pair<int, string>(it.second->cooldown(), it.second->id()));
    }
}

void
Combat::enemy_attack(Character* enemy)
{
    Character *character = m_characters.begin()->second;

    m_damage = character->receive_damage(enemy);

    Environment *env = Environment::get_instance();
    receiver(character->x() + character->w() / 2, ((character->y() - 35) / H * env->canvas->h()));

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

void
Combat::receiver(double x, double y)
{
    m_receiver = make_pair(x, y);
}
