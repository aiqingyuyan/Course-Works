/*
 * ChainedHashTable.h
 *
 *  Created on: 2011-11-30
 *      Author: morin
 */

#ifndef CHAINEDHASHTABLE_H_
#define CHAINEDHASHTABLE_H_
#include <climits>
#include "utils.h"
#include "array.h"
#include "ArrayStack.h"
#include "List.h"

namespace ods {

template<class T>
class ChainedHashTable : List<T> {
private:
    long prime_generator(int n);
    
protected:
	typedef ArrayStack<T> List;
	T null;
	array<List> t;
	int n;
	int d;
	int z;
	static const int w = 32; //sizeof(int)*8;
    long prime;
	void allocTable(int m);
	void resize();
	int hash(T x);

public:
	ChainedHashTable();
	virtual ~ChainedHashTable();
	bool add(T x);
	bool remove(T x);
	T find(T x);
	int size() {
		return n;
	}
	void clear();
};

/**
 * FIXME:  A copy-constructor for arrays would be useful here
 */
template<class T>
void ChainedHashTable<T>::resize() {
	d = 1;
	while (1<<d <= n) d++;
    n = 0;
	array<List> newTable(1<<d);
    prime = prime_generator(newTable.length);
	for (int i = 0; i < t.length; i++) {
		for (int j = 0; j < t[i].size(); j++) {
			T x = t[i].get(j);
			newTable[hash(x)].add(x);
		}
	}
	t = newTable;
}

template<typename T>
long ChainedHashTable<T>::prime_generator(int n) {
	int tmp = (rand() % 1000 + 1000) * n, i = n + 1;
	for (; i < tmp; i++) {
	    if (isPrime(i))
		 break;
	}
	return i;
}

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
    prime = prime_generator(t.length);
}



template<class T>
ChainedHashTable<T>::~ChainedHashTable() {
}


template<class T>
bool ChainedHashTable<T>::add(T x) {
	if (find(x) != null) return false;
	if (n+1 > t.length) resize();
	t[hash(x)].add(x);
	n++;
	return true;
}


template<class T>
bool ChainedHashTable<T>::remove(T x) {
	int j = hash(x);
	for (int i = 0; i < t[j].size(); i++) {
		T y = t[j].get(i);
		if (x == y) {
			t[j].remove(i);
			n--;
			return true;
		}
	}
	return false;
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

} /* namespace ods */
#endif /* CHAINEDHASHTABLE_H_ */
