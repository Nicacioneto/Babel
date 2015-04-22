#include "state_menu.h"
#include "canvas.h"
#include "rect.h"

StateMenu::StateMenu()
{
    env = Environment::get_instance();
}

void
StateMenu::load() throw (Exception)
{
    Rect rect;
    rect.set(0, 0);
    rect.set_dimensions(env->video->resolution().first, env->video->resolution().second);
    // env->canvas->load_image("res/images/menu.png", rect);
}

void
StateMenu::unload()
{
    // SDL_DestroyTexture(env->canvas->image()->texture());
}
void
StateMenu::update() throw (Exception)
{

}
void
StateMenu::render() throw (Exception)
{
    env->canvas->update();
}
