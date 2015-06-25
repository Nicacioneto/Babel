/*
 * Barracks class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "barracks.h"
#include "character.h"
#include "colony.h"
#include <core/font.h>
#include <core/keyboardevent.h>
#include <core/rect.h>
#include <core/settings.h>

#define W 1024.0
#define H 768.0

using std::to_string;

Barracks::Barracks(int slot, const string& next)
    : Level("barracks", next), m_slot(slot), m_character(0), m_screen(INSPECT)
{
    Environment *env = Environment::get_instance();
    string path = "res/images/colony/barracks/";

    env->events_manager->register_listener(this);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Button *button = new Button(this, "left_arrow", path + "left_arrow.png",
        60 * scale_w, 218 * scale_h, 26 * scale_w, 45 * scale_h);
    button->set_sprites(1);
    m_buttons[button->id()] = button;

    button = new Button(this, "right_arrow", path + "right_arrow.png",
        362 * scale_w, 218 * scale_h, 26 * scale_w, 45 * scale_h);
    button->set_sprites(1);
    m_buttons[button->id()] = button;

    button = new Button(this, "levelup_m", path + "levelup-m.png",
        409 * scale_w, 390 * scale_h, 60 * scale_w, 40 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "levelup_p", path + "levelup-p.png",
        485 * scale_w, 390 * scale_h, 60 * scale_w, 40 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "levelup_t", path + "levelup-t.png",
        560 * scale_w, 390 * scale_h, 60 * scale_w, 40 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "equip_shelf", path + "equip_shelf.png",
        690 * scale_w, 322 * scale_h, 225 * scale_w, 122 * scale_h);
    button->set_sprites(3);
    m_buttons[button->id()] = button;

    button = new Button(this, "back", path + "back.png",
        912 * scale_w, 55 * scale_h, 67 * scale_w, 26 * scale_h);
    button->set_sprites(1);
    m_buttons[button->id()] = button;

    button = new Button(this, "rifle", path + "rifle.png",
        112 * scale_w, 222 * scale_h, 55 * scale_w, 75 * scale_h);
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    button = new Button(this, "armor", path + "armor.png",
        194 * scale_w, 222 * scale_h, 55 * scale_w, 75 * scale_h);
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);
    m_buttons[button->id()] = button;

    button = new Button(this, "shield", path + "shield.png",
        278 * scale_w, 222 * scale_h, 55 * scale_w, 75 * scale_h);
    button->set_sprites(1);
    button->set_active(false);
    button->set_visible(false);
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
    m_textures["skill_m_locked"] = env->resources_manager->get_texture(path + "Skill_M_Locked.png");
    m_textures["skill_p_locked"] = env->resources_manager->get_texture(path + "Skill_P_Locked.png");
    m_textures["skill_t_locked"] = env->resources_manager->get_texture(path + "Skill_T_Locked.png");
    m_textures["bracket_equip"] = env->resources_manager->get_texture(path + "equip/Bracket.png");
    m_textures["rifle_katana"] = env->resources_manager->get_texture(path +
        "equip/Rifle_Katana.png");

    for (int i = 1; i <= 20; ++i)
    {
        string index = to_string(i);
        m_textures["skill_m_" + index] = env->resources_manager->get_texture(path + "Skill_M_" +
            index + ".png");
        m_textures["skill_p_" + index] = env->resources_manager->get_texture(path + "Skill_P_" +
            index + ".png");
        m_textures["skill_t_" + index] = env->resources_manager->get_texture(path + "Skill_T_" +
            index + ".png");
    }

    load_characters();
    Character *character = current_char();
    character->set_visible(true);
}

Barracks::~Barracks()
{
    Environment *env = Environment::get_instance();
    env->events_manager->unregister_listener(this);
}

void
Barracks::draw_self()
{
    Environment *env = Environment::get_instance();
    env->canvas->clear();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;
    
    env->canvas->draw(m_textures["bracket"].get(), 30 * scale_w, 25 * scale_h);
    env->canvas->draw(m_textures["attributes"].get(), 402 * scale_w, 87 * scale_h);
    env->canvas->draw(m_textures["stats"].get(), 690 * scale_w, 87 * scale_h);
    draw_attributes();

    switch (m_screen)
    {
        case INSPECT:
            inspect_screen();
            break;
        case EQUIP:
            equip_screen();
            break;
    }
}

void
Barracks::inspect_screen()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw("Inspect Hero", 52 * scale_w, 52 * scale_h, Color(84, 107, 95));

    int y = 470 * scale_h;
    env->canvas->draw(m_textures["bracket_m"].get(), 112 * scale_w, y);
    env->canvas->draw(m_textures["bracket_p"].get(), 402 * scale_w, y);
    env->canvas->draw(m_textures["bracket_t"].get(), 690 * scale_w, y);

    y = 485 * scale_h;
    env->canvas->draw(m_textures["military"].get(), 135 * scale_w, y);
    env->canvas->draw(m_textures["psionic"].get(), 420 * scale_w, y);
    env->canvas->draw(m_textures["tech"].get(), 710 * scale_w, y);

    env->canvas->draw(m_textures["levelup"].get(), 402 * scale_w, 322 * scale_h);
    env->canvas->draw(m_textures["equip"].get(), 690 * scale_w, 322 * scale_h);
    env->canvas->draw(m_textures["rifle"].get(), 702 * scale_w, 360 * scale_h);
    env->canvas->draw(m_textures["armor"].get(), 772 * scale_w, 360 * scale_h);
    env->canvas->draw(m_textures["shield"].get(), 847 * scale_w, 360 * scale_h);

    draw_character();
    draw_skills();
}

void
Barracks::draw_character()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    Color color(170, 215, 190);
    
    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Character *character = current_char();

    env->canvas->draw(m_textures["isaac_skills"].get(), 112 * scale_w, 376 * scale_h);

    font->set_size(16);
    int data = Colony(m_slot).data();
    env->canvas->draw(to_string(data), 500 * scale_w, 353 * scale_h, Color(170, 215, 190));

    int x = 112;
    int y = 87;
    font->set_size(18);
    m_buttons["levelup_m"]->set_text(to_string(character->levelup_m()), Color(168, 145, 35));
    m_buttons["levelup_p"]->set_text(to_string(character->levelup_p()), Color(166, 69, 151));
    m_buttons["levelup_t"]->set_text(to_string(character->levelup_t()), Color(79, 194, 193));
    env->canvas->draw(m_textures["card_big"].get(), x * scale_w, y * scale_h);
    env->canvas->draw(character->id(), (x + 131) * scale_w, (y + 5) * scale_h, color);

    font->set_style(Font::BOLD);
    env->canvas->draw(to_string(character->military()), (x+182) * scale_w,
        (y + 38) * scale_h, Color(208, 179, 43));
    env->canvas->draw(to_string(character->psionic()), (x+182) * scale_w,
        (y + 70) * scale_h, Color(166, 69, 151));
    env->canvas->draw(to_string(character->tech()), (x+182) * scale_w,
        (y + 100) * scale_h, Color(78, 191, 190));

    font->set_size(14);
    font->set_style(Font::NORMAL);

    // number of digits
    char buffer_length[100];
    sprintf(buffer_length, "%d\n", character->shield());
    string sf = buffer_length;

    env->canvas->draw(to_string(character->shield()),
        (x + 159 + 7 * (3 - sf.size())) * scale_w, (y + 162) * scale_h, Color::WHITE);

    sprintf(buffer_length, "%d\n", character->life());
    sf = buffer_length;
    env->canvas->draw(to_string(character->life()),
        (x + 159 + 7 * (3 - sf.size())) * scale_w, (y + 191) * scale_h, Color::WHITE);

    sprintf(buffer_length, "%d\n", character->mp());
    sf = buffer_length;
    env->canvas->draw(to_string(character->mp()),
        (x + 159 + 7 * (3 - sf.size())) * scale_w, (y + 220) * scale_h, Color::WHITE);

    env->canvas->draw("/", (x + 175) * scale_w, (y-5 + 166) * scale_h, Color::WHITE);
    env->canvas->draw("/", (x + 175) * scale_w, (y-5 + 195) * scale_h, Color::WHITE);
    env->canvas->draw("/", (x + 175) * scale_w, (y-5 + 225) * scale_h, Color::WHITE);

    font->set_size(10);
    env->canvas->draw(to_string(character->max_shield()), (x + 183) * scale_w,
        (y + 166) * scale_h, Color::WHITE);
    env->canvas->draw(to_string(character->max_life()), (x + 183) * scale_w,
        (y + 195) * scale_h, Color::WHITE);
    env->canvas->draw(to_string(character->max_mp()), (x + 183) * scale_w,
        (y + 225) * scale_h, Color::WHITE);
}

void
Barracks::draw_attributes()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    Color color(170, 215, 190);
    
    font->set_size(14);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    Character *character = current_char();

    int x = 606 * scale_w;
    env->canvas->draw(to_string(character->might()), x, 120 * scale_h, color);
    env->canvas->draw(to_string(character->mind()), x, 150 * scale_h, color);
    env->canvas->draw(to_string(character->agility()), x, 180 * scale_h, color);
    env->canvas->draw(to_string(character->willpower()), x, 210 * scale_h, color);
    env->canvas->draw(to_string(character->resilience()), x, 240 * scale_h, color);
    env->canvas->draw(to_string(character->perception()), x, 270 * scale_h, color);

    font->set_size(12);
    x = 780 * scale_w;
    env->canvas->draw(to_string(character->might_attack()), x, 125 * scale_h, color);
    env->canvas->draw(to_string(character->mind_attack()), x, 172 * scale_h, color);
    env->canvas->draw(to_string(character->cooldown()), x, 219 * scale_h, color);
    env->canvas->draw(to_string(character->defense()), x, 266 * scale_h, color);

    x = 900 * scale_w;
    env->canvas->draw(to_string(character->might_armor()), x, 125 * scale_h, color);
    env->canvas->draw(to_string(character->mind_armor()), x, 172 * scale_h, color);
    env->canvas->draw(to_string(character->critical()), x, 219 * scale_h, color);
    env->canvas->draw(to_string(character->critical()), x, 266 * scale_h, color);
}

void
Barracks::draw_skills()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int x = 130 * scale_w;
    int y = 530 * scale_h;
    int w = 38 * scale_w;
    int h = 37 * scale_h;

    Character *character = current_char();
    
    for (int i = 1; i <= 4; ++i, y += h)
    {
        x = 130 * scale_w;
        for (int j = 1; j <= 5; ++j, x += w)
        {
            if ( (i - 1)*5 + j <= character->military())
            {
                env->canvas->draw(m_textures["skill_m_" + to_string((i - 1)*5 + j)].get(), x, y);
            }
            else
            {
                env->canvas->draw(m_textures["skill_m_locked"].get(), x, y);
            }
        }
    }

    x = 420 * scale_w;
    y = 530 * scale_h;

    for (int i = 1; i <= 4; ++i, y += h)
    {
        x = 420 * scale_w;
        for (int j = 1; j <= 5; ++j, x += w)
        {
            if ( (i - 1)*5 + j <= character->psionic())
            {
                env->canvas->draw(m_textures["skill_p_" + to_string((i - 1)*5 + j)].get(), x, y);
            }
            else
            {
                env->canvas->draw(m_textures["skill_p_locked"].get(), x, y);
            }
        }
    }

    x = 710 * scale_w;
    y = 530 * scale_h;

    for (int i = 1; i <= 4; ++i, y += h)
    {
        x = 710 * scale_w;
        for (int j = 1; j <= 5; ++j, x += w)
        {
            if ( (i - 1)*5 + j <= character->tech())
            {
                env->canvas->draw(m_textures["skill_t_" + to_string((i - 1)*5 + j)].get(), x, y);
            }
            else
            {
                env->canvas->draw(m_textures["skill_t_locked"].get(), x, y);
            }
        }
    }
}

void
Barracks::equip_screen()
{
    Environment *env = Environment::get_instance();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(16);

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    env->canvas->draw(m_textures["bracket_equip"].get(), 112 * scale_w, 322 * scale_h);
    env->canvas->draw("Equip Hero", 52 * scale_w, 52 * scale_h, Color(84, 107, 95));
    env->canvas->draw(m_textures["rifle_katana"].get(), 145 * scale_w, 512 * scale_h);
}

bool
Barracks::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    Character *character = current_char();
    character->set_visible(false);

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
        int data = Colony(m_slot).data() - c->levelup_m();

        if (c->military() < 20 and data > 0)
        {
            Colony(m_slot).set_data(data);

            int lvl = c->military();
            c->set_military(lvl + 1);

            update_char_attributes(c, "M");
        }
    }
    else if (button->id() == "levelup_p")
    {
        Character *c = current_char();
        int data = Colony(m_slot).data() - c->levelup_p();

        if (c->psionic() < 20 and data > 0)
        {
            Colony(m_slot).set_data(data);

            int lvl = c->psionic();
            c->set_psionic(lvl + 1);

            update_char_attributes(c, "P");
        }
    }
    else if (button->id() == "levelup_t")
    {
        Character *c = current_char();
        int data = Colony(m_slot).data() - c->levelup_t();

        if (c->tech() < 20 and data > 0)
        {
            Colony(m_slot).set_data(data);

            int lvl = c->tech();
            c->set_tech(lvl + 1);

            update_char_attributes(c, "T");
        }
    }
    else if (button->id() == "equip_shelf")
    {
        m_screen = EQUIP;

        Environment *env = Environment::get_instance();
        double scale_w = env->canvas->w() / W;
        double scale_h = env->canvas->h() / H;

        m_buttons["left_arrow"]->set_position(57 * scale_w, 128 * scale_h);
        m_buttons["right_arrow"]->set_position(360 * scale_w, 128 * scale_h);

        m_buttons["rifle"]->set_active(true);
        m_buttons["rifle"]->set_visible(true);
        m_buttons["armor"]->set_active(true);
        m_buttons["armor"]->set_visible(true);
        m_buttons["shield"]->set_active(true);
        m_buttons["shield"]->set_visible(true);

        m_buttons["levelup_m"]->set_visible(false);
        m_buttons["levelup_m"]->set_active(false);
        m_buttons["levelup_p"]->set_visible(false);
        m_buttons["levelup_p"]->set_active(false);
        m_buttons["levelup_t"]->set_visible(false);
        m_buttons["levelup_t"]->set_active(false);
        m_buttons["equip_shelf"]->set_visible(false);
        m_buttons["equip_shelf"]->set_active(false);

        for (auto character : m_characters)
        {
            character.second->set_texture("booker.png");
            character.second->set_h(123 * scale_h);
        }
    }
    else if (button->id() == "back")
    {
        if (m_screen == EQUIP)
        {
            m_screen = INSPECT;

            Environment *env = Environment::get_instance();
            double scale_w = env->canvas->w() / W;
            double scale_h = env->canvas->h() / H;

            m_buttons["left_arrow"]->set_position(60 * scale_w, 218 * scale_h);
            m_buttons["right_arrow"]->set_position(362 * scale_w, 218 * scale_h);

            m_buttons["levelup_m"]->set_visible(true);
            m_buttons["levelup_m"]->set_active(true);
            m_buttons["levelup_p"]->set_visible(true);
            m_buttons["levelup_p"]->set_active(true);
            m_buttons["levelup_t"]->set_visible(true);
            m_buttons["levelup_t"]->set_active(true);
            m_buttons["equip_shelf"]->set_visible(true);
            m_buttons["equip_shelf"]->set_active(true);

            for (auto character : m_characters)
            {
                character.second->set_texture("test_big.png");
                character.second->set_h(270 * scale_h);
            }
        }
        else
        {
            Environment *env = Environment::get_instance();
            shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
                to_string(m_slot) + "/colony.sav");
            string prev = settings->read<string>("Barracks", "prev", "base");
            set_next(prev);
            finish();
        }
    }

    character = current_char();
    character->set_visible(true);
    
    return true;
}

void
Barracks::load_characters()
{
    Environment *env = Environment::get_instance();

    double scale_w = env->canvas->w() / W;
    double scale_h = env->canvas->h() / H;

    int x = 112 * scale_w;
    int y = 87 * scale_h;
    int w = 222 * scale_w;
    int h = 270 * scale_h;

    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/characters.sav");
    auto sections = settings->sections();

    for (auto section : sections)
    {
        if (section.first != "Default")
        {
            Character *character = new Character(m_slot, this, section.first, "test_big.png",
                x, y, w, h, section.first);
            character->set_active(false);
            character->set_visible(false);
            character->add_observer(this);
            add_child(character);
            m_characters[character->id()] = character;
        }
    }
}

Character *
Barracks::current_char() const
{
    auto it = m_characters.begin();
    for (int i = 0; i < m_character; ++it, ++i) {}; // not work very well with other ++ operators
    return it->second;
}

void
Barracks::update_char_attributes(Character *c, string class_)
{
    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/datas/slot" +
        to_string(m_slot) + "/levelup.sav");

    c->set_life(c->life() + settings->read<int>(class_, "life", 0));
    c->set_max_life(c->max_life() + settings->read<int>(class_, "life", 0));
    c->set_mp(c->mp() + settings->read<int>(class_, "mp", 0));
    c->set_max_mp(c->max_mp() + settings->read<int>(class_, "mp", 0));
    c->set_might(c->might() + settings->read<int>(class_, "might", 0));
    c->set_mind(c->mind() + settings->read<int>(class_, "mind", 0));
    c->set_resilience(c->resilience() + settings->read<int>(class_, "resilience", 0));
    c->set_willpower(c->willpower() + settings->read<int>(class_, "willpower", 0));
    c->set_agility(c->agility() + settings->read<int>(class_, "agility", 0));
    c->set_perception(c->perception() + settings->read<int>(class_, "perception", 0));
}

bool
Barracks::on_event(const KeyboardEvent& event)
{
    if (event.state() == KeyboardEvent::PRESSED
        and event.key() == KeyboardEvent::ESCAPE)
    {
        set_next("base");
        finish();

        Environment *env = Environment::get_instance();
        env->sfx->play("res/sfx/uiConfirm1.ogg", 1);

        return true;
    }

    return false;
}
