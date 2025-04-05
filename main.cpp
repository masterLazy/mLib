#include <iostream>
using namespace std;

#include "console/console.hpp"
using namespace mlib::console;

int main() {
	ProgressBar pb(10000000000,102);
	pb.print();
	return 0;
}