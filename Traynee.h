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

#define LIST_MEMORY_TEST

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


template<typename T>
class SharedPtr
{
public:
    SharedPtr();
    SharedPtr(T* ptr);
    SharedPtr(SharedPtr& ptr);
    SharedPtr(SharedPtr&& ptr);

    T* Get();
    T* Release();
    void Reset(T* ptr);
    void Reset();
    void swap(T* ptr);

    T& operator *();

    ~SharedPtr();

private:
    T* ptr_;

};



template <typename T>
class List
{
private:
    struct Node {
        shared_ptr<T> elem_ptr;
        shared_ptr<Node> next_node_ptr;
        shared_ptr<Node> prev_node_ptr;

        Node() {}

        Node(const T& elem) {
            elem_ptr.reset(new T { elem });
        }

        Node(T* elem_ptr) {
            this->elem_ptr.reset(elem_ptr);
        }

        Node(const T& elem, const shared_ptr<Node>& next_node_ptr) {
            elem_ptr.reset(new T{ elem });
            this->next_node_ptr = next_node_ptr;
        }
        
        Node(T* elem_ptr, const shared_ptr<Node>& next_node_ptr) {
            this->elem_ptr.reset(elem_ptr);
            this->next_node_ptr = next_node_ptr;
        }
        
        Node(const T& elem, const shared_ptr<Node>& next_node_ptr, const shared_ptr<Node>& prev_node_ptr) {
            elem_ptr.reset(new T{ elem });
            this->next_node_ptr = next_node_ptr;
            this->prev_node_ptr = prev_node_ptr;
        }
        
        Node(T* elem_ptr, const shared_ptr<Node>& next_node_ptr, const shared_ptr<Node>& prev_node_ptr) {
            this->elem_ptr.reset(elem_ptr);
            this->next_node_ptr = next_node_ptr;
            this->prev_node_ptr = prev_node_ptr;
        }
        
        Node(const T& elem, int, const shared_ptr<Node>& prev_node_ptr) {
            elem_ptr.reset(new T{ elem });
            this->prev_node_ptr = prev_node_ptr;
        }
        
        Node(T* elem_ptr, int, const shared_ptr<Node>& prev_node_ptr) {
            this->elem_ptr.reset(elem_ptr);
            this->prev_node_ptr = prev_node_ptr;
        }
    };

    shared_ptr<Node> first_node_;
    shared_ptr<Node> tail_node_;
    int size_;

#ifdef LIST_MEMORY_TEST
    vector<Node*> nodes_;

#endif


public:
    List();
    List(const T& elem);
    List(const T& elem, const T& next);
    List(List& list);
    List(List&& list) noexcept;
    List(size_t num, const T& elem);

    T& front();
    T& back();
    void insert(int num, const T& elem);
    void push_back(const T& elem);
    void push_front(const T& elem);
    void erase(int num);
    void pop_back();
    void pop_front();
    void clear();
    bool empty();

    T& operator [](int num);

    inline int size();

    ~List();

private:
    shared_ptr<Node> find_node(int num);
    void push_first_node(const T& elem);
    void push_next_node(const T& elem);
};

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


template <typename T>
List<T>::List() {
    first_node_.reset();
    tail_node_.reset();
    size_ = 0;
}

template <typename T>
List<T>::List(const T& elem) {
    push_first_node(elem);
}

template <typename T>
List<T>::List(const T& elem, const T& next) {
    push_first_node(elem);
    push_next_node(next);
}

template <typename T>
List<T>::List(List& list) {
    for (int num = 0; num < list.size(); num++) {
        this->push_back(list[num]);
    }
}

template <typename T>
List<T>::List(List&& list) noexcept {
    this->first_node_ = move(list.first_node_);
    this->tail_node_ = move(list.tail_node_);
    this->size_ = list.size_;
    list.size_ = 0;
}

template <typename T>
List<T>::List(size_t num, const T& elem) {
    for (int i(0); i < num; i++) {
        push_back(elem);
    }
}

template <typename T>
List<T>::~List() {
    if (size_) {
        clear();
    }
}

template <typename T>
void List<T>::push_back(const T& elem) {
    switch (size_) {
    case 0:
        push_first_node(elem);
        break;
    case 1:
        push_next_node(elem);
        break;
    default:
        tail_node_->next_node_ptr.reset(new Node(new T{ elem }, NULL, tail_node_));
        tail_node_->prev_node_ptr->next_node_ptr = tail_node_;
        tail_node_ = tail_node_->next_node_ptr;
        size_++;

#ifdef LIST_MEMORY_TEST
        nodes_.push_back(tail_node_.get());
#endif // LIST_MEMORY_TEST
    }
}

template <typename T>
void List<T>::push_front(const T& elem) {
    switch (size_) {
    case 0:
        push_first_node(elem);
        break;
    case 1:
        push_next_node(elem);
        break;
    default:
        first_node_.reset(new Node(new T{ elem }, first_node_));
        first_node_->next_node_ptr->prev_node_ptr = first_node_;
        size_++;

#ifdef LIST_MEMORY_TEST
        nodes_.push_back(first_node_.get());
#endif // LIST_MEMORY_TEST
    }
}

template <typename T>
void List<T>::pop_front() {
    if (!size_) {
        return;
    }

    size_--;
    first_node_ = first_node_->next_node_ptr;
}

template <typename T>
void List<T>::pop_back() {
    if (!size_) {
        return;
    }

    size_--;
    tail_node_ = tail_node_->prev_node_ptr;
    tail_node_->next_node_ptr.reset();
}

template <typename T>
shared_ptr <typename List<T>::Node> List<T>::find_node(int num) { // figure out with that shity type definition
    if (num >= size_ || num < 0) {
        throw out_of_range("Class List - method find_node - attempt to access out of range data"s);
    }
    
    shared_ptr<Node> temp = first_node_;
    if (num / 2 <= size_) {
        for (int i = 0; i < num; i++) {
            temp = temp->next_node_ptr;
        }
    }
    else {
        temp = tail_node_;
        for (int i = size_ - 1; i >= num; i--) {
            temp = temp->prev_node_ptr;
        }
    }

    return temp;
}

template <typename T>
void List<T>::push_first_node(const T& elem) {
    first_node_.reset(new Node(new T{ elem }));
    tail_node_ = first_node_;
    size_ = 1;

#ifdef LIST_MEMORY_TEST
    nodes_.push_back(first_node_.get());
#endif // LIST_MEMORY_TEST

}

template <typename T>
void List<T>::push_next_node(const T& elem) {
    first_node_->next_node_ptr.reset(new Node(new T{ elem }));
    tail_node_ = first_node_->next_node_ptr;
    tail_node_->prev_node_ptr = first_node_;
    size_ = 2;

#ifdef LIST_MEMORY_TEST
    nodes_.push_back(tail_node_.get());
#endif // LIST_MEMORY_TEST
}

template <typename T>
void List<T>::insert(int num, const T& elem) {
    if (!num) {
        this->push_front(elem);
    }
    else if (num == size_ - 1) {
        this->push_back(elem);
    }
    else {
        const shared_ptr<Node> node_ptr = find_node(num);
        shared_ptr<Node> new_node_ptr(new Node(new T{ elem }, node_ptr, node_ptr->prev_node_ptr) );
        node_ptr->prev_node_ptr->next_node_ptr = new_node_ptr;
        node_ptr->prev_node_ptr = new_node_ptr;

        size_++;

#ifdef LIST_MEMORY_TEST
        auto it = nodes_.begin();
        advance(it, num);
        nodes_.insert(it, new_node_ptr.get());
#endif // LIST_MEMORY_TEST
    }

}

template <typename T>
void List<T>::erase(int num) {
    if (!num) {
        pop_front();
    }   
    else if (num == size_ - 1) {
        pop_back();
    }
    else {
        shared_ptr<Node> node_ptr = find_node(num);
        node_ptr->prev_node_ptr->next_node_ptr = node_ptr->next_node_ptr;
        node_ptr->next_node_ptr->prev_node_ptr = node_ptr->prev_node_ptr;
        size_--;
    }
}

template <typename T>
T& List<T>::operator [](int num) {
    shared_ptr<Node> node_ptr = find_node(num);
    return *(node_ptr->elem_ptr);
}

template <typename T>
void List<T>::clear() {
    
    shared_ptr<Node>& temp = first_node_;
    temp = first_node_;
    for (int i = 0; i < size_ - 1; i++) {
        temp = temp->next_node_ptr;
        temp->prev_node_ptr.reset();
    }
    temp.reset();
    tail_node_.reset();
    
    size_ = 0;
}

template <typename T>
int List<T>::size() {
    return size_;
}

template <typename T>
bool List<T>::empty() {
    return !(static_cast<bool>(size_));
}

template <typename T>
T& List<T>::front() {
    return *(first_node_->elem_ptr);
}

template <typename T>
T& List<T>::back() {
    return *(tail_node_->elem_ptr);
}

void list_tester();
