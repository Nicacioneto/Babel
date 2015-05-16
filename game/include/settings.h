#ifndef SETTINGS_H
#define SETTINGS_H

#include "button.h"
#include "file.h"
#include <core/level.h>
#include <memory>
#include <vector>

class Texture;

class Settings : public Level
{
public:
    Settings(const string& next = "menu",
        const string& texture = "res/images/menu/init-screen.png");
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Texture> m_texture;
    shared_ptr<Texture> m_logo;
    shared_ptr<Texture> m_soundvideo;
    shared_ptr<Texture> m_volume;
    shared_ptr<Texture> m_arrow;
    Button *m_up_volume, *m_down_volume;
    Button *m_up_resolution, *m_down_resolution;
    Button *m_back;
    File *m_file;
    int m_vol;

    std::vector<int> m_resolutions = {800, 1024};

    void draw_self();
    void update_coordinates();
};

#endif
