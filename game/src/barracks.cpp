#include "barracks.h"
#include "character.h"
#include "colony.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>
#include <iostream>
using namespace std;

#define W 1024.0
#define H 768.0

using std::to_string;

Barracks::Barracks(const string& next)
    : Level("barracks", next), m_settings(nullptr), m_big_card(nullptr),
    m_bracket(nullptr), m_bracket_m(nullptr), m_bracket_p(nullptr), m_bracket_t(nullptr),
    m_attributes(nullptr), m_stats(nullptr), m_levelup(nullptr),
    m_military(nullptr), m_psionic(nullptr), m_tech(nullptr), m_equip(nullptr),
    m_rifle(nullptr), m_armor(nullptr), m_shield(nullptr), m_equip_change(nullptr), m_character(0)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/barracks/";

    Button *button = new Button(this, "left_arrow", path + "left_arrow.png",
        (60/W) * env->canvas->w(), (218/H) * env->canvas->h(), 26, 45);
    button->set_sprites(1);
    m_buttons[button->id()] = button;

    button = new Button(this, "right_arrow", path + "right_arrow.png",
        (362/W) * env->canvas->w(), (218/H) * env->canvas->h(), 26, 45);
    button->set_sprites(1);
    m_buttons[button->id()] = button;

    for (auto b : m_buttons)
    {
        b.second->add_observer(this);
        add_child(b.second);
    }

    m_big_card = env->resources_manager->get_texture(path + "card_big.png");
    m_bracket = env->resources_manager->get_texture(path + "bracket.png");
    m_bracket_m = env->resources_manager->get_texture(path + "bracket-m.png");
    m_bracket_p = env->resources_manager->get_texture(path + "bracket-p.png");
    m_bracket_t = env->resources_manager->get_texture(path + "bracket-t.png");
    m_attributes = env->resources_manager->get_texture(path + "attributes.png");
    m_stats = env->resources_manager->get_texture(path + "stats.png");
    m_levelup = env->resources_manager->get_texture(path + "levelup.png");
    m_military = env->resources_manager->get_texture(path + "military.png");
    m_psionic = env->resources_manager->get_texture(path + "psionic.png");
    m_tech = env->resources_manager->get_texture(path + "tech.png");
    m_equip = env->resources_manager->get_texture(path + "equip_shelf.png");
    m_rifle  = env->resources_manager->get_texture(path + "rifle.png");
    m_armor  = env->resources_manager->get_texture(path + "armor.png");
    m_shield  = env->resources_manager->get_texture(path + "shield.png");
    m_equip_change = env->resources_manager->get_texture(path + "equip_change.png");

    load_characters();

    m_settings = env->resources_manager->get_settings("res/datas/characters.sav");
}

void
Barracks::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_big_card.get(), (112 / W) * env->canvas->w(), (130 / H) * env->canvas->h());
    env->canvas->draw(m_bracket.get(), (30 / W) * env->canvas->w(), (25 / H) * env->canvas->h());

    int y = (470 / H) * env->canvas->h();
    env->canvas->draw(m_bracket_m.get(), (112 / W) * env->canvas->w(), y);
    env->canvas->draw(m_bracket_p.get(), (402 / W) * env->canvas->w(), y);
    env->canvas->draw(m_bracket_t.get(), (690 / W) * env->canvas->w(), y);

    y = (485 / H) * env->canvas->h();
    env->canvas->draw(m_military.get(), (135 / W) * env->canvas->w(), y);
    env->canvas->draw(m_psionic.get(), (420 / W) * env->canvas->w(), y);
    env->canvas->draw(m_tech.get(), (710 / W) * env->canvas->w(), y);


    y = (128 / H) * env->canvas->h();

    env->canvas->draw(m_attributes.get(), (402 / W) * env->canvas->w(), y);
    env->canvas->draw(m_stats.get(), (690 / W) * env->canvas->w(), y);
    
    env->canvas->draw(m_levelup.get(), (402 / W) * env->canvas->w(), (322 / H) * env->canvas->h());
    
    // x = (670/W) * env->canvas->w();
    // y = (353/H) * env->canvas->h();
    // texture = env->resources_manager->get_texture(path + "equip_change.png");
    // env->canvas->draw(texture.get(), x, y);

    // x = (130/W) * env->canvas->w();
    // y = (530/H) * env->canvas->h();

    // for (int i = 1; i <= 5; ++i, x += 38)
    // {
    //     texture = env->resources_manager->get_texture(path + "Skill_M_" + to_string(i) + ".png");
    //     env->canvas->draw(texture.get(), x, y);
    // }

    // for (int i = 0, x = 130; i < 5; ++i, x += 38)
    // {
    //     for (int j = 0, y = 567; j < 3; ++j, y += 37)
    //     {
    //         texture = env->resources_manager->get_texture(path + "Skill_M_Locked.png");
    //         env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
    //     }
    // }

    // x = (420/W) * env->canvas->w();

    // for (int i = 1; i <= 5; ++i, x += 38)
    // {
    //     texture = env->resources_manager->get_texture(path + "Skill_P_" + to_string(i) + ".png");
    //     env->canvas->draw(texture.get(), x, y);
    // }

    // for (int i = 0, x = 420; i < 5; ++i, x += 38)
    // {
    //     for (int j = 0, y = 567; j < 3; ++j, y += 37)
    //     {
    //         texture = env->resources_manager->get_texture(path + "Skill_P_Locked.png");
    //         env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
    //     }
    // }

    // x = (710/W) * env->canvas->w();

    // for (int i = 1; i <= 5; ++i, x += 38)
    // {
    //     texture = env->resources_manager->get_texture(path + "Skill_T_" + to_string(i) + ".png");
    //     env->canvas->draw(texture.get(), x, y);
    // }

    // for (int i = 0, x = 710; i < 5; ++i, x += 38)
    // {
    //     for (int j = 0, y = 567; j < 3; ++j, y += 37)
    //     {
    //         texture = env->resources_manager->get_texture(path + "Skill_T_Locked.png");
    //         env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
    //     }
    // }

    // shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    // env->canvas->set_font(font);
    // font->set_size(32);
    // Color color(170, 215, 190);

    // auto it = m_settings->sections().begin();
    // for (int i = 0; i < m_character; ++it, ++i); // not very well with other ++ operators

    // string name = it->first;
    // string levelup = it->second["levelup"];
    // string military = it->second["military"];
    // string psionic = it->second["psionic"];
    // string tech = it->second["tech"];
    // string might = it->second["might"];
    // string mind = it->second["mind"];
    // string perception = it->second["perception"];
    // string agility = it->second["agility"];
    // string might_attack = it->second["might_attack"];
    // string mind_attack = it->second["mind_attack"];
    // string cooldown = it->second["cooldown"];
    // string defense = it->second["defense"];
    // string might_armor = it->second["might_armor"];
    // string mind_armor = it->second["mind_armor"];
    // string critical = it->second["critical"];

    // if (levelup.back() == '\r')
    // {
    //     levelup.pop_back();
    //     military.pop_back();
    //     psionic.pop_back();
    //     tech.pop_back();
    //     might.pop_back();
    //     mind.pop_back();
    //     perception.pop_back();
    //     agility.pop_back();
    //     might_attack.pop_back();
    //     mind_attack.pop_back();
    //     cooldown.pop_back();
    //     defense.pop_back();
    //     might_armor.pop_back();
    //     mind_armor.pop_back();
    //     critical.pop_back();
    // }

    // env->canvas->draw(name, (112/W) * env->canvas->w(), (90/H) * env->canvas->h(), color);

    // font->set_size(16);
    // env->canvas->draw(name, (250/W) * env->canvas->w(), (129/H) * env->canvas->h(), color);
    // font->set_size(18);
    // env->canvas->draw(military, (295/W) * env->canvas->w(), (167/H) * env->canvas->h(), Color(208, 179, 43));
    // env->canvas->draw(psionic, (295/W) * env->canvas->w(), (198/H) * env->canvas->h(), Color(166, 69, 151));
    // env->canvas->draw(tech, (295/W) * env->canvas->w(), (229/H) * env->canvas->h(), Color(78, 191, 190));

    // font->set_size(22);
    // env->canvas->draw(levelup, (500/W) * env->canvas->w(), (370/H) * env->canvas->h(), Color(62, 108, 236));

    // font->set_size(12);
    // env->canvas->draw(might, (607/W) * env->canvas->w(), (165/H) * env->canvas->h(), color);
    // env->canvas->draw(mind, (607/W) * env->canvas->w(), (200/H) * env->canvas->h(), color);
    // env->canvas->draw(perception, (607/W) * env->canvas->w(), (235/H) * env->canvas->h(), color);
    // env->canvas->draw(agility, (607/W) * env->canvas->w(), (270/H) * env->canvas->h(), color);

    // color = Color(82, 104, 93);
    // env->canvas->draw(might_attack, (780/W) * env->canvas->w(), (165/H) * env->canvas->h(), color);
    // env->canvas->draw(mind_attack, (780/W) * env->canvas->w(), (200/H) * env->canvas->h(), color);
    // env->canvas->draw(cooldown, (780/W) * env->canvas->w(), (235/H) * env->canvas->h(), color);
    // env->canvas->draw(defense, (780/W) * env->canvas->w(), (270/H) * env->canvas->h(), color);

    // env->canvas->draw(might_armor, (900/W) * env->canvas->w(), (165/H) * env->canvas->h(), color);
    // env->canvas->draw(mind_armor, (900/W) * env->canvas->w(), (200/H) * env->canvas->h(), color);
    // env->canvas->draw(critical, (900/W) * env->canvas->w(), (235/H) * env->canvas->h(), color);
    // env->canvas->draw(critical, (900/W) * env->canvas->w(), (270/H) * env->canvas->h(), color);

    // font->set_size(16);
    // auto settings = env->resources_manager->get_settings("res/datas/colony.sav");
    // int data = settings->read<int>("Colony", "data", 0);
    // env->canvas->draw(to_string(data), (500/W) * env->canvas->w(), (415/H) * env->canvas->h(), Color(170, 215, 190));
}

bool
Barracks::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    if (button->id() == "left_arrow")
    {
        if (--m_character < 0)
        {
            m_character = m_settings->sections().size() - 1;
        }
    }
    else if (button->id() == "right_arrow")
    {
        m_character = (m_character + 1) % m_settings->sections().size();
    }

    return true;
}

void
Barracks::load_characters()
{
    Environment *env = Environment::get_instance();
    
    int x = (112 / W) * env->canvas->w();
    int y = (130 / H) * env->canvas->h();
    int w = (222 / W) * env->canvas->w();
    int h = (270 / H) * env->canvas->h();

    Character *character = new Character(this, "Isaac", "psionic_big.png", x, y, w, h, "Isaac");
    m_characters[character->id()] = character;

    // Character *character = new Character(this, "Albert", "psionic_big.png", x, y, w, h, "Albert");
    // m_characters[character->id()] = character;

    // Character *character = new Character(this, "Newton", "psionic_big.png", x, y, w, h, "Newton");
    // m_characters[character->id()] = character;

    // Character *character = new Character(this, "Clarke", "psionic_big.png", x, y, w, h, "Clarke");
    // m_characters[character->id()] = character;

    // Character *character = new Character(this, "Brooker", "psionic_big.png", x, y, w, h, "Brooker");
    // m_characters[character->id()] = character;

    // Character *character = new Character(this, "Michael", "psionic_big.png", x, y, w, h, "Michael");
    m_characters[character->id()] = character;

    for (auto it : m_characters)
    {
        it.second->add_observer(this);
        add_child(it.second);
        it.second->set_active(false);
    }
}
