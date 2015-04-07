#include "DLList.h"
#include <iostream>

using namespace ods;

int main() {
    // test
    std::cout << "test IsPalinddrome: (original list 1 2 3 1)" << std::endl; 
    DLList<int> l1;
    l1.add(l1.size(), 1);
    l1.add(l1.size(), 2);
    l1.add(l1.size(), 3);
    l1.add(l1.size(), 1);

    std::cout << "Is Palinddrome? " << l1.IsPalinddrome() << std::endl << std::endl;

    /********************************/
	// test Rotate
    std::cout << "test rotate (Rotate 3): (original list a b c d e f)" << std::endl;
	DLList<char> list;
	
	list.add(list.size(), 'a');
	list.add(list.size(), 'b');
	list.add(list.size(), 'c');
	list.add(list.size(), 'd');
	list.add(list.size(), 'e');
	list.add(list.size(), 'f');
	
	for(int i = 0; i < list.size(); i++) {
		std::cout << list.get(i) << " ";
	}
	std::cout << std::endl;

	list.Rotate(3);
	for (int i = 0; i < list.size(); i++) {
	    std::cout << list.get(i) << " ";
	}
	std::cout << std::endl << std::endl;
	
	// test Absorb
    std::cout << "test absorb: " << std::endl;
	DLList<char> list_tmp;
	list_tmp.add(list_tmp.size(), 'g');
	list_tmp.add(list_tmp.size(), 'h');
	list_tmp.add(list_tmp.size(), 'i');
	list_tmp.add(list_tmp.size(), 'j');
	
	list.Absorb(list_tmp);
	for(int i = 0; i < list.size(); i++) {
		std::cout << list.get(i) << " ";
	}
	std::cout << std::endl;
	
	for(int i = 0; i < list_tmp.size(); i++) {
		std::cout << list_tmp.get(i) << " ";
	}
	std::cout << std::endl << std::endl;
	
	// test deal
    std::cout << "test deal: " << std::endl;
	DLList<char> tmp = list.deal();
	for(int i = 0; i < list.size(); i++) {
		std::cout << list.get(i) << " ";
	}
	std::cout << std::endl;
	
	for(int i = 0; i < tmp.size(); i++) {
		std::cout << tmp.get(i) << " ";
	}
	std::cout << std::endl;
	
	return 0;
}
