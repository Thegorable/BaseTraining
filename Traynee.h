#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <string>
#include <memory>
#include <list>
#include "List.h"

using namespace std;
using namespace chrono;

template <typename T>
T remapValue(T inputV, T inputMin, T inputMax, T outputMin, T outputMax, bool clamp);

class timer
{
public:
	timer(short sec);

	void setTimer(short sec);
	void updateTimer();
	const bool* getIsExpired();
	void restartTimer();

private:
	short secDur;
	duration<long long, nano> dur;
	time_point<steady_clock, duration<long long, nano>> prevNow;
	bool isExpired;

};

void list_tester();

class TextEditor
{
public:
    TextEditor();
    void edit();

private: //variables
    List<char> prev_text;
    HANDLE h_out;
    string input_text;

private: // methods
    void MoveCursorUp();
};