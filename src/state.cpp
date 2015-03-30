#include "include/state.h"

State::State(){

    requested_delete = false;
    requested_quit = false;
}

State::~State(){}

//void State::Add_object(GameObject *object){

//    object_array.emplace_back(object);

//}

bool State::Requested_delete(){

    return requested_delete;

}

bool State::Requested_quit(){

    return requested_quit;

}

