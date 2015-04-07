//
//  trequeue.h
//  Trequeue
//
//  Created by Yan Yu on 9/5/14.
//  Copyright (c) 2014 Yan Yu. All rights reserved.
//

#ifndef Trequeue_trequeue_h
#define Trequeue_trequeue_h

#include "array.h"

//class declaration:

template <typename T>
class Trequeue
{
private:
    array<T> first,second;            // two back up arrays
    int head_first, head_second;      // used to record the head indexs of first and second subqueues
    int count_total;                  // number of elements currently in queue
    int count_first, count_second;    // used to number of elements in first and second subqueues
    
protected:
    // resize the back up array if current number of elements in queue plus 1 is greater than back up array size
    void resize(array<T> &a, int count_a, int &head);
    
    // balacne the two back up arays, so the difference between the actual number of elements in them is less than or equal to 1
    void balanceArrays();
    
    // insert at the index of subqueue
    void insertAt(array<T> &a, int pos, const T &x, int &head, int &count);
    
    // delete at the index of subqueue
    void deleteAt(array<T> &a, int pos, int &head, int &count);
    
public:
    Trequeue();                 // constructor
    virtual ~Trequeue();        // destructor
    
    T get(int i);               // get the ith element in queue
    T set(int i, T x);          // set the ith element to x;
    int size();                 // return the size of trequeue
    void add(int i, T x);       // insert x to ith pos
    T remove(int i);            // remove ith element
    void clear();
};

// implentation:

template<typename T>
Trequeue<T>::Trequeue() : first(1), second(1),  head_first(0), head_second(0),
                            count_total(0), count_first(0), count_second(0) {
    
}

template<typename T>
Trequeue<T>::~Trequeue() {}

template<typename T>
inline T Trequeue<T>::get(int i) {
    assert( i >= 0 && i < count_total);
    
    if ((i - count_first) < 0) {
        return first[(head_first + i) % first.length];
    }
    else {
        return second[(head_second + (i - count_first)) % second.length];
    }
}

template<typename T>
inline T Trequeue<T>::set(int i, T x) {
    assert( i >= 0 && i < count_total);
    
    T tmp;
    
    if ((i - count_first) < 0) {
        tmp = first[(head_first + i) % first.length];
        first[(head_first + i) % first.length] = x;
    }
    else {
        tmp = second[(head_second + (i - count_first)) % second.length];
        second[(head_second + (i - count_first)) % second.length] = x;
    }
    
    return tmp;
}

template<typename T>
inline int Trequeue<T>::size() {
    //std::cout << "first size: " << count_first << " " << "second size: " << count_second << std::endl;
    return count_total;
}

template<typename T>
void Trequeue<T>::resize(array<T> &a, int count_a, int &head) {
    array<T> b(std::max(1, 2 * count_a));
    
    for (int i = 0; i < count_a; i++) {
        b[i] = a[(head + i) % a.length];
    }
    
    a = b;
    head = 0;
}

template<typename T>
void Trequeue<T>::balanceArrays() {
    // if the difference between the two subqueue is less than 1, then they are balanced
    if (std::abs(count_second - count_first) <= 1)
        return ;
    
    if (count_first > count_second) {       // move the last element in first subqueue to the first element in second subqueue
        if(count_second + 1 > second.length) resize(second, count_second, head_second);
        insertAt(second, 0, get(count_first - 1), head_second, count_second);
        count_first--;
    }
    else {                                  // move the first element in second subqueue to the last element in first subqueue
        if (count_first + 1 > first.length) resize(first, count_first, head_first);
        first[(head_first + count_first) % first.length] = second[(head_second + 0) % second.length];
        count_first++;
        count_second--;
        head_second = (head_second + 1) % second.length;
    }
}

template<typename T>
void Trequeue<T>::insertAt(array<T> &a, int pos, const T &x, int &head, int &count) {
    if (pos < count/2) {
        head = (head == 0) ? a.length - 1 : head - 1;
        
        for(int j = 0; j < pos; j++) {
            a[(head + j) % a.length] = a[(head + j + 1) % a.length];
        }
        
        a[(head + pos) % a.length] = x;
    }
    else {
        for (int j = count; j > pos; j--) {
            a[(head + j) % a.length] = a[(head + j - 1) % a.length];
        }
        
        a[(head + pos) % a.length] = x;
    }
    
    count++;
}

template<typename T>
void Trequeue<T>::deleteAt(array<T> &a, int pos, int &head, int &count) {
    if (pos < count/2) {
        for (int j = pos; j > 0; j--) {
            a[(head + j) % a.length] = a[(head + j - 1) % a.length];
        }
        
        head = (head + 1) % a.length;
    }
    else {
        for (int j = pos; j < count - 1; j++) {
            a[(head + j) % a.length] = a[(head + j + 1) % a.length];
        }
    }
    
    count--;
    
    if(a.length > 3 * count) resize(a, count, head);
}

template<typename T>
void Trequeue<T>::add(int i, T x) {
    assert(i >= 0);
    
    if (i <= count_total/2) {       // insert at first subqueue
        if (count_first + 1 > first.length) resize(first, count_first, head_first);
        
        insertAt(first, i, x, head_first, count_first);
    }
    else {                          // insert at second subqueue
        int i_in_second = i - count_first;
        
        if(count_second + 1 > second.length) resize(second, count_second, head_second);
        
        insertAt(second, i_in_second, x, head_second, count_second);
    }
    
    count_total++;
    
    balanceArrays();
}

template<typename T>
T Trequeue<T>::remove(int i) {
    assert(i >= 0 && i < count_total);
    
    T tmp ;
    
    if (i < count_total/2) {
        tmp = get(i);
        deleteAt(first, i, head_first, count_first);
    }
    else {
        int i_in_second = i - count_first;
        tmp = get(i_in_second);
        deleteAt(second, i_in_second, head_second, count_second);
    }
    
    count_total--;
    
    balanceArrays();
    
    return tmp;
}

template<typename T>
void Trequeue<T>::clear() {
    head_first = 0;
    head_second = 0;
    count_first = 0;
    count_second = 0;
    count_total = 0;
    
    array<T> a(1);
    array<T> b(1);
    
    first = a;
    second = b;
}


#endif
