#include"List.h"
#include"Traynee.h"

using namespace std;

int main()
{
    List l((size_t)6, 6);
    vector<const int*> v;
    for (int i = 0; i < 6; i++) {
        v.push_back(&l.at(i));
    }

    for (int i = 0; i < 6; i++) {
        l.pop_back();
    }

    l.push_back(5);
    
    l.clear();

    return 0;
};