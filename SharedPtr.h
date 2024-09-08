#pragma once
template<typename T>
class SharedPtr
{
public:
    SharedPtr();
    SharedPtr(T* ptr);
    SharedPtr(const SharedPtr& s_ptr);
    SharedPtr(SharedPtr&& s_ptr) noexcept;

    T* get();
    void reset(T* elem_ptr);
    void reset();
    void swap(SharedPtr& s_ptr);

    T& operator *();
    T* operator ->();
    SharedPtr& operator =(const SharedPtr& s_ptr);
    SharedPtr& operator =(SharedPtr&& s_ptr) noexcept;

    ~SharedPtr();

private:
    T* ptr_;
    int* count_;

    void reduce_count(int num = 1);
};

template<typename T>
SharedPtr<T>::SharedPtr() {
    ptr_ = nullptr;
    count_ = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr) {
    ptr_ = ptr;
    count_ = new int{ 1 };
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& s_ptr) {
    ptr_ = s_ptr.ptr_;
    count_ = s_ptr.count_;
    ++*count_;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& ptr) noexcept {
    ptr_ = ptr.ptr_;
    count_ = ptr.count_;
    ptr.ptr_ = nullptr;
    ptr.count_ = nullptr;
}

template<typename T>
T* SharedPtr<T>::get() {
    return ptr_;
}

template<typename T>
void SharedPtr<T>::reset(T* elem_ptr) {
    reduce_count(1);
    ptr_ = elem_ptr;
    count_ = new int{ 1 };
}

template<typename T>
void SharedPtr<T>::reset() {
    reduce_count(1);
    ptr_ = nullptr;
    count_ = nullptr;
}

template<typename T>
void SharedPtr<T>::swap(SharedPtr<T>& s_ptr) {
    T* temp_ptr = s_ptr.ptr_;
    int* temp_count = s_ptr.count_;

    s_ptr.ptr_ = ptr_;
    s_ptr.count_ = count_;

    ptr_ = temp_ptr;
    count_ = temp_count;
}

template<typename T>
T& SharedPtr<T>::operator *() {
    return *ptr_;
}

template<typename T>
T* SharedPtr<T>::operator ->() {
    return ptr_;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator =(const SharedPtr<T>& s_ptr) {
    reduce_count(1);
    ptr_ = s_ptr.ptr_;
    count_ = s_ptr.count_;
    ++*count_;

    return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator =(SharedPtr<T>&& s_ptr) noexcept {
    reduce_count(1);
    ptr_ = s_ptr.ptr_;
    count_ = s_ptr.count_;
    s_ptr.ptr_ = nullptr;
    s_ptr.count_ = nullptr;

    return *this;
}

template<typename T>
void SharedPtr<T>::reduce_count(int num) {
    if (count_ != nullptr) {
        *count_ -= num;
        if (!(*count_)) {
            delete ptr_;
            delete count_;
            ptr_ = nullptr;
            count_ = nullptr;
        }
    }
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    reduce_count(1);
    if (ptr_ != nullptr && !count_) {
        delete ptr_;
        ptr_ = nullptr;
    }
}