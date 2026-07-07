//
// Created by yaara.p on 07/07/2026.
//

#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
template<class T>
class DynamicArray {
private:
    T* data;
    int current_size;
    int current_capacity;

public:
    DynamicArray();
    ~DynamicArray();

    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);

    int size() const;
    int capacity() const;
    bool empty() const;

    T& operator[](int index);
    const T& operator[](int index) const;

    //bool operator==(const DynamicArray& other) const;
    //bool operator!=(const DynamicArray& other) const;
};


#endif //DYNAMICARRAY_H
