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
#include <functional>
#include <cstdlib>
using namespace std;
#include "array.h"
namespace ods {

//extern unsigned int tab[4][256];

template<class T>
class LinearHashTable {
private:
	std::hash<T> T_Hash_Fn;
	int prime_generator(int n);

protected:
	static const int w = 32;
	static const int r = 8;
	//array<T> t;
	array<T> first, second;
	int n;   // number of items in T
	int q;   // number of non-null entries in T
	int d;   // t.length = 2^d
	
	int tableLength;  // tableLength = first.length + second.length
	int z;   // random odd number used in hash function
	int prime;
	
	T null, del;
	void resize();
	int hash(T x);
	int hash2(T x);

public:
	// FIXME: get rid of default constructor
	LinearHashTable();
	LinearHashTable(T null, T del);
	virtual ~LinearHashTable();
	bool add(T x);
	//bool addSlow(T x);
	T remove(T x);
	T find(T x);
	int size() { return n; }
	void clear();
	// FIXME: yuck
	void setNull(T null) { this->null = null; first.fill(null); second.fill(null); }
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
LinearHashTable<T>::LinearHashTable() : /*t(2),*/ first(1), second(1) {
/*
	this->null = null;
	this->del = del;
*/
	n = 0;
	q = 0;
	d = 1;
	z = rand() | 1;     // is a random odd integer
	tableLength = first.length + second.length;
	prime = prime_generator(tableLength/*t.length*/);
}


template<class T>
LinearHashTable<T>::LinearHashTable(T null, T del) : /*t(2, null),*/ first(1, null), second(1, null) {
	this->null = null;
	this->del = del;
	n = 0;
	q = 0;
	d = 1;
	z = rand() | 1;     // is a random odd integer
	tableLength = first.length + second.length;
	prime = prime_generator(tableLength/*t.length*/);
}

template<class T>
LinearHashTable<T>::~LinearHashTable() {
}

template<typename T>
int LinearHashTable<T>::prime_generator(int n) {
	int tmp = (rand() % 1000 + 1000) * n, i = n + 1;
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

template<typename T>
int LinearHashTable<T>::hash2(T x) {
	int i = 1 + hash(x) % (tableLength/*t.length*/ - 1);
	
	return i;
}

template<class T>
void LinearHashTable<T>::resize() {
	d = 1;
	while ((1<<d) < 3 * n) d++;
	array<T> tnew(1<<d, null);
	array<T> firstNew(1<<(d-1), null);
	array<T> secondNew(1<<(d-1), null);
	tableLength = firstNew.length + secondNew.length;
	q = n;

	// insert everything into tnew
	/*
	for (int k = 0; k < t.length; k++) {
		if (t[k] != null && t[k] != del) {
			int i = hash(t[k]), j = 0;
			while (tnew[i] != null) {
				i = (i + (++j) * hash2(t[k])) % t.length;
			}
			tnew[i] = t[k];
		}
	}
	t = tnew;
	*/
	// loop first array
	for (int k = 0; k < first.length; k++) {
		if (first[k] != null && first[k] != del) {
			int i = hash(first[k]), j = 0;
			while (true) {
				if (i <= tableLength / 2 - 1) {
					if (firstNew[i] == null)
						break;
				}
				else {
					if (secondNew[i - tableLength / 2] == null)
						break;
				}
				i = (i + (++j) * hash2(first[k])) % /*t.length*/ tableLength;
			}
			if (i <= tableLength / 2 - 1)
				firstNew[i] = first[k];
			else
				secondNew[i - tableLength / 2] = first[k];
		}
	}

	// loop second array
	for (int k = 0; k < second.length; k++) {
		if (second[k] != null && second[k] != del) {
			int i = hash(second[k]), j = 0;
			while (true) {
				if (i <= tableLength / 2 - 1) {
					if (firstNew[i] == null)
						break;
				}
				else {
					if (secondNew[i - tableLength / 2] == null)
						break;
				}
				i = (i + (++j) * hash2(second[k])) % /*t.length*/ tableLength;
			}
			if (i <= tableLength / 2 - 1)
				firstNew[i] = second[k];
			else
				secondNew[i - tableLength / 2] = second[k];
		}
	}

	first = firstNew;
	second = secondNew;
}

template<class T>
void LinearHashTable<T>::clear() {
	n = 0;
	q = 0;
	d = 1;
	//array<T> tnew(2, null);
	//t = tnew;
	array<T> firstNew(1, null);
	array<T> secondNew(1, null);
	first = firstNew;
	second = secondNew;
	tableLength = first.length + second.length;
	prime = prime_generator(tableLength);
}

template<class T>
bool LinearHashTable<T>::add(T x) {
	if (find(x) != null) return false;
	if (2*(q+1) > /*t.length*/ tableLength) resize();   // max 50% occupancy
	int i = hash(x), j = 0;
	/*
	while (t[i] != null && t[i] != del) {
		//i = (i == t.length-1) ? 0 : i + 1; // increment i
		i = (i + (++j) * hash2(x)) % t.length;
	}
	*/
	while (true) {
		if (i <= tableLength / 2 - 1) {
			if (first[i] == null)
				break;
		}
		else {
			if (second[i - tableLength / 2] == null)
				break;
		}
		i = (i + (++j) * hash2(x)) % /*t.length*/ tableLength;
	}
	
	if (i <= tableLength / 2 - 1) {
		if (first[i] == null) 
			q++;
		first[i] = x;
	}
	else {
		if (second[i - tableLength / 2] == null)
			q++;
		second[i - tableLength / 2] = x;
	}
		
	//if (t[i] == null) q++;
	n++;
	//t[i] = x;
	return true;
}

template<class T>
T LinearHashTable<T>::find(T x) {
	int i = hash(x), j = 0;
	/*
	while (t[i] != null) {
		if (t[i] != del && t[i] == x) return t[i];
		//i = (i == t.length-1) ? 0 : i + 1; // increment i
		i = (i + (++j) * hash2(x)) % t.length;
	}
	*/
	while (true) {
		if (i <= tableLength / 2 - 1) {
			if (first[i] == null)
				break;
			if (first[i] != null && first[i] == x)
				return first[i];
				
		}
		else {
			if (second[i - tableLength / 2] == null)
				break;
			if (second[i - tableLength / 2] != null &&
			    second[i - tableLength / 2] == x)
				return second[i - tableLength / 2];
		}
		i = (i + (++j) * hash2(x)) % /*t.length*/ tableLength;
	}
	
	return null;
}

template<class T>
T LinearHashTable<T>::remove(T x) {
	int i = hash(x), j = 0;
	/*
	while (t[i] != null) {
		T y = t[i];
		if (y != del && x == y) {
			t[i] = del;
			n--;
			if (8*n < t.length) resize(); // min 12.5% occupancy
			return y;
		}
		//i = (i == t.length-1) ? 0 : i + 1;  // increment i
		i = (i + (++j) * hash2(x)) % t.length;
	}
	*/
	while (true) {
		if (i <= tableLength / 2 - 1) {
			if (first[i] == null)
				break;
			T y = first[i];	
			if (y != del && x == y) {
				first[i] = del;
				n--;
				if (8*n < tableLength) resize(); // min 12.5% occupancy
				return y;
			}
		}
		else {
			if (second[i - tableLength / 2] == null)
				break;
			T y = second[i - tableLength / 2];	
			if (y != del && x == y) {
				second[i - tableLength / 2] = del;
				n--;
				if (8*n < tableLength) resize(); // min 12.5% occupancy
				return y;
			}
		}
		i = (i + (++j) * hash2(x)) % /*t.length*/ tableLength;
	}
	return null;
}

/*
template<class T>
bool LinearHashTable<T>::addSlow(T x) {
	if (2*(q+1) >  tableLength) resize();   // max 50% occupancy
	int i = hash(x), j = 0;
	while (t[i] != null) {
		if (t[i] != del && x.equals(t[i])) return false;
		//i = (i == t.length-1) ? 0 : i + 1; // increment i
		i = (i + (++j) * hash2(x)) % t.length;
	}
	t[i] = x;
	n++; q++;
	return true;
}
*/

} /* namespace ods */
#endif /* LINEARHASHTABLE_H_ */
