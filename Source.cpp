#include"Traynee.h"
#include<fstream>
#include<sstream>
#include<memory>
#include <iostream>

using namespace std;

int main()
{
    List<int> list_test;
    list<int> list_std;
    auto it = list_std.begin();

    int iterations = 10000;

    int push_back = 0;
    int push_front = 0;
    int insert = 0;
    int erase = 0;
    
    for (int i = 0; i < iterations; i++) {
        
        int method_num = rand() % 4;
        int rand_num = rand() % (iterations - 1);
        int rand_place = 0;
        
        if (list_test.size() > 1) {
            rand_place = rand() % (list_test.size() - 1);
        }

        it = list_std.begin();

        switch (method_num) {
        case 0:
            list_test.push_back(rand_num);
            list_std.push_back(rand_num);
            push_back++;
            break;

        case 1:
            list_test.push_front(rand_num);
            list_std.push_front(rand_num);
            push_front++;
            break;

        case 2:
            list_test.insert(rand_num, rand_place);
            advance(it, rand_place);
            list_std.insert(it, rand_num);
            insert++;
            break;

        case 3:
            list_test.erase(rand_place);
            advance(it, rand_place);
            list_std.erase(it);
            erase++;
            break;
        }
    }
    //for (int i = 0; i < list_test.size(); i++) {
    //    cout << list_test[i];
    //}
    cout << "push_back: "s << '\t' << push_back << '\n';
    cout << "push_front: "s << '\t' << push_front << '\n';
    cout << "insert: "s << '\t' << insert << '\n';
    cout << "erase: "s << '\t' << erase << '\n';
    
    //cout << '\n';
    //for (int i : list_std) {
    //    cout << i;
    //}
    
    //cout << '\n';
    /*TextEditor editor;

    while (true) {
        editor.edit();
    }*/

    system("pause");
};