#include <iostream>
#include <string>
#include "../source/landb.hpp"

int main(){
	lan::db database;
	
	//declaring a string
	database.set_string("name", "landia");
	
	// getting the string
	std::cout << database.get_string("name") << std::endl;
}
