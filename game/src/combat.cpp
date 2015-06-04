#include "combat.h"
#include "character.h"

Combat::Combat(const string& next, const string& image)
    : Level("combat", next), m_texture(nullptr), m_character_attacker(0), m_enemy_attacker(0)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(image);

    load_characters();
    load_enemies();
}

void
Combat::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    env->canvas->draw(m_texture.get());
}

bool
Combat::on_message(Object *sender, MessageID id, Parameters)
{
    Character *character = dynamic_cast<Character *>(sender);

    if (not character or m_characters.find(character->id()) != m_characters.end())
    {
        return false;
    }
    else if (not m_characters.size())
    {
        set_next("base");
        finish();
        return false;
    }

    auto it = m_characters.begin();
    m_character_attacker %= m_characters.size();
    for (int i = 0; i < m_character_attacker; ++i, ++it); // Not work well as other ++ operators
    ++m_character_attacker;

    Character *attacker = it->second;
    Character *enemy = m_enemies[id];

    enemy->receive_damage(attacker->attack());

    if (enemy->life() <= 0)
    {
        enemy->remove_observer(this);
        remove_child(enemy);

        m_enemies.erase(id);
        delete enemy;

        if (not m_enemies.size())
        {
            set_next("dungeon");
            finish();
        }
    }
    else
    {
        enemy_attack();
    }

    return true;
}

void
Combat::load_characters()
{
    Character *character = new Character(this, "luigi", "luigi.png",
        500, 300, 200, 300);

    m_characters[character->id()] = character;

    character = new Character(this, "link", "link.png",
        300, 300, 200, 300);

    m_characters[character->id()] = character;

    for (auto it : m_characters)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}

void
Combat::load_enemies()
{
    Character *enemy = new Character(this, "god_of_war", "god_of_war.png",
        0, 0, 200, 300);
    enemy->set_life(30);
    enemy->set_attack(50);

    m_enemies[enemy->id()] = enemy;

    for (auto it : m_enemies)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}

void
Combat::enemy_attack()
{
    auto it = m_enemies.begin();
    m_enemy_attacker %= m_enemies.size();
    for (int i = 0; i < m_enemy_attacker; ++i, ++it); // Not work well as other ++ operators
    ++m_enemy_attacker;

    Character *enemy = it->second;
    Character *character = m_characters.begin()->second;

    character->receive_damage(enemy->attack());

    if (character->life() <= 0)
    {
        character->remove_observer(this);
        remove_child(character);

        m_characters.erase(character->id());
        delete character;
    }
}
