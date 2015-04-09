#include <iostream>
#include "voidcrawlers.h"

using namespace std;

int main() {
    try
    {
        VoidCrawlers voidcrawlers;
        voidcrawlers.init("VoidCrawlers Game", 800, 600);
        voidcrawlers.run();
    } catch (Exception ex)
    {
        cerr << ex.message() << endl;
        return -1;
    }
    
    return 0;
}
