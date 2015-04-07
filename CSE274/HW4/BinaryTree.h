/*
 * BinaryTree.h
 *
 *  Created on: 2011-11-28
 *      Author: morin
 */

#ifndef BINARYTREE_H_
#define BINARYTREE_H_
#include <cstdlib>
#include <queue>
#include "ArrayDeque.h"

namespace ods {

template<class N>
class BTNode {
public:
	N *left;
	N *right;
	N *parent;
	BTNode() {
		left = right = parent = NULL;
	}
};


template<class Node>
class BinaryTree {
protected:
	Node *r;    // root node
	Node *nil;  // null-like node
	virtual int size(Node *u);
	virtual int height(Node *u);
        virtual int height2(Node *u);
	virtual void traverse(Node *u);
	bool checkBalance(Node* u);
public:
	virtual ~BinaryTree();
	BinaryTree();
	BinaryTree(Node *nil);
	virtual void clear();
	virtual int depth(Node *u);
	virtual int size();
	virtual int size2();
	virtual int height();
	virtual int height2();
	virtual void traverse();
	virtual void traverse2();
	virtual void bfTraverse();
	
	virtual bool isBalanced();
};

class BTNode1 : public BTNode<BTNode1> { };


template<class Node>
BinaryTree<Node>::~BinaryTree() {
	clear();
}

template<class Node>
void BinaryTree<Node>::clear() {
	Node *u = r, *prev = nil, *next;
	while (u != nil) {
		if (prev == u->parent) {
			if (u->left != nil) next = u->left;
			else if (u->right != nil) next = u->right;
			else next = u->parent;
		} else if (prev == u->left) {
			if (u->right != nil) next = u->right;
			else next = u->parent;
		} else {
			next = u->parent;
		}
		prev = u;
		if (next == u->parent)
			delete u;
		u = next;
	}
	r = nil;
}

template<class Node>
BinaryTree<Node>::BinaryTree(Node *nil) {
	this->nil = nil;
	r = nil;
}

template<class Node>
BinaryTree<Node>::BinaryTree() {
	nil = NULL;
	r = nil;
}


template<class Node>
int BinaryTree<Node>::depth(Node *u) {
	int d = 0;
	while (u != r) {
		u = u->parent;
		d++;
	}
	return d;
}

template<class Node>
int BinaryTree<Node>::size() {
	return size(r);
}

template<class Node>
int BinaryTree<Node>::size(Node *u) {
	if (u == nil) return 0;
	return 1 + size(u->left) + size(u->right);
}

template<class Node>
int BinaryTree<Node>::size2() {
		Node *u = r, *prev = nil, *next;
		int n = 0;
		while (u != nil) {
			if (prev == u->parent) {
				n++;
				if (u->left != nil) next = u->left;
				else if (u->right != nil) next = u->right;
				else next = u->parent;
			} else if (prev == u->left) {
				if (u->right != nil) next = u->right;
				else next = u->parent;
			} else {
				next = u->parent;
			}
			prev = u;
			u = next;
		}
		return n;
	}


template<class Node>
int BinaryTree<Node>::height() {
	return height(r);
}

template<class Node>
int BinaryTree<Node>::height(Node *u) {
	if (u == nil) return -1;
	return 1 + max(height(u->left), height(u->right));
}

template<class Node>
void BinaryTree<Node>::traverse() {
	traverse(r);
}

template<class Node>
void BinaryTree<Node>::traverse(Node *u) {
		if (u == nil) return;
		traverse(u->left);
		traverse(u->right);
}

template<class Node>
void BinaryTree<Node>::traverse2() {
	Node *u = r, *prev = nil, *next;
	while (u != nil) {
		if (prev == u->parent) {
			if (u->left != nil) next = u->left;
			else if (u->right != nil) next = u->right;
			else next = u->parent;
		} else if (prev == u->left) {
			if (u->right != nil) next = u->right;
			else next = u->parent;
		} else {
			next = u->parent;
		}
		prev = u;
		u = next;
	}
}

template<class Node>
void BinaryTree<Node>::bfTraverse() {
	ArrayDeque<Node*> q;
	if (r != nil) q.add(q.size(),r);
	while (q.size() > 0) {
		Node *u = q.remove(q.size()-1);
		if (u->left != nil) q.add(q.size(),u->left);
		if (u->right != nil) q.add(q.size(),u->right);
	}
}

/**** New Functions ****/
template<typename Node>
int BinaryTree<Node>::height2() {
    return height2(r);
}

template<typename Node>
int BinaryTree<Node>::height2(Node* u) {
	if (u == NULL)
	   return -1;

	int height = -1;
	std::queue<Node*> q;
	q.push(u);
	while (q.size() > 0) {
		int size = q.size();
		while (size > 0) {
			if (q.front()->left != nil) {
				q.push(q.front()->left);
			}
			if (q.front()->right != nil) {
				q.push(q.front()->right);
			}
			size--;
			q.pop();
		}
		height++;
	}
	
	return height;
}

template<typename Node>
bool BinaryTree<Node>::checkBalance(Node* u) {
	if (u == nil)
		return true;
	
	int sizeL = size(u->left);
	int sizeR = size(u->right);
	
	if (abs(sizeL - sizeR) <= 1) {
		return checkBalance(u->left) && checkBalance(u->right);
	}
	
	return false;
}

template<typename Node>
bool BinaryTree<Node>::isBalanced() {
	return checkBalance(r);
}

} /* namespace ods */
#endif /* BINARYTREE_H_ */
