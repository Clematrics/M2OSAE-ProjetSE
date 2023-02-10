//============================================================================
// Name        : ProjetSE.cpp
// Author      : Clément Devatine & Hadrien Caumont
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "test.hpp"

int main() {
	test("start", 0);
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	test("end", 2);
	return 0;
}
