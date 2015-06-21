/*
 * Class that represents the Mapping
 *
 * Author: Tiamat
 * Date: 21/06/2015
 * License: LGPL. No copyright.
 */
#ifndef MAPPING_H
#define MAPPING_H

#include <core/rect.h>

class Bitmap;

class Mapping
{
public:
    Mapping(double vertical_ratio = 0.6, double horizontal_ratio = 1.7);

    void draw_center(Bitmap *screen, Bitmap *bitmap, const Rect clip);
    void draw_walls(Bitmap *screen, Bitmap *bitmap, const Rect front, const Rect back);
    void draw_ceiling_floor(Bitmap *screen, Bitmap *bitmap, const Rect front, const Rect back);

private:
    double m_vertical_ratio;
    double m_horizontal_ratio;
};

#endif
