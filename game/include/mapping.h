#ifndef MAPPING_H
#define MAPPING_H

#include <core/rect.h>

class Texture;

class Mapping
{
public:
    Mapping(double ratio = 0.6);

    void draw(Texture *texture, const Rect clip);
    void draw(Texture *texture, const Rect front, const Rect back);

private:
    double m_ratio;
};

#endif
