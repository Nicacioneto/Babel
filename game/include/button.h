#ifndef BUTTON_H
#define BUTTON_H

#include <core/environment.h>
#include <core/listener.h>
#include <core/object.h>
#include <core/text.h>
#include <core/texture.h>
#include <memory>

using std::shared_ptr;
using std::string;

typedef enum { IDLE, ON_HOVER, HIDE } State;

class MouseButtonEvent;
class MouseMotionEvent;

class Button : public Object, public Listener
{
public:
    Button(Object *parent = nullptr, ObjectID id = "", const string& image = "",
        double x = 0, double y = 0, double w = 100, double h = 100);

    ~Button();

    bool on_event(const MouseButtonEvent& event);
    bool on_event(const MouseMotionEvent& event);
    void set_text(const string& str, const Color& color = Color(170, 215, 190));
    Text* text();

    static ActionID clickedID;

private:
    Text *m_text;
    shared_ptr<Texture> m_texture;
    State m_state;
    Environment *env;

    void draw_self(double x0 = 0, double y0 = 0);
};

#endif
