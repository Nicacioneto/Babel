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
    Settings(const string& next = "", const string& image = "res/images/settings.png");
    bool on_message(Object *sender, MessageID id, Parameters p);

private:
    shared_ptr<Image> m_image;
    Button *m_back;
    Button *m_up_resolution;
    Button *m_down_resolution;

    std::vector<int> m_resolutions = {800, 1024};

    void draw_self();
    void update_self(unsigned long elapsed);
};

#endif
