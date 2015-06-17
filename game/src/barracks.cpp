#include "colony.h"
#include "barracks.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Barracks::Barracks(const string& next)
    : Level("barracks", next)
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
}

void
Barracks::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();
    shared_ptr<Texture> texture;
    string path = "res/images/colony/";

    int x = (112/W) * env->canvas->w();
    int y = (130/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "characters/psionic_big.png");
    env->canvas->draw(texture.get(), x, y);
    texture = env->resources_manager->get_texture(path + "characters/card_big.png");
    env->canvas->draw(texture.get(), x, y);
    
    path += "barracks/";

    x = (30/W) * env->canvas->w();
    y = (25/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "bracket.png");
    env->canvas->draw(texture.get(), x, y);

    x = (112/W) * env->canvas->w();
    y = (470/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "bracket-m.png");
    env->canvas->draw(texture.get(), x, y);

    x = (402/W) * env->canvas->w();
    texture = env->resources_manager->get_texture(path + "bracket-m.png");
    env->canvas->draw(texture.get(), x, y);

    x = (690/W) * env->canvas->w();
    texture = env->resources_manager->get_texture(path + "bracket-m.png");
    env->canvas->draw(texture.get(), x, y);

    x = (402/W) * env->canvas->w();
    y = (128/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "attributes.png");
    env->canvas->draw(texture.get(), x, y);
    
    y = (322/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "levelup.png");
    env->canvas->draw(texture.get(), x, y);

    x = (690/W) * env->canvas->w();
    y = (128/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "stats.png");
    env->canvas->draw(texture.get(), x, y);

    y = (322/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "equip_shelf.png");
    env->canvas->draw(texture.get(), x, y);

    y = (360/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "rifle.png");
    env->canvas->draw(texture.get(), x, y);

    x = (772/W) * env->canvas->w();
    texture = env->resources_manager->get_texture(path + "armor.png");
    env->canvas->draw(texture.get(), x, y);

    x = (855/W) * env->canvas->w();
    texture = env->resources_manager->get_texture(path + "shield.png");
    env->canvas->draw(texture.get(), x, y);
    
    x = (670/W) * env->canvas->w();
    y = (353/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "equip_change.png");
    env->canvas->draw(texture.get(), x, y);

    x = (135/W) * env->canvas->w();
    y = (485/H) * env->canvas->h();
    texture = env->resources_manager->get_texture(path + "military.png");
    env->canvas->draw(texture.get(), x, y);

    x = (420/W) * env->canvas->w();
    texture = env->resources_manager->get_texture(path + "psionic.png");
    env->canvas->draw(texture.get(), x, y);

    x = (710/W) * env->canvas->w();
    texture = env->resources_manager->get_texture(path + "tech.png");
    env->canvas->draw(texture.get(), x, y);

    x = (130/W) * env->canvas->w();
    y = (530/H) * env->canvas->h();

    for (int i = 1; i <= 5; ++i, x += 38)
    {
        texture = env->resources_manager->get_texture(path + "Skill_M_" + to_string(i) + ".png");
        env->canvas->draw(texture.get(), x, y);
    }

    for (int i = 0, x = 130; i < 5; ++i, x += 38)
    {
        for (int j = 0, y = 567; j < 3; ++j, y += 37)
        {
            texture = env->resources_manager->get_texture(path + "Skill_M_Locked.png");
            env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
        }
    }

    x = (420/W) * env->canvas->w();

    for (int i = 1; i <= 5; ++i, x += 38)
    {
        texture = env->resources_manager->get_texture(path + "Skill_P_" + to_string(i) + ".png");
        env->canvas->draw(texture.get(), x, y);
    }

    for (int i = 0, x = 420; i < 5; ++i, x += 38)
    {
        for (int j = 0, y = 567; j < 3; ++j, y += 37)
        {
            texture = env->resources_manager->get_texture(path + "Skill_P_Locked.png");
            env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
        }
    }

    x = (710/W) * env->canvas->w();

    for (int i = 1; i <= 5; ++i, x += 38)
    {
        texture = env->resources_manager->get_texture(path + "Skill_T_" + to_string(i) + ".png");
        env->canvas->draw(texture.get(), x, y);
    }

    for (int i = 0, x = 710; i < 5; ++i, x += 38)
    {
        for (int j = 0, y = 567; j < 3; ++j, y += 37)
        {
            texture = env->resources_manager->get_texture(path + "Skill_T_Locked.png");
            env->canvas->draw(texture.get(), (x/W) * env->canvas->w(), (y/H) * env->canvas->h());
        }
    }

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(32);
    Color color(170, 215, 190);

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/barracks.sav");
    auto sections = settings->sections();

    for (auto character : sections)
    {
        string name = character.first;
        string level = character.second["level"];
        string levelup = character.second["levelup"];
        string m = character.second["m"];
        string p = character.second["p"];
        string t = character.second["t"];
        string might = character.second["might"];
        string mind = character.second["mind"];
        string perception = character.second["perception"];
        string agility = character.second["agility"];
        string attack = character.second["attack"];
        string speed = character.second["speed"];
        string defence = character.second["defence"];
        string armor = character.second["armor"];
        string critical = character.second["critical"];

        if (level.back() == '\r')
        {
            level.pop_back();
            levelup.pop_back();
            m.pop_back();
            p.pop_back();
            t.pop_back();
            might.pop_back();
            mind.pop_back();
            perception.pop_back();
            agility.pop_back();
            attack.pop_back();
            speed.pop_back();
            defence.pop_back();
            armor.pop_back();
            critical.pop_back();
        }

        env->canvas->draw(name, (112/W) * env->canvas->w(), (90/H) * env->canvas->h(), color);
        env->canvas->draw(level, (300/W) * env->canvas->w(), (90/H) * env->canvas->h(), color);

        font->set_size(16);
        env->canvas->draw(name, (250/W) * env->canvas->w(), (129/H) * env->canvas->h(), color);
        font->set_size(18);
        env->canvas->draw("level", (257/W) * env->canvas->w(), (99/H) * env->canvas->h(), Color(82, 104, 93));
        env->canvas->draw(m, (295/W) * env->canvas->w(), (167/H) * env->canvas->h(), Color(208, 179, 43));
        env->canvas->draw(p, (295/W) * env->canvas->w(), (198/H) * env->canvas->h(), Color(166, 69, 151));
        env->canvas->draw(t, (295/W) * env->canvas->w(), (229/H) * env->canvas->h(), Color(78, 191, 190));

        font->set_size(22);
        env->canvas->draw(levelup, (500/W) * env->canvas->w(), (370/H) * env->canvas->h(), Color(62, 108, 236));

        font->set_size(12);
        env->canvas->draw(might, (607/W) * env->canvas->w(), (165/H) * env->canvas->h(), color);
        env->canvas->draw(mind, (607/W) * env->canvas->w(), (200/H) * env->canvas->h(), color);
        env->canvas->draw(perception, (607/W) * env->canvas->w(), (235/H) * env->canvas->h(), color);
        env->canvas->draw(agility, (607/W) * env->canvas->w(), (270/H) * env->canvas->h(), color);

        color = Color(82, 104, 93);
        env->canvas->draw(attack, (780/W) * env->canvas->w(), (165/H) * env->canvas->h(), color);
        env->canvas->draw(attack, (780/W) * env->canvas->w(), (200/H) * env->canvas->h(), color);
        env->canvas->draw(speed, (780/W) * env->canvas->w(), (235/H) * env->canvas->h(), color);
        env->canvas->draw(defence, (780/W) * env->canvas->w(), (270/H) * env->canvas->h(), color);

        env->canvas->draw(armor, (900/W) * env->canvas->w(), (165/H) * env->canvas->h(), color);
        env->canvas->draw(armor, (900/W) * env->canvas->w(), (200/H) * env->canvas->h(), color);
        env->canvas->draw(critical, (900/W) * env->canvas->w(), (235/H) * env->canvas->h(), color);
        env->canvas->draw(critical, (900/W) * env->canvas->w(), (270/H) * env->canvas->h(), color);

        font->set_size(16);
        settings = env->resources_manager->get_settings("res/datas/colony.sav");
        int data = settings->read<int>("Colony", "data", 0);
        env->canvas->draw(to_string(data), (500/W) * env->canvas->w(), (415/H) * env->canvas->h(), Color(170, 215, 190));
    }
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
        // TODO
    }
    else if (button->id() == "right_arrow")
    {

        // TODO
    }

    return true;
}
