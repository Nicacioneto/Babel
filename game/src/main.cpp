#include <iostream>
#include "babel.h"

using namespace std;

int main() {
    try
    {
        Babel babel;
        babel.init("Babel Game", 800, 600);
        babel.draw_home();
        babel.run();
    } catch (Exception ex)
    {
        cerr << ex.message() << endl;
        return -1;
    }
    
    return 0;
}
