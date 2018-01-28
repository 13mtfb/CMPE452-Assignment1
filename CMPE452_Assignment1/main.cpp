// CMPE452_Assignment1.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "iris.h"

using namespace std;

void printWeights(vector< vector<float> >);


int main()
{
	//create a vector of iris structs for training
	vector <iris> input_train;

	//create a vector of iris structs for testing
	vector <iris> input_test;

	//create a vector of weight and deltaWeight values
	vector < vector<float> > weights;
	vector <float> deltaWeights;

	//create a vector of neuron values (equal to number of outputs => 3 in this case)
	vector <float> neuron;
	neuron.push_back(0);//neuron[0]: output neuron for setosa
	neuron.push_back(0);//neuron[1]: output neuron for versicolour
	neuron.push_back(0);//neuron[2]: output neuron for virginia
	//define a learning rate
	float learningRate = 0.01;

	//count the number of train data
	int countDataTrain = 0;
	//count the number of test data
	int countDataTest = 0;

	//declare an input filestream to read data
	ifstream data_train, data_test;
	//open the file stream
	data_train.open("Train.txt");
	data_test.open("Test.txt");
	//check for errors opening file
	if (!data_train || !data_test) {
		cerr << "Unable to open file" << endl;
		exit(1);
	}
	//iterate through the train file, line-by-line
	for (string line; getline(data_train, line); ) {
		//create iris struct to push to vector
		iris addData;
		//increment count
		countDataTrain++;
		//parse string line
		addData = parseInput(line);	
		//print the struct data before saving
		//printIris(addData);
		//push the struct to the vector
		input_train.push_back(addData);
	}
	//iterate through the test file, line-by-line
	for (string line; getline(data_test, line); ) {
		//create iris struct to push to vector
		iris addData;
		//increment count
		countDataTest++;
		//parse string line
		addData = parseInput(line);
		//print the struct data before saving
		//printIris(addData);
		//push the struct to the vector
		input_test.push_back(addData);
	}
	//close the file stream
	data_train.close();
	data_test.close();
	cout << "Number of training datapoints: " << countDataTrain << endl;

	//initialize weight vectors to random value between 1-5
	//initialize deltaWeight vectors to 0
	float randomNumber;
	for (int i = 0; i <= 2; i++) {
		vector <float> row;//create an empty row weight
		for (int j = 0; j<= 4; j++){
			randomNumber = ((double)rand() / (RAND_MAX));
			if (j == 0) { row.push_back(-0.5); } //push back -theta for bias weight
			else {
			row.push_back(randomNumber);
			}
			if (i == 0) { deltaWeights.push_back(0); }
		}
		weights.push_back(row);
	}

	printWeights(weights);

	
	//define variable to count number of correct classifications per round
	int correctClassification, missedClassification;
	//define the number of rounds to iterate
	int iterations = 500;
	for (int a = 1; a <= iterations; a++){
		correctClassification = 0;
		missedClassification = 0;
		//iterates over data points in set
		for (int i = 0; i < countDataTrain; i++){
			if (input_train[i].type[0] == true) {
				//cout << "classifying a setosa" << endl;
			}
			else if (input_train[i].type[1] == true) {
				//cout << "classifying a versicolour" << endl;
			}
			else { //cout << "classifying a virginia" << endl; 
			}
			//reset neuron values
			neuron[0] = 0;
			neuron[1] = 0;
			neuron[2] = 0;
			//calculates sum of input-weight pair for each neuron
			for (int j = 0; j <=4; j++) {
				//calculate the deltaweight in case of miscategorization
				deltaWeights[j] = input_train[i].data[j] * learningRate;
				//calculate the neuron values
				neuron[0] += weights[0][j] * input_train[i].data[j];
				neuron[1] += weights[1][j] * input_train[i].data[j];
				neuron[2] += weights[2][j] * input_train[i].data[j];
			}

			for (int j = 0; j <3; j++){
				//cout << "neuron " << j << ", output is: " << neuron[j] << endl;
				//if neuron > 0, then output is 1
				//		    <=0, then output is 0
				//Then, three possible outcomes:
				//1: Y = 1 and D = 0 -> therefore must subtract
				//2: Y = 0 and D = 1 -> therefore must add
				//3: otherwise neuron correctly classified
				if (neuron[j] > 0 && input_train[i].type[j]==false) {//outcome 1
					for (int w = 0; w <= 4; w++) { weights[j][w] = weights[j][w] - deltaWeights[w]; deltaWeights[w] = 0; }
					missedClassification++;
					//subtract
				}
				else if (neuron[j] < 0 && input_train[i].type[j]==true) {//outcome 2
					for (int w = 0; w <= 4; w++) { weights[j][w] = weights[j][w] + deltaWeights[w]; deltaWeights[w] = 0; }
					missedClassification++;
					//add
				}
				else { correctClassification++; } //outcome 3
			}

		}
		cout << "Round #" << a << ", correct classifications: " << correctClassification << ", missed classifications: " << missedClassification << endl;
	}
	
	printWeights(weights);
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// TRAINING COMPLETE ///////////////////////////////////////////

	correctClassification = 0;
	missedClassification = 0;

	cout << "Testing over " << countDataTest << " data points" << endl;
	//iterates over data points in set
	for (int i = 0; i < countDataTest; i++) {
		if (input_test[i].type[0] == true) {
			//cout << "classifying a setosa" << endl;
		}
		else if (input_test[i].type[1] == true) {
			//cout << "classifying a versicolour" << endl;
		}
		else { //cout << "classifying a virginia" << endl; 
		}
		//reset neuron values
		neuron[0] = 0;
		neuron[1] = 0;
		neuron[2] = 0;
		//calculates sum of input-weight pair for each neuron
		for (int j = 0; j <= 4; j++) {
			//calculate the neuron values
			neuron[0] += weights[0][j] * input_test[i].data[j];
			neuron[1] += weights[1][j] * input_test[i].data[j];
			neuron[2] += weights[2][j] * input_test[i].data[j];
		}

		for (int j = 0; j <3; j++) {
			//cout << "neuron " << j << ", output is: " << neuron[j] << endl;
			//if neuron > 0, then output is 1
			//		    <=0, then output is 0
			//Then, three possible outcomes:
			//1: Y = 1 and D = 0 -> therefore must subtract
			//2: Y = 0 and D = 1 -> therefore must add
			//3: otherwise neuron correctly classified
			if (neuron[j] > 0 && input_train[i].type[j] == false) {//outcome 1
				missedClassification++;
				//subtract
			}
			else if (neuron[j] < 0 && input_train[i].type[j] == true) {//outcome 2
				missedClassification++;
				//add
			}
			else { correctClassification++; } //outcome 3
		}

	}

	cout << "number of correct classifications: " << correctClassification << endl;
	cout << "number of incorrect classifications: " << missedClassification << endl;


    return 0;
}


void printWeights(vector< vector<float> > Weights) {
	int dim1 = Weights.size();
	int dim2 = Weights[0].size();//assume square

	for (int i = 0; i < dim1; i++) {
		cout << "<";
		for (int j = 0; j < dim2; j++) {
			cout << Weights[i][j] << ", ";
		}
		cout << ">" << endl;
	}
}