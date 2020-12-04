#include <iostream>
#include <string>
#include "../source/landb.hpp"

int main(){
	lan::db database;
	
	if(database.connect("example.ldb"))
		std::cout << "sucessfull connected" << std::endl;	
	else 
		std::cout << "failed to connect" << std::endl;
	
	//declaring a string
	database.set_string("name", "landia");
	
	// getting the string
	std::cout << database.get_string("name") << std::endl;

	database.push();
}
