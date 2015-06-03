#include "combat.h"
#include "character.h"

Combat::Combat(const string& next, const string& image)
    : Level("combat", next), m_texture(nullptr)
{
	Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(image);

    load_characters();
}

void
Combat::draw_self()
{
    Environment *env = Environment::get_instance();
    
    env->canvas->clear();
    env->canvas->draw(m_texture.get());
}

void
Combat::load_characters()
{
	Character* character = new Character(this, "god_of_war", "god_of_war.png", 0,
	    0);

	m_characters[character->id()] = character;

	character = new Character(this, "luigi", "luigi.png", 500,
	    300);

	m_characters[character->id()] = character;

	for (auto it : m_characters)
	{
	    it.second->add_observer(this);
	    add_child(it.second);
	}
}
