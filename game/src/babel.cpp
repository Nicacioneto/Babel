#include "babel.h"
#include "button.h"
#include "circle.h"
#include "image.h"
#include "point.h"
#include "rect.h"
#include "state_menu.h"
#include <iostream>

using namespace std;

Babel::Babel() 
    : Game("a")
{
    env = Environment::get_instance();
}
