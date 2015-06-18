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
    : Level("barracks", next), m_settings(nullptr), m_character(0)
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

    m_textures["card_big"] = env->resources_manager->get_texture(path + "card_big.png");
    m_textures["bracket"] = env->resources_manager->get_texture(path + "bracket.png");
    m_textures["bracket_m"] = env->resources_manager->get_texture(path + "bracket-m.png");
    m_textures["bracket_p"] = env->resources_manager->get_texture(path + "bracket-p.png");
    m_textures["bracket_t"] = env->resources_manager->get_texture(path + "bracket-t.png");
    m_textures["attributes"] = env->resources_manager->get_texture(path + "attributes.png");
    m_textures["stats"] = env->resources_manager->get_texture(path + "stats.png");
    m_textures["levelup"] = env->resources_manager->get_texture(path + "levelup.png");
    m_textures["military"] = env->resources_manager->get_texture(path + "military.png");
    m_textures["psionic"] = env->resources_manager->get_texture(path + "psionic.png");
    m_textures["tech"] = env->resources_manager->get_texture(path + "tech.png");
    m_textures["equip"] = env->resources_manager->get_texture(path + "equip_shelf.png");
    m_textures["rifle"] = env->resources_manager->get_texture(path + "rifle.png");
    m_textures["armor"] = env->resources_manager->get_texture(path + "armor.png");
    m_textures["shield"] = env->resources_manager->get_texture(path + "shield.png");
    m_textures["equip_change"] = env->resources_manager->get_texture(path + "equip_change.png");

    load_characters();

}

void
Barracks::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    env->canvas->draw(m_textures["card_big"].get(), (112 / W) * env->canvas->w(), (130 / H) * env->canvas->h());
    env->canvas->draw(m_textures["bracket"].get(), (30 / W) * env->canvas->w(), (25 / H) * env->canvas->h());

    int y = (470 / H) * env->canvas->h();
    env->canvas->draw(m_textures["bracket_m"].get(), (112 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["bracket_p"].get(), (402 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["bracket_t"].get(), (690 / W) * env->canvas->w(), y);

    y = (485 / H) * env->canvas->h();
    env->canvas->draw(m_textures["military"].get(), (135 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["psionic"].get(), (420 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["tech"].get(), (710 / W) * env->canvas->w(), y);

    y = (128 / H) * env->canvas->h();
    env->canvas->draw(m_textures["attributes"].get(), (402 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["stats"].get(), (690 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["levelup"].get(), (402 / W) * env->canvas->w(), (322 / H) * env->canvas->h());

    draw_character();

    shared_ptr<Texture> texture;
    string path = "res/images/colony/barracks/";

    int x = 130;
    y = (530/H) * env->canvas->h();

    for (int i = 1; i <= 5; ++i, x += 38)
    {
        texture = env->resources_manager->get_texture(path + "Skill_M_" + to_string(i) + ".png");
        env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), y);
    }

    for (int i = 0, x = 130; i < 5; ++i, x += 38)
    {
        for (int j = 0, y = 567; j < 3; ++j, y += 37)
        {
            texture = env->resources_manager->get_texture(path + "Skill_M_Locked.png");
            env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
        }
    }

    x = 420;

    for (int i = 1; i <= 5; ++i, x += 38)
    {
        texture = env->resources_manager->get_texture(path + "Skill_P_" + to_string(i) + ".png");
        env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), y);
    }

    for (int i = 0, x = 420; i < 5; ++i, x += 38)
    {
        for (int j = 0, y = 567; j < 3; ++j, y += 37)
        {
            texture = env->resources_manager->get_texture(path + "Skill_P_Locked.png");
            env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
        }
    }

    x = 710;

    for (int i = 1; i <= 5; ++i, x += 38)
    {
        texture = env->resources_manager->get_texture(path + "Skill_T_" + to_string(i) + ".png");
        env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), y);
    }

    for (int i = 0, x = 710; i < 5; ++i, x += 38)
    {
        for (int j = 0, y = 567; j < 3; ++j, y += 37)
        {
            texture = env->resources_manager->get_texture(path + "Skill_T_Locked.png");
            env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
        }
    }
}

void
Barracks::draw_character()
{
    auto it = m_characters.begin();
    for (int i = 0; i < m_character; ++it, ++i) {}; // not work very well with other ++ operators

    Character *character = it->second;

    character->set_visible(true);

    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(32);
    Color color(170, 215, 190);
    
    env->canvas->draw(character->id(), (112 / W) * env->canvas->w(), (90 / H) * env->canvas->h(), color);

    font->set_size(14);
    int x = (607 / W) * env->canvas->w();
    env->canvas->draw(to_string(character->might()), x, (165 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->mind()), x, (200 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->perception()), x, (235 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->agility()), x, (270 / H) * env->canvas->h(), color);

    font->set_size(12);
    x = (780 / W) * env->canvas->w();
    env->canvas->draw(to_string(character->might_attack()), x, (165 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->mind_attack()), x, (200 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->cooldown()), x, (235 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->defense()), x, (270 / H) * env->canvas->h(), color);

    x = (900 / W) * env->canvas->w();
    env->canvas->draw(to_string(character->might_armor()), x, (165 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->mind_armor()), x, (200 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->critical()), x, (235 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->critical()), x, (270 / H) * env->canvas->h(), color);

    font->set_size(22);
    env->canvas->draw(to_string(character->levelup()), (500/W) * env->canvas->w(),
        (370/H) * env->canvas->h(), Color(62, 108, 236));

    font->set_size(16);
    auto settings = env->resources_manager->get_settings("res/datas/colony.sav");
    int data = settings->read<int>("Colony", "data", 0);
    env->canvas->draw(to_string(data), (500/W) * env->canvas->w(),
        (415/H) * env->canvas->h(), Color(170, 215, 190));

    font->set_size(18);
    env->canvas->draw(character->id(), (250/W) * env->canvas->w(), (132/H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->military()), (295/W) * env->canvas->w(),
        (167/H) * env->canvas->h(), Color(208, 179, 43));
    env->canvas->draw(to_string(character->psionic()), (295/W) * env->canvas->w(),
        (198/H) * env->canvas->h(), Color(166, 69, 151));
    env->canvas->draw(to_string(character->tech()), (295/W) * env->canvas->w(),
        (229/H) * env->canvas->h(), Color(78, 191, 190));
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
            m_character = m_characters.size() - 1;
        }
    }
    else if (button->id() == "right_arrow")
    {
        m_character = (m_character + 1) % m_characters.size();
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

    auto settings = env->resources_manager->get_settings("res/datas/characters.sav");
    auto sections = settings->sections();

    for (auto section : sections)
    {
        Character *character = new Character(this, section.first, "psionic_big.png", x, y, w, h);
        m_characters[character->id()] = character;
    }

    for (auto it : m_characters)
    {
        it.second->add_observer(this);
        add_child(it.second);

        it.second->set_active(false);
        it.second->set_visible(false);
    }
}
