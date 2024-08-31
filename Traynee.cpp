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
//#endif

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
        auto it = prev_text.begin();

        switch (input_text[i]) {
        case ' ':
            advance(it, i);
            break;

        case '-':
            advance(it, i);
            prev_text.erase(it);
            break;
        
        default:
            advance(it, i);
            if (i <= prev_text.size()) {
                prev_text.insert(it, input_text[i]);
            }
            else {
                prev_text.push_back(input_text[i]);
            }
            break;
        }
    }

    MoveCursorUp();
    system("cls");

    for (const char& c : prev_text) {
        cout << c;
    }
    cout << '\n';
}

TextEditor::TextEditor() {
    h_out = GetStdHandle(STD_OUTPUT_HANDLE);
}

void TextEditor::MoveCursorUp() {
    SetConsoleCursorPosition(h_out, {0, 0});
}