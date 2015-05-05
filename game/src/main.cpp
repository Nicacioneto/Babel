#include <iostream>
#include "babel.h"

using namespace std;

int main(int, char**)
{
    try
    {
        Babel babel;
        babel.init("Babel Game", 1024, 768);
        babel.run();
    } catch (Exception ex)
    {
        cerr << ex.message() << endl;
        return -1;
    }
    
    return 0;
}
