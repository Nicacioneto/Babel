/*
 * Research class implementation
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#include "hospital.h"
#include "research.h"
#include <core/color.h>
#include <core/font.h>
#include <core/rect.h>
#include <core/settings.h>
#include <core/texture.h>

using std::to_string;

#define W 1024.0
#define H 768.0

Research::Research(int slot, Object *parent)
	: Object(parent), m_slot(slot), m_settings(nullptr),
    m_page(1), m_max_pages(0), m_font(nullptr)
{
	parent->add_observer(this);

	Environment *env = Environment::get_instance();
	string path = "res/images/colony/";

	m_textures["matter_energy"] = env->resources_manager->get_texture(path +
	    "icons/matter_energy.png");
	m_textures["health"] = env->resources_manager->get_texture(path + "/icons/health.png");
	m_textures["big_list"] = env->resources_manager->get_texture(path + "big_list.png");

	m_settings = env->resources_manager->get_settings("res/datas/slot" +
	    to_string(m_slot) + "/items.sav");

	m_max_pages = (m_settings->sections().size() / BIG_LIST) +
	    (m_settings->sections().size() % BIG_LIST != 0);

	m_font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
	env->canvas->set_font(m_font);
}

void
Research::draw_self()
{
	Environment *env = Environment::get_instance();
	Color color(170, 215, 190);
	double scale_w = env->canvas->w() / W;
	double scale_h = env->canvas->h() / H;

	m_font->set_size(18);

	env->canvas->draw("Name", 360 * scale_w, 188 * scale_h, color);
	env->canvas->draw("Time", 855 * scale_w, 186 * scale_h, color);
	env->canvas->draw(m_textures["matter_energy"].get(), 690 * scale_w, 188 * scale_h);

	int y = 236;
	int i = -1;

	for (auto section : m_settings->sections())
	{
		i++;
		if (i < (m_page - 1) * BIG_LIST or i >= BIG_LIST * m_page)
		{
		    continue;
		}

		env->canvas->draw(m_textures["health"].get(), Rect(0, 25, 50, 50/2),
		    310 * scale_w, y * scale_h, 50 * scale_w, 25 * scale_h);
		env->canvas->draw(m_textures["big_list"].get(), Rect(0, 0, 602, 50/2),
		    310 * scale_w, (y+5) * scale_h, 602 * scale_w, 25 * scale_h);

		env->canvas->draw(section.first, 360 * scale_w, y * scale_h, color);
		env->canvas->draw(section.second["matter"] + "/" + section.second["energy"],
		    690 * scale_w, y * scale_h, color);
		env->canvas->draw(section.second["time"], 855 * scale_w, y * scale_h, color);

		y += 64;
	}
}

bool
Research::on_message(Object *sender, MessageID id, Parameters p)
{
    Hospital *hospital = dynamic_cast<Hospital *>(sender);

    if (not hospital)
    {
        return false;
    }
    set_visible(id == "research");
    
    if (visible())
    {
        notify("max_pages", to_string(m_max_pages));
        
        if (p.size())
        {
            m_page = atoi(p.c_str());
        }
    }

    return false;
}
