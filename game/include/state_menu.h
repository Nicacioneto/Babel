#ifndef STATE_MENU_H
#define STATE_MENU_H

#include "state.h"
#include "environment.h"
#include "exception.h"
#include "image.h"

class StateMenu : public State
{
public:
	StateMenu();
	virtual void load() throw (Exception) override;
	virtual void unload() override;
	virtual void update() throw (Exception) override;
	virtual void render() throw (Exception) override;
	
private:
	Environment *env;
};

#endif
