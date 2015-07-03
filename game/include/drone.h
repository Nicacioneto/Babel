/*
 * Class that represents the Drone
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef DRONE_H
#define DRONE_H

#include <core/object.h>
#include <map>
#include <vector>

using std::map;
using std::shared_ptr;
using std::vector;

class Button;
class Character;
class Font;
class Settings;
class Texture;

class Drone : public Object
{
public:
    Drone(int slot, Object *parent = nullptr);

    void confirm();
    void load_drones();
    void change_buttons(bool state);
    
private:
    int m_slot;
    shared_ptr<Settings> m_settings;
    shared_ptr<Font> m_font;
    shared_ptr<Texture> m_background;

    map<ObjectID, Button*> m_buttons;
    string m_drone;

    void draw_self();
    bool on_message(Object *sender, MessageID id, Parameters p);
    void create_buttons();
    void change_buttons(string id);
};

#endif
