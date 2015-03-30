#ifndef STATE
#define STATE

#include <vector>
#include <memory>

class State{

public:

    State();
    virtual ~State();

    virtual void Update( float delta_time ) = 0;
    virtual void Rendering() = 0;
//    virtual void Add_object (GameObject *object);

    bool Requested_quit();
    bool Requested_delete();

protected:
    bool requested_delete;
    bool requested_quit;
//    virtual void Update_array(float delta_time);
//    virtual void Render_array();

//    std::vector<std::unique_ptr<GameObject>> object_array;
};

#endif // STATE

