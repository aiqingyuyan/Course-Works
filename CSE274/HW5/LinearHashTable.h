/*
 * LinearHashTable.h
 *
 *  Created on: 2011-12-16
 *      Author: morin
 */

#ifndef LINEARHASHTABLE_H_
#define LINEARHASHTABLE_H_
#include <climits>
#include <iostream>
#include "List.h"
using namespace std;
#include "array.h"
namespace ods {

extern unsigned int tab[4][256];

template<class T>
class LinearHashTable : List<T> {
    long prime_generator(int n);
    
	static const int w = 32;
	static const int r = 8;
	array<T> t;
	int n;   // number of values in T
	int q;   // number of non-null entries in T
	int d;   // t.length = 2^d
	int z;
	T null, del;
    long prime;
    
	void resize();
	int hash(T x);
	// Sample code for the book only -- never use this
	/*
	int idealHash(T x) {
		return tab[hashCode(x) >> w-d];
	}
	*/


public:
	// FIXME: get rid of default constructor
	LinearHashTable();
	LinearHashTable(T null, T del);
	virtual ~LinearHashTable();
	bool add(T x);
	bool addSlow(T x);
	bool remove(T x);
	T find(T x);
	int size() { return n; }
	void clear();
	// FIXME: yuck
	void setNull(T null) { this->null = null; t.fill(null); }
	void setDel(T del) { this->del = del; }
};

/*
template<>
LinearHashTable<int>::LinearHashTable() : t(2, INT_MIN) {
	null = INT_MIN;
	del = INT_MIN + 1;
	n = 0;
	q = 0;
	d = 1;
}
*/

/**
 * FIXME: Dangerous - leaves null and del uninitialized
 */
template<class T>
LinearHashTable<T>::LinearHashTable() : t(2, -1) {

	this->null = -1;
	this->del = -2;

	n = 0;
	q = 0;
	d = 1;
	z = rand() | 1;
    prime = prime_generator(1<<d);
}


template<class T>
LinearHashTable<T>::LinearHashTable(T null, T del) : t(2, null) {
	this->null = null;
	this->del = del;
	n = 0;
	q = 0;
	d = 1;
	z = rand() | 1;
    prime = prime_generator(1<<d);
}

template<class T>
LinearHashTable<T>::~LinearHashTable() {
}

template<typename T>
long LinearHashTable<T>::prime_generator(int n) {
	long tmp = (rand() % 1000 + 1000) * n, i = n + 1;
	for (; i < tmp; i++) {
	    if (isPrime(i))
		 break;
	}
	return i;
}

template<typename T>
int LinearHashTable<T>::hash(T x) {
	// first, get the hash code of the key, then compress the hash code
	//size_t hashCode = T_Hash_Fn(x);
	// second, compress the hash code so that the result is within 0 to t.length - 1
	int index = abs(((int)(x + z) % prime) % (1 << d))/*t.length*/;

	return index;
}

template<class T>
void LinearHashTable<T>::resize() {
	d = 1;
	while ((1<<d) < 3*n) d++;
	array<T> tnew(1<<d, null);
    prime = prime_generator(1<<d);
	q = n;
	// insert everything into tnew
	for (int k = 0; k < t.length; k++) {
		if (t[k] != null && t[k] != del) {
			int i = hash(t[k]);
			while (tnew[i] != null)
				i = (i == tnew.length-1) ? 0 : i + 1;
			tnew[i] = t[k];
		}
	}
	t = tnew;
}

template<class T>
void LinearHashTable<T>::clear() {
	n = 0;
	q = 0;
	d = 1;
	array<T> tnew(2, null);
	t = tnew;
}

template<class T>
bool LinearHashTable<T>::add(T x) {
	if (find(x) != null) return false;
	if (2*(q+1) > t.length) resize();   // max 50% occupancy
	int i = hash(x);
	while (t[i] != null && t[i] != del)
		i = (i == t.length-1) ? 0 : i + 1; // increment i
	if (t[i] == null) q++;
	n++;
	t[i] = x;
	return true;
}

template<class T>
T LinearHashTable<T>::find(T x) {
	int i = hash(x);
	while (t[i] != null) {
		if (t[i] != del && t[i] == x) return t[i];
		i = (i == t.length-1) ? 0 : i + 1; // increment i
	}
	return null;
}

template<class T>
bool LinearHashTable<T>::remove(T x) {
	int i = hash(x);
	while (t[i] != null) {
		T y = t[i];
		if (y != del && x == y) {
			t[i] = del;
			n--;
			if (8*n < t.length) resize(); // min 12.5% occupancy
			return true;
		}
		i = (i == t.length-1) ? 0 : i + 1;  // increment i
	}
	return false;
}

template<class T>
bool LinearHashTable<T>::addSlow(T x) {
	if (2*(q+1) > t.length) resize();   // max 50% occupancy
	int i = hash(x);
	while (t[i] != null) {
			if (t[i] != del && x.equals(t[i])) return false;
			i = (i == t.length-1) ? 0 : i + 1; // increment i
	}
	t[i] = x;
	n++; q++;
	return true;
}


} /* namespace ods */
#endif /* LINEARHASHTABLE_H_ */
