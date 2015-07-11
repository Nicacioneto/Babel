/*
 * TextBox implementation
 *
 * Author: Tiamat
 * Date: 10/07/2015
 * License: LGPL. No copyright.
 */
#include "textbox.h"
#include <core/point.h>
#include <core/rect.h>
#include <core/texture.h>
#include <core/environment.h>

#include <vector>
#include <list>
#include <sstream>

using namespace std;

class TextBox::Impl
{
public:
    Impl(TextBox *text_box, const string& text, const Color& text_color, Alignment alignment)
        : m_text_box(text_box), m_text(""), m_alignment(alignment), m_border(Color::BLUE),
            m_background(Color::TRANSPARENT), m_text_color(text_color)
    {
        set_text(text);
    }

    ~Impl()
    {
        if (not m_textures.empty())
        {
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                delete m_textures[i].first;
            }

            m_textures.clear();
        }
    }

    void set_text(const string& text)
    {
        // Erase old textures, if it exists
        if (not m_textures.empty())
        {
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                delete m_textures[i].first;
            }

            m_textures.clear();
        }

        // Reads the set of the words of the message
        istringstream is(text);
        string word;
        list<string> words;

        while (is >> word)
        {
            words.push_back(word);
        }

        // Renders the words, line by line
        Environment *env = Environment::get_instance();
        double x = m_text_box->x() + 5;
        double y = m_text_box->y() + 5;
        double total, spaces, separator;

        while (not words.empty())
        {
            ostringstream os;
            list<string> wl;
            list<Texture *> tl;
            os << words.front();

            Texture *line = nullptr;
            Texture *texture = env->canvas->render_text(os.str(), m_text_color);

            while (texture->w() < m_text_box->w() - 10)
            {
                wl.push_back(words.front());
                words.pop_front();

                delete line;
                line = texture;

                if (words.empty())
                {
                    break;
                }

                os << " " << words.front();
                texture = env->canvas->render_text(os.str(), m_text_color);
            }

            if (texture != line)
            {
                delete texture;
            }

            if (line == nullptr)
            {
                break;
            }

            switch (m_alignment)
            {
            case Alignment::RIGHT:
                x = m_text_box->x() + (m_text_box->w() - 5 - line->w());
                m_textures.push_back(make_pair(line, Point(x, y)));
                break;

            case Alignment::LEFT:
                x = m_text_box->x() + 5;
                m_textures.push_back(make_pair(line, Point(x, y)));
                break;

            case Alignment::CENTER:
                x = m_text_box->x() + (m_text_box->w() - 5 - line->w())/2;
                m_textures.push_back(make_pair(line, Point(x, y)));
                break;

            case Alignment::JUSTIFY:
                total = 0;

                for (auto w : wl)
                {
                    texture = env->canvas->render_text(w, m_text_color);
                    total += texture->w();
                    tl.push_back(texture);
                }

                spaces = m_text_box->w() - 10 - total;
                separator = wl.size() > 1 ? spaces/(wl.size() - 1) : 0;

                x = m_text_box->x() + 5;

                for (auto t : tl)
                {
                    m_textures.push_back(make_pair(t, Point(x, y)));
                    x += t->w() + separator;
                }
            }

            y += line->h() + 5;
        }
    }

    void set_colors(const Color& border, const Color& background)
    {
        m_border = border;
        m_background = background;
    }

    void set_text_color(const Color& text_color)
    {
        m_text_color = text_color;
    }

    void update_self(unsigned long)
    {
    }

    void draw_self()
    {
        Environment *env = Environment::get_instance();
        env->canvas->set_blend_mode(Canvas::BLEND);
        env->canvas->fill(m_text_box->bounding_box(), m_background);
        env->canvas->draw(m_text_box->bounding_box(), m_border);
        env->canvas->set_blend_mode(Canvas::NONE);

        for (auto it : m_textures)
        {
            Texture *texture = it.first;
            Point p = it.second;
            env->canvas->draw(texture, p.x(), p.y());
        }
    }

private:
    TextBox *m_text_box;
    string m_text;
    Alignment m_alignment;
    Color m_border, m_background, m_text_color;

    vector< pair<Texture *, Point> > m_textures;
};

TextBox::TextBox(Object *parent, const Rect& area, const string& text,
    const Color& text_color, Alignment alignment)
    : Object(parent, "textbox", area.x(), area.y(), area.w(), area.h()),
        m_impl(new Impl(this, text, text_color, alignment))
{
}

void
TextBox::set_text(const string& text)
{
    m_impl->set_text(text);
}

void
TextBox::set_colors(const Color& border, const Color& background)
{
    m_impl->set_colors(border, background);
}

void
TextBox::set_text_color(const Color& text_color)
{
    m_impl->set_text_color(text_color);
}

void
TextBox::update_self(unsigned long elapsed)
{
    m_impl->update_self(elapsed);
}

void
TextBox::draw_self()
{
    m_impl->draw_self();
}
