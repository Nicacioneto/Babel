#include "barracks.h"
#include "character.h"
#include "colony.h"
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>

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

    button = new Button(this, "levelup_m", path + "levelup-m.png", (409/W) * env->canvas->w(),
        (390/H) * env->canvas->h(), 60, 40);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "levelup_p", path + "levelup-p.png", (485/W) * env->canvas->w(),
        (390/H) * env->canvas->h(), 60, 40);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "levelup_t", path + "levelup-t.png", (560/W) * env->canvas->w(),
        (390/H) * env->canvas->h(), 60, 40);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "equip_shelf", path + "equip_shelf.png", (690/W) * env->canvas->w(),
        (322/H) * env->canvas->h(), 225, 122);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "back", path + "back.png", (912/W) * env->canvas->w(),
        (55/H) * env->canvas->h(), 67, 26);
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
    m_textures["rifle"] = env->resources_manager->get_texture(path + "rifle.png");
    m_textures["armor"] = env->resources_manager->get_texture(path + "armor.png");
    m_textures["shield"] = env->resources_manager->get_texture(path + "shield.png");
    m_textures["isaac_skills"] = env->resources_manager->get_texture(path + "isaac_skills.png");

    load_characters();
}

void
Barracks::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    env->canvas->draw("Inspect Hero", (52 / W) * env->canvas->w(), (52 / H) * env->canvas->h(), Color(84, 107, 95));

    env->canvas->draw(m_textures["bracket"].get(), (30 / W) * env->canvas->w(), (25 / H) * env->canvas->h());

    int y = (470 / H) * env->canvas->h();
    env->canvas->draw(m_textures["bracket_m"].get(), (112 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["bracket_p"].get(), (402 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["bracket_t"].get(), (690 / W) * env->canvas->w(), y);

    y = (485 / H) * env->canvas->h();
    env->canvas->draw(m_textures["military"].get(), (135 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["psionic"].get(), (420 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["tech"].get(), (710 / W) * env->canvas->w(), y);

    y = (87 / H) * env->canvas->h();
    env->canvas->draw(m_textures["attributes"].get(), (402 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["stats"].get(), (690 / W) * env->canvas->w(), y);
    env->canvas->draw(m_textures["levelup"].get(), (402 / W) * env->canvas->w(), (322 / H) * env->canvas->h());

    env->canvas->draw(m_textures["equip"].get(), (690 / W) * env->canvas->w(), (322 / H) * env->canvas->h());
    env->canvas->draw(m_textures["rifle"].get(), (702 / W) * env->canvas->w(), (360 / H) * env->canvas->h());
    env->canvas->draw(m_textures["armor"].get(), (772 / W) * env->canvas->w(), (360 / H) * env->canvas->h());
    env->canvas->draw(m_textures["shield"].get(), (847 / W) * env->canvas->w(), (360 / H) * env->canvas->h());

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
    Character *character = current_char();
    character->set_visible(true);

    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    Color color(170, 215, 190);
    
    font->set_size(14);
    int x = (606 / W) * env->canvas->w();
    env->canvas->draw(to_string(character->might()), x, (120 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->mind()), x, (150 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->agility()), x, (180 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->willpower()), x, (210 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->resilience()), x, (240 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->perception()), x, (270 / H) * env->canvas->h(), color);

    font->set_size(12);
    x = (780 / W) * env->canvas->w();
    env->canvas->draw(to_string(character->might_attack()), x, (125 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->mind_attack()), x, (172 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->cooldown()), x, (219 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->defense()), x, (266 / H) * env->canvas->h(), color);

    x = (900 / W) * env->canvas->w();
    env->canvas->draw(to_string(character->might_armor()), x, (125 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->mind_armor()), x, (172 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->critical()), x, (219 / H) * env->canvas->h(), color);
    env->canvas->draw(to_string(character->critical()), x, (266 / H) * env->canvas->h(), color);

    env->canvas->draw(m_textures["isaac_skills"].get(), (112 / W) * env->canvas->w(), (376 / H) * env->canvas->h());

    font->set_size(16);
    int data = Colony().data();
    env->canvas->draw(to_string(data), (500/W) * env->canvas->w(),
        (353/H) * env->canvas->h(), Color(170, 215, 190));

    x = 112;
    int y = 87;
    font->set_size(18);
    m_buttons["levelup_m"]->set_text(to_string(character->levelup_m()), Color(168, 145, 35));
    m_buttons["levelup_p"]->set_text(to_string(character->levelup_p()), Color(166, 69, 151));
    m_buttons["levelup_t"]->set_text(to_string(character->levelup_t()), Color(79, 194, 193));
    env->canvas->draw(m_textures["card_big"].get(), (x / W) * env->canvas->w(), (y / H) * env->canvas->h());
    env->canvas->draw(character->id(), (x + 131)/W * env->canvas->w(), (y + 5)/H * env->canvas->h(), color);

    font->set_style(Font::BOLD);
    env->canvas->draw(to_string(character->military()), (x+182)/W * env->canvas->w(),
        (y + 38)/H * env->canvas->h(), Color(208, 179, 43));
    env->canvas->draw(to_string(character->psionic()), (x+182)/W * env->canvas->w(),
        (y + 70)/H * env->canvas->h(), Color(166, 69, 151));
    env->canvas->draw(to_string(character->tech()), (x+182)/W * env->canvas->w(),
        (y + 100)/H * env->canvas->h(), Color(78, 191, 190));

    font->set_size(14);
    font->set_style(Font::NORMAL);

    // number of digits
    char buffer_length[100];
    sprintf(buffer_length, "%d\n", character->shield());
    string sf = buffer_length;

    env->canvas->draw(to_string(character->shield()),
        (x + 159 + 7 * (3 - sf.size()))/W * env->canvas->w(),
        (y + 162)/H * env->canvas->h(), Color::WHITE);

    sprintf(buffer_length, "%d\n", character->life());
    sf = buffer_length;
    env->canvas->draw(to_string(character->life()),
        (x + 159 + 7 * (3 - sf.size()))/W * env->canvas->w(),
        (y + 191)/H * env->canvas->h(), Color::WHITE);

    sprintf(buffer_length, "%d\n", character->mp());
    sf = buffer_length;
    env->canvas->draw(to_string(character->mp()),
        (x + 159 + 7 * (3 - sf.size()))/W * env->canvas->w(),
        (y + 220)/H * env->canvas->h(), Color::WHITE);

    env->canvas->draw("/", (x + 175)/W * env->canvas->w(), (y-5 + 166)/H * env->canvas->h(), Color::WHITE);
    env->canvas->draw("/", (x + 175)/W * env->canvas->w(), (y-5 + 195)/H * env->canvas->h(), Color::WHITE);
    env->canvas->draw("/", (x + 175)/W * env->canvas->w(), (y-5 + 225)/H * env->canvas->h(), Color::WHITE);

    font->set_size(10);
    env->canvas->draw(to_string(character->max_shield()), (x + 183)/W * env->canvas->w(),
        (y + 166)/H * env->canvas->h(), Color::WHITE);
    env->canvas->draw(to_string(character->max_life()), (x + 183)/W * env->canvas->w(),
        (y + 195)/H * env->canvas->h(), Color::WHITE);
    env->canvas->draw(to_string(character->max_mp()), (x + 183)/W * env->canvas->w(),
        (y + 225)/H * env->canvas->h(), Color::WHITE);
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
    else if (button->id() == "levelup_m")
    {
        Character *c = current_char();
        int data = Colony().data();
        Colony().set_data(data - c->levelup_m());

        int lvl = c->military();
        c->set_military(lvl + 1);
    }
    else if (button->id() == "levelup_p")
    {
        Character *c = current_char();
        int data = Colony().data();
        Colony().set_data(data - c->levelup_p());

        int lvl = c->psionic();
        c->set_psionic(lvl + 1);
    }
    else if (button->id() == "levelup_t")
    {
        Character *c = current_char();
        int data = Colony().data();
        Colony().set_data(data - c->levelup_t());

        int lvl = c->tech();
        c->set_tech(lvl + 1);
    }
    else if (button->id() == "back")
    {
        set_next("base");
        finish();
    }

    return true;
}

void
Barracks::load_characters()
{
    Environment *env = Environment::get_instance();
    
    int x = (112 / W) * env->canvas->w();
    int y = (87 / H) * env->canvas->h();
    int w = (222 / W) * env->canvas->w();
    int h = (270 / H) * env->canvas->h();

    auto settings = env->resources_manager->get_settings("res/datas/characters.sav");
    auto sections = settings->sections();

    for (auto section : sections)
    {
        if (section.first != "Default")
        {
            Character *character = new Character(this, section.first, "test_big.png",
                x, y, w, h, section.first);
            m_characters[character->id()] = character;
        }
    }

    for (auto it : m_characters)
    {
        it.second->add_observer(this);
        add_child(it.second);

        it.second->set_active(false);
        it.second->set_visible(false);
    }
}

Character *
Barracks::current_char() const
{
    auto it = m_characters.begin();
    for (int i = 0; i < m_character; ++it, ++i) {}; // not work very well with other ++ operators
    return it->second;
}
