#ifndef SETTINGS_H
#define SETTINGS_H

#include "button.h"
#include "level.h"
#include <memory>
#include <vector>

class Image;

class Settings : public Level
{
public:
    Settings(const string& next = "menu", const string& image = "res/images/init_screen.png");
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Image> m_image;
    shared_ptr<Image> m_logo;
    shared_ptr<Image> m_soundvideo;
    shared_ptr<Image> m_volume;
    shared_ptr<Image> m_arrow;
    Button *m_up_volume, *m_down_volume;
    Button *m_up_resolution, *m_down_resolution;
    Button *m_back;

    std::vector<int> m_resolutions = {800, 1024};

    void draw_self();
    void update_self(unsigned long elapsed);
};

#endif
