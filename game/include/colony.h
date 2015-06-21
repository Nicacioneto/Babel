/*
 * Class that represents the Colony
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef COLONY_H
#define COLONY_H

#include "button.h"
#include <core/environment.h>
#include <core/listener.h>
#include <core/object.h>
#include <core/text.h>
#include <core/texture.h>
#include <memory>

class Colony : public Object
{
public:
    Colony(int slot, Object *parent = nullptr, ObjectID id = "");

    int data() const;
    int matter() const;
    int energy() const;
    int mwaked() const;
    int pwaked() const;
    int twaked() const;

    void set_data(int data);
    void set_matter(int matter);
    void set_energy(int energy);
    void set_mwaked(int mwaked);
    void set_pwaked(int pwaked);
    void set_twaked(int twaked);

private:
    int m_slot, m_data, m_matter, m_energy;
    int m_mwaked, m_pwaked, m_twaked;

    map<ObjectID, shared_ptr<Texture>> m_textures;
    map<ObjectID, Button *> m_buttons;
    shared_ptr<Settings> m_settings;

    bool on_message(Object *sender, MessageID id, Parameters p);
    void draw_self();

    template<typename T>
    void write(const string& attr, const T& value);
};

#endif
