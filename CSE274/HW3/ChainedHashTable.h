/*
 * ChainedHashTable.h
 *
 *  Created on: 2011-11-30
 *      Author: morin
 */

#ifndef CHAINEDHASHTABLE_H_
#define CHAINEDHASHTABLE_H_
#include <climits>
#include <functional>
#include <cstdlib>
//#include "utils.h"
#include "array.h"
//#include "ArrayStack.h"
#include "DLList.h"
#include <iostream>
#include <cmath>

namespace ods {

template<class T>
class ChainedHashTable {
private:
	std::hash<T> T_Hash_Fn;		// functor used to get hash code
	int prime_generator(int n);

protected:
	//typedef ArrayStack<T> List;
	typedef DLList<T> List;
	T null;
	array<List> t;
	int n;			 // size of elements
	int d;
	int z;			 // random odd number used in hash function
	static const int w = 32; //sizeof(int)*8;

	float loadFactor;
	int prime;

	void allocTable(int m);
	void resize();
	int hash(T x); /*{ return //((unsigned)(z * hashCode(x))) >> (w-d); }*/

public:
	ChainedHashTable();
	virtual ~ChainedHashTable();
	bool add(T x);
	T remove(T x);
	T find(T x);
	int size() {
		return n;
	}
	void clear();
	
	/*new functions section*/
	void SetLocalFactor(float f);
	int GetLongestList();
};

/**
 * FIXME:  A copy-constructor for arrays would be useful here
 */
template<class T>
void ChainedHashTable<T>::resize() {
	d = 1;
	while (1<<d <= (int)(n / loadFactor)) d++;
    	n = 0;
	array<List> newTable(1<<d);
	prime = prime_generator(newTable.length);
	for (int i = 0; i < t.length; i++) {
		for (int j = 0; j < t[i].size(); j++) {
			T x = t[i].get(j);
			newTable[hash(x)].add(x);
			n++;
		}
	}
	t = newTable;
}

template<typename T>
int ChainedHashTable<T>::prime_generator(int n) {
	int tmp = (rand() % 1000 + 1000) * n, i = n + 1;
	for (; i < tmp; i++) {
	    if (isPrime(i))
		 break;
	}
	return i;
}

// new Hash function
template<typename T>
int ChainedHashTable<T>::hash(T x) {
	// first, get the hash code of the data, then compress the hash code
	//size_t hashCode = T_Hash_Fn(x);
	// second, compress the hash code so that the result is within 0 to t.length - 1
	int index = abs(((x + z) % prime) % (1 << d));	// assume x is int
	//std::cout << index << std::endl;
	return index;
}


template<class T>
ChainedHashTable<T>::ChainedHashTable() : t(2) {
	n = 0;
	d = 1;
	null = INT_MIN;
	z = rand() | 1;     // is a random odd integer
	loadFactor = 1.0f;
	prime = prime_generator(t.length);
}


template<class T>
ChainedHashTable<T>::~ChainedHashTable() {
}


template<class T>
bool ChainedHashTable<T>::add(T x) {
	//std::cout << x << std::endl;
	if (find(x) != null)
		return false;
	if (n + 1 > loadFactor * t.length)
		resize();
	t[hash(x)].add(x);
	n++;
	return true;
}


template<class T>
T ChainedHashTable<T>::remove(T x) {
	int j = hash(x);
	for (int i = 0; i < t[j].size(); i++) {
		T y = t[j].get(i);
		if (x == y) {
			t[j].remove(i);
			n--;
			return y;
		}
	}
	return null;
}


template<class T>
T ChainedHashTable<T>::find(T x) {
	int j = hash(x);
	for (int i = 0; i < t[j].size(); i++)
		if (x == t[j].get(i))
			return t[j].get(i);
	return null;
}


template<class T>
void ChainedHashTable<T>::clear() {
	n = 0;
	d = 1;
	array<List> b(2);
	t = b;
}

/*new functions section*/
template<typename T>
inline void ChainedHashTable<T>::SetLocalFactor(float f) {
	loadFactor = f;
}

template<typename T>
int ChainedHashTable<T>::GetLongestList() {
	if (t.length == 0) 
		return 0;

	int longestLength = 0;
	for (int i = 0; i < t.length; i++) {
		longestLength = longestLength < t[i].size() ? t[i].size() : longestLength;
	}
	
	return longestLength;
}

} /* namespace ods */
#endif /* CHAINEDHASHTABLE_H_ */
