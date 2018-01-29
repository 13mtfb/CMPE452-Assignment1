#include "stdafx.h"
#include "iris.h"

void printIris(iris print) {
	if (print.type[0] == true) {
		std::cout << "Iris-setosa: ";
	}
	else if (print.type[1] == true) {
		std::cout << "Iris-versicolor: ";
	}
	else if (print.type[2] == true) {
		std::cout << "Iris-virginica: ";
	}
	std::cout << print.data[1] << ", " << print.data[2] << ", " << print.data[3] << ", " << print.data[4] << std::endl;
}


//assumes four float values separated by a single comma:
// petalWidth, petalLength, sepalWidth, sepalLength
//followed by a string indicating the type of plant
iris parseInput(std::string line) {

	//iris return
	iris input;
	//set input bias in vector
	input.data[0] = 1;
	//default type array all false
	input.type[0] = false;
	input.type[1] = false;
	input.type[2] = false;

	//substring to parse csv
	std::string subString;
	//values for string find
	int firstPos = 0;
	int subPos;
	float value;

	//parse float values
	for (int i = 0; i < 5; i++) {
		subPos = line.find(",", firstPos);
		subString = line.substr(firstPos, subPos);
		firstPos = subPos + 1;

		//don't parse string value
		if (i < 4) { value = atof(subString.c_str()); }

		if (i == 0) { input.data[1] = value; }
		if (i == 1) { input.data[2] = value; }
		if (i == 2) { input.data[3] = value; }
		if (i == 3) { input.data[4] = value; }
		if (i == 4) {
			//match the string to the enum type
			if (subString == "Iris-setosa")
				input.type[0] = true;
			else if (subString == "Iris-versicolor")
				input.type[1] = true;
			else if (subString == "Iris-virginica")
				input.type[2] = true;
		}
	}
	return input;
}