#ifndef BARRACKS_H
#define BARRACKS_H

#include "button.h"
#include <core/level.h>
#include <memory>
#include <map>

using std::map;

class Texture;

class Barracks : public Level
{
public:
    Barracks(const string& next = "base");

private:
    map<ObjectID, Button*> m_buttons;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();
};

#endif
