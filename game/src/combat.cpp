#include "combat.h"
#include "character.h"

Combat::Combat(const string& next, const string& image)
    : Level("combat", next), m_texture(nullptr), m_attacker(0)
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

    auto it = m_characters.begin();
    for(int i = 0; i < m_attacker; ++i, ++it); //Not work well as other operators ++
    
    m_attacker++;
    m_attacker %= m_characters.size();

    Character *attacker = it->second;
    Character *enemy = m_enemies[id];

    enemy->receive_damage(attacker->attack());

    if (enemy->life() <= 0)
    {
        enemy->remove_observer(this);
        remove_child(enemy);

        delete enemy;
    }

    return true;
}

void
Combat::load_characters()
{
    Character *character = new Character(this, "luigi", "luigi.png", 500,
        300, 200, 300);

    m_characters[character->id()] = character;

    character = new Character(this, "link", "link.png", 300,
        300, 200, 300);

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
    Character *enemy = new Character(this, "god_of_war", "god_of_war.png", 0,
        0, 200, 300);

    m_enemies[enemy->id()] = enemy;

    for (auto it : m_enemies)
    {
        it.second->add_observer(this);
        add_child(it.second);
    }
}
