#include <iostream>
#include "babel.h"

using namespace std;

int main(int, char**)
{
    try
    {
        Babel babel;
        babel.init("res/settings.ini");
        babel.run();
    } catch (Exception ex)
    {
        cerr << ex.message() << endl;
        return -1;
    }
    
    return 0;
}
