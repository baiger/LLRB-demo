#include "llrb.h"
#include <iostream>

int main(int argc, char *argv[])
{
     LLRB tree;
     std::string command;
     
     std::cin >> command;
     while (command != "exit") {
	  if (command == "insert") {
	       int key;
	       std::cin >> key;
	       tree.insert(key);
	  } else if (command == "print") {
	       std::cout << tree;
	  }
	  std::cin >> command;
     }
     
     return 0;
}
