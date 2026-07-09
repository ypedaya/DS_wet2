#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

template <class T, class K>
class HashTable
{
private:
    T* data;
    int currentSize; //num of keys
    int currentCapacity; //size of array
    float loadFactor

public:
    HashTable();
    ~HashTable();

    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);

    int size() const;
    int capacity() const;
    bool empty() const;

    T& operator[](int index);
    const T& operator[](int index) const; //הנחתי שאם אני ניגדשת arr[] ולא קיים איבר כזה הוא מחזיר לי nullPtr
    void remove(int index);

    //bool operator==(const DynamicArray& other) const;
    //bool operator!=(const DynamicArray& other) const;
};


#endif //DYNAMICARRAY_H
