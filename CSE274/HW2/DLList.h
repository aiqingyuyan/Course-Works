/*
 * DLList.h
 *
 *  Created on: 2011-11-24
 *      Author: morin
 */

#ifndef DLLIST_H_
#define DLLIST_H_

#include <iostream>

namespace ods {

template<class T>
class DLList {
protected:
	struct Node {
		T x;
		Node *prev, *next;
	};
	Node dummy;
	int n;
	void remove(Node *w);
	Node* addBefore(Node *w, T x);
	Node* getNode(int i);
public:
	DLList();
	virtual ~DLList();
	int size() { return n; }
	T get(int i);
	T set(int i, T x);
	virtual void add(int i, T x);
	virtual void add(T x) { add(size(), x); }
	virtual T remove(int i);
	virtual void clear();
	
	// new methods section for homeword 2
	bool IsPalinddrome() const;
	void Rotate(const int r);
	void Absorb(DLList& l2);
	DLList deal();
};

template<class T>
DLList<T>::DLList() {
	dummy.next = &dummy;
	dummy.prev = &dummy;
	n = 0;
}

template<class T>
typename DLList<T>::Node* DLList<T>::addBefore(Node *w, T x) {
	Node *u = new Node;
	u->x = x;
	u->prev = w->prev;
	u->next = w;
	u->next->prev = u;
	u->prev->next = u;
	n++;
	return u;
}

template<class T>
typename DLList<T>::Node* DLList<T>::getNode(int i) {
	Node* p;
	if (i < n / 2) {
		p = dummy.next;
		for (int j = 0; j < i; j++)
			p = p->next;
	} else {
		p = &dummy;
		for (int j = n; j > i; j--)
			p = p->prev;
	}
	return (p);
}


template<class T>
DLList<T>::~DLList() {
	clear();
}

template<class T>
void DLList<T>::clear() {
	Node *u = dummy.next;
	while (u != &dummy) {
		Node *w = u->next;
		delete u;
		u = w;
	}
    dummy.next = &dummy;
	dummy.prev = &dummy;
	n = 0;
}


template<class T>
void DLList<T>::remove(Node *w) {
	w->prev->next = w->next;
	w->next->prev = w->prev;
	delete w;
	n--;
}


template<class T>
T DLList<T>::get(int i) {
    return getNode(i)->x;
}

template<class T>
T DLList<T>::set(int i, T x) {
	Node* u = getNode(i);
	T y = u->x;
	u->x = x;
	return y;
}

template<class T>
void DLList<T>::add(int i, T x) {
    addBefore(getNode(i), x);
}

template<class T>
T DLList<T>::remove(int i) {
	Node *w = getNode(i);
	T x = w->x;
	remove(w);
	return x;
}

template<typename T>
bool DLList<T>::IsPalinddrome() const {
	if (n <= 1) return true;
      
    Node* p = dummy.next;
    Node* q = dummy.prev;
    
	int i = 0;
    while(i < n/2) {
	   if (p->x != q->x) {
	       return false;
       }   
                     
       p = p->next;
       q = q->prev;
       i++;
    }

    return true;  
}

template<typename T>
void DLList<T>::Rotate(const int r) {
	Node* p = getNode(r);				  // new 0 position 
	Node* q = getNode((n - 1 + r) % n);   // new n - 1 position
	
	Node* old_p = dummy.next; //getNode(0);			  // old 0 position
	Node* old_q = dummy.prev; //getNode(n - 1);	      // old n - 1 position
	
	// update the head node pointing
	dummy.next = p;
	dummy.prev = q;
	
	p->prev = &dummy;
	q->next = &dummy;
	
	old_q->next = old_p;
	old_p->prev = old_q;
}

template<typename T>
void DLList<T>::Absorb(DLList& l2) {
	int l2_size = l2.size();
	
	Node* l1_last_node = dummy.prev;    // getNode(n-1);
	Node* l2_first_node = l2.dummy.next;// l2.getNode(0);
	Node* l2_last_node = l2.dummy.prev; // l2.getNode(l2.size()-1);
	
	l1_last_node->next = l2_first_node;
	l2_first_node->prev = l1_last_node;
	
	this->dummy.prev = l2_last_node;
	l2_last_node->next = &(this->dummy);
	
	l2.dummy.prev = &l2.dummy;
	l2.dummy.next = &l2.dummy;
	l2.n = 0;
	
	this->n += l2_size;
}

template<typename T>
DLList<T> DLList<T>::deal() {
  	DLList<T> list_tmp; 
  
    Node* p = dummy.next->next;
    Node* q;
	Node* tmp = p->next;;

  	for (int i = 1; i < n; i++) {
        // always insert at the end of list
        q = list_tmp.dummy.prev; //list_tmp.getNode(list_tmp.n - 1);

        p->prev->next = p->next;
        p->next->prev = p->prev;

        q->next = p;
        p->prev = q;
        p->next = &(list_tmp.dummy);
        list_tmp.dummy.prev = p;
		
		p = tmp->next;
		tmp = p->next;

        n--;
        list_tmp.n++;
  	}

  	return list_tmp;
}

} /* namespace ods */
#endif /* DLLIST_H_ */
