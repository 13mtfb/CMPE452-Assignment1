#include "stdafx.h"

//contains structs and functions for iris data

//float data array[5] has following indices
//data[0]: bias input
//data[1]: petalWidth
//data[2]: petalLength
//data[3]: sepalWidth
//data[4]: sepalLength
//bool type array[3] has following indices
//type[0]: true -> setosa
//type[1]: true -> versicolor
//type[3]: true -> viginicia
struct iris {
	float data[5];
	bool  type[3];
};

//function declarations
void printIris(iris);
iris parseInput(std::string);
