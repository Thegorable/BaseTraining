#include"Traynee.h"
#include"ctime"
#include<stdio.h>

timer::timer(short sec)
{
	secDur = sec;
	setTimer(secDur);
	prevNow = steady_clock::now();
	isExpired = false;
}

void timer::setTimer(short sec)
{
	dur = duration_cast<nanoseconds>(duration<long long, ratio<1, 1>>(sec));
}

void timer::updateTimer()
{
	dur = dur - (steady_clock::now() - prevNow);
	long long durCount = dur.count();
	prevNow = steady_clock::now();

	if (dur.count() <= 0)
	{
		isExpired = true;
	}
}

const bool* timer::getIsExpired()
{
	return &isExpired;
}

void timer::restartTimer()
{
	isExpired = false;
	prevNow = steady_clock::now();
	setTimer(secDur);
}


template <typename T>
T remapValue(T inputV, T inputMin, T inputMax, T outputMin, T outputMax, bool clamp)
{
	if (clamp)
	{
		if (inputV <= inputMin)
		{
			inputV = inputMin;
		}
		else if (inputV >= inputMax)
		{
			inputV = inputMax;
		}
	}

	T normIn = inputMax - inputMin;
	T normOut = outputMax - outputMin;
	T devider = normOut / normIn;

	return ((inputV - inputMin) * devider) + outputMin;
}

// SharedPtr
//#ifdef ListNode
template<typename T>
SharedPtr<T>::SharedPtr() {
    ptr_ = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr) {
    ptr_ = ptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>& ptr) {
    ptr_ = SharedPtr<T>(ptr.Get());
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& ptr) {
    ptr_ = ptr.Release();
}

template<typename T>
T* SharedPtr<T>::Get() {
    return ptr_;
}

template<typename T>
T* SharedPtr<T>::Release() {

    T* ptr_temp = ptr_;
    ptr_ = nullptr;
    return ptr_temp;
}

template<typename T>
void SharedPtr<T>::Reset(T* ptr) {
    ptr_ = ptr;
}

template<typename T>
void SharedPtr<T>::Reset() {
    delete ptr_;
    ptr_ = nullptr;
}

template<typename T>
void SharedPtr<T>::swap(T* ptr) {
    T* temp_ptr = ptr;
    ptr = ptr_;
    ptr_ = temp_ptr;
}

template<typename T>
T& SharedPtr<T>::operator *() {
    return *ptr_;
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    if (ptr_ != nullptr)
    {
        delete ptr_;
        ptr_ = nullptr;
    }
}

void TextEditor::edit() {
    getline(cin, input_text);

    if (prev_text.empty())
    {
        for (const char& c : input_text) {
            prev_text.push_back(c);
        }
        return;
    }

    for (int i = 0; i < static_cast<int>(input_text.size()) && i < prev_text.size(); i++) {

        switch (input_text[i]) {
        case ' ':
            break;

        case '-':
            prev_text.erase(i);
            break;
        
        default:
            if (i <= prev_text.size()) {
                prev_text.insert(i, input_text[i]);
            }
            else {
                prev_text.push_back(input_text[i]);
            }
            break;
        }
    }

    MoveCursorUp();
    system("cls");

    for (int i(0); i < prev_text.size(); i++) {
        cout << prev_text[i];
    }
    cout << '\n';
}

TextEditor::TextEditor() {
    h_out = GetStdHandle(STD_OUTPUT_HANDLE);
}

void TextEditor::MoveCursorUp() {
    SetConsoleCursorPosition(h_out, {0, 0});
}

void list_tester() {
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
    for (int i = 0; i < list_test.size(); i++) {
        cout << list_test[i];
    }
    cout << "push_back: "s << '\t' << push_back << '\n';
    cout << "push_front: "s << '\t' << push_front << '\n';
    cout << "insert: "s << '\t' << insert << '\n';
    cout << "erase: "s << '\t' << erase << '\n';

    cout << '\n';
    for (int i : list_std) {
        cout << i;
    }

    cout << '\n';
}