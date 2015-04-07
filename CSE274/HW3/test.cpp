#include "ChainedHashTable.h"
#include "LinearHashTable.h"
#include <iostream>

const int N = 1000;

int main() {
	ods::ChainedHashTable<int> hash_table;

	std::cout << "Chained Hash Table Test: " << std::endl;
	for (unsigned int i = 0; i < N; i++) {
	    hash_table.add(rand());
	}	

	std::cout << "longest list is: " << hash_table.GetLongestList() 
		  << std::endl << std::endl;
	
	
	std::cout << "Linear Hash Table Test: " << std::endl;
	ods::LinearHashTable<int> linear_hash_table(0, -1);
	std::cout << "insert 10" << std::endl;	
	std::cout << "insert 3" << std::endl;
	std::cout << "insert 104" << std::endl;
	std::cout << "insert 101" << std::endl;
	std::cout << "insert 29" << std::endl;
	std::cout << "insert 77" << std::endl;
	std::cout << "insert 83" << std::endl;

	linear_hash_table.add(10);
	linear_hash_table.add(3);
	linear_hash_table.add(104);
	linear_hash_table.add(101);
	linear_hash_table.add(29);
	linear_hash_table.add(77);
	linear_hash_table.add(83);

	std::cout << "find 10: " << linear_hash_table.find(10) << std::endl;
	std::cout << "find 3: " << linear_hash_table.find(3) << std::endl;
	std::cout << "find 101: " << linear_hash_table.find(101) << std::endl;
	std::cout << "find 104: " << linear_hash_table.find(104) << std::endl;
	std::cout << "find 29: " << linear_hash_table.find(29) << std::endl;
	std::cout << "find 77: " << linear_hash_table.find(77) << std::endl;
	std::cout << "find 83: " << linear_hash_table.find(83) << std::endl;
	std::cout << "find 5: " << linear_hash_table.find(5) << std::endl;
	

	return 0;
}
