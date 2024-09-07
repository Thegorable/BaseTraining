#include"Traynee.h"

using namespace std;

int main()
{
    List<int> l_1((size_t)4, 5);

    List<int> l_2(move(l_1));

    l_2.clear();

    return 0;
};