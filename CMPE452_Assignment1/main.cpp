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

	//using binary output for neurons, so 2 -> 4 mapping 
	vector <float> neuron;
	neuron.push_back(0);//neuron[0]: hidden neuron 1
	neuron.push_back(0);//neuron[1]: hidden neuron 2

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

	printIris(input_test[5]);

	//initialize weight vectors to random value between 1-5
	//initialize deltaWeight vectors to 0
	float randomNumber;
	for (int i = 0; i <= 1; i++) {
		vector <float> row;//create an empty row weight
		for (int j = 0; j<= 4; j++){
			randomNumber = ((double)rand() / (RAND_MAX));
			//randomNumber = (rand() % 10) - 5;
			//randomNumber = 0;
			if (j == 0) { row.push_back(-0.5); } //push back -theta for bias weight
			else { row.push_back(randomNumber); }
			if (i == 0) { deltaWeights.push_back(0); }
		}
		weights.push_back(row);
	}

	printWeights(weights);

	bool missed = false;
	//define variable to count number of correct classifications per round
	int correctClassification, missedClassification;


	//define the number of rounds to iterate
	int iterations = 70;
	//define a learning rate
	float learningRate = 0.01;


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
			else {
				//cout << "classifying a virginia" << endl; 
			}
			//reset neuron values
			neuron[0] = 0;
			neuron[1] = 0;
			//calculates sum of input-weight pair for each neuron
			for (int j = 0; j <=4; j++) {
				//calculate the deltaweight in case of miscategorization
				deltaWeights[j] = input_train[i].data[j] * learningRate;
				//calculate the neuron values
				neuron[0] += weights[0][j] * input_train[i].data[j];
				neuron[1] += weights[1][j] * input_train[i].data[j];
			}

			//cout << "neuron " << j << ", output is: " << neuron[j] << endl;
			//if neuron > 0, then output is 1
			//		    <=0, then output is 0
			//Classifying the flowers is a binary combination of the two neurons
			//
			//Neuron[0]==0 & Neuron[1]==0: setosa
			//Neuron[0]==0 & Neuron[1]==1: versicolour
			//Neuron[0]==1 & Neuron[1]==0: virginia
			//Neuron[0]==1 & Neuron[1]==1: Nothing
			//1: Y = 1 and D = 0 -> therefore must subtract
			//2: Y = 0 and D = 1 -> therefore must add
			//3: otherwise neuron correctly classified
			missed = false;
			//cout << "Neuron[0]= " << neuron[0] << ", Neuron[1]=" << neuron[1];
			//check is setosa?
			if (input_train[i].type[0] == true) {
				if (neuron[0] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[0][w] = weights[0][w] - deltaWeights[w];}
					missed = true;
					//subtract
					//cout << " [0]: Subtract";
				}
				if (neuron[1] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[1][w] = weights[1][w] - deltaWeights[w];}
					missed = true;
					//subtract
					//cout << " [1]: Subtract";
				}

			}
			//check if versicolour
			else if (input_train[i].type[1] == true) {
				if (neuron[0] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[0][w] = weights[0][w] - deltaWeights[w];}
					missed = true;
					//subtract
					//cout << " [0]: Subtract";
				}
				if (neuron[1] <= 0) {//less than outcome -> add
					for (int w = 0; w <= 4; w++) { weights[1][w] = weights[1][w] + deltaWeights[w];}
					missed = true;
					//add
					//cout << " [1]: Add";

				}
			}
			//virgincia
			else if (input_train[i].type[2] == true) {
				if (neuron[0] <= 0) {//less than outcome -> add
					for (int w = 0; w <= 4; w++) { weights[0][w] = weights[0][w] + deltaWeights[w];}
					missed = true;
					//cout << " [0]: Add";
					//add
				}
				if (neuron[1] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[1][w] = weights[1][w] - deltaWeights[w];}
					missed = true;
					//cout << " [1]: Subtract";
					//subtract
				}
			}
			if (missed) { 
				missedClassification++; 
			}
			else { 
				correctClassification++;  
			}
			//cout << endl;
		}
		cout << "Round #" << a << ", correct classifications: " << correctClassification << ", missed classifications: " << missedClassification << endl;
	}
	
	cout << "latest weights: " << endl;
	printWeights(weights);

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// TRAINING COMPLETE ///////////////////////////////////////////



	cout << "Testing over " << countDataTest << " data points" << endl;
	//iterates over data points in set
		correctClassification = 0;
		missedClassification = 0;

	for (int i = 0; i < countDataTest; i++) {
		if (input_test[i].type[0] == true) {
			cout << "classifying a setosa ";
		}
		else if (input_test[i].type[1] == true) {
			cout << "classifying a versicolour ";
		}
		else { 
			cout << "classifying a virginia ";
		}
		//reset neuron values
		neuron[0] = 0;
		neuron[1] = 0;
		//calculates sum of input-weight pair for each neuron
		for (int j = 0; j <= 4; j++) {
			//calculate the neuron values
			neuron[0] += weights[0][j] * input_test[i].data[j];
			neuron[1] += weights[1][j] * input_test[i].data[j];
		}

		//cout << "neuron " << j << ", output is: " << neuron[j] << endl;
		//if neuron > 0, then output is 1
		//		    <=0, then output is 0
		//Classifying the flowers is a binary combination of the two neurons
		//
		//Neuron[0]==0 & Neuron[1]==0: setosa
		//Neuron[0]==0 & Neuron[1]==1: versicolour
		//Neuron[0]==1 & Neuron[1]==0: virginia
		//Neuron[0]==1 & Neuron[1]==1: Nothing
		//1: Y = 1 and D = 0 -> therefore must subtract
		//2: Y = 0 and D = 1 -> therefore must add
		//3: otherwise neuron correctly classified
		missed = false;
		//check is setosa?
		if (input_test[i].type[0] == true) {
			if (neuron[0] > 0) {//greater than outcome -> subtract
				missed = true;
			}
			if (neuron[1] > 0) {//greater than outcome -> subtract
				missed = true;
			}
		}
		//check if versicolour
		else if (input_test[i].type[1] == true) {
			if (neuron[0] > 0) {//greater than outcome -> subtract
				missed = true;
			}
			if (neuron[1] <= 0) {//less than outcome -> add
				missed = true;
			}
		}
		else if (input_test[i].type[2] == true) {
			if (neuron[0] <= 0) {//less than outcome -> add
				missed = true;
			}
			if (neuron[1] > 0) {//greater than outcome -> subtract
				missed = true;
			}
		}
		if (missed) { missedClassification++; cout << " -> missed " << "neuron[0]: " << neuron[0] << " neuron[1]: " << neuron[1] << endl; }
		else { correctClassification++; cout << " -> correct" << "neuron[0]: " << neuron[0] << " neuron[1]: " << neuron[1] << endl;
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