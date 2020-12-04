#include <iostream>
#include <string>
#include "../source/landb.hpp"

int main(){
	
	//our database
	lan::db database;
	
	//declares a class
	database.set_class("food");	

	//declares a string array inside of the "food" class
	std::string fruits[] = {"Mango", "Banana", "Apple"};
	database.set_string_array("food.fruits", fruits, 3);

	// prints the 1st member of our string array
	std::cout << "i love " << database.get_string("food.fruits[0]") + 's' << std::endl;

	//changes the 1st member of our string array
	database.set_string("food.fruits[0]", "Avocado");

        // prints the 1st member of our string array again
        std::cout << "i also love " << database.get_string("food.fruits[0]") + 's' << std::endl;

	// connects the database to the file "example.ldb"
        database.connect("example.ldb");

	// if ou database is connected, push data to it (generate offline variables)
	if(database.is_connected())
		database.push();

}
