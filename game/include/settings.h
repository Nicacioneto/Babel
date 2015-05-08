#ifndef SETTINGS_H
#define SETTINGS_H

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
    shared_ptr<Image> m_resolution;
    int m_resolution_position = 0;

    std::vector<int> m_resolutions_size = {800, 1024};

    void draw_self();
};

#endif
