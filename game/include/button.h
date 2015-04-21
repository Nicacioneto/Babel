#ifndef BUTTON_H
#define BUTTON_H

#include "rect.h"

class Button : public Rect {
public:
	Button(double x, double y, double w, double h);

	bool is_clicked(const int x, const int y);
};

#endif
