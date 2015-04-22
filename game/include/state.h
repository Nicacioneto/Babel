#ifndef STATE_H
#define STATE_H

class State
{
public:
    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};

#endif
