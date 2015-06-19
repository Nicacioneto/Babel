#ifndef BARRACKS_H
#define BARRACKS_H

#include "button.h"
#include <core/level.h>
#include <core/listener.h>
#include <memory>
#include <map>

using std::map;

class Texture;
class Character;
class KeyboardEvent;

class Barracks : public Level, public Listener
{
public:
    Barracks(const string& next = "base");
    ~Barracks();

private:
    map<ObjectID, Button*> m_buttons;
    map<ObjectID, Character*> m_characters;
    map<string, shared_ptr<Texture>> m_textures;
    shared_ptr<Settings> m_settings;
    int m_character;

    void draw_self();
    void draw_character();
    void draw_skills();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void load_characters();
    Character * current_char() const;
    void update_char_attributes(Character *c, string class_);
    bool on_event(const KeyboardEvent& event);
};

#endif
