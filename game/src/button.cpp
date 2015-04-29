#include "button.h"

Button::Button(double x, double y, double w, double h)
    : Rect(x, y, w, h)
{
}

bool
Button::is_clicked(const int x, const int y)
{
	return  x >= m_x and x <= m_x + m_w and y >= m_y and y <= m_y + m_h;
}
