#include "combat.h"
#include "character.h"
#include <core/font.h>

using std::make_pair;
using std::to_string;

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

    m_last = elapsed;
    m_state = ENEMY_ATTACK;
    
    if (m_state == ENEMY_ATTACK)
    {
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
        font->set_size(20);
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
    receiver(enemy->x() + enemy->w() / 2, enemy->y() + enemy->h() + (10 / H * env->canvas->h()));

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
    Character *character = new Character(this, "luigi", "luigi.png",
        500, 300, 200, 300);
    character->set_cooldown(5);
    character->set_attack(50);

    m_characters[character->id()] = character;

    character = new Character(this, "link", "link.png",
        300, 300, 200, 300);
    character->set_cooldown(3);

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
    Character *enemy = new Character(this, "god_of_war", "god_of_war.png",
        0, 0, 200, 300);
    enemy->set_life(30);
    enemy->set_attack(50);
    enemy->set_cooldown(2);

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
    receiver(character->x() + character->w() / 2, character->y() - (10 / H * env->canvas->h()));

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
