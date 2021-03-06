// CMPE452_Assignment1.cpp : Defines the entry point for the console application.
//Written by: Matt Burton
//			  13mtfb
//Student #	  10107168

#include "stdafx.h"
#include "iris.h"

using namespace std;

void printWeights(vector< vector<float> >, ofstream &);


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
	neuron.push_back(0);//neuron[0]: neuron 1
	neuron.push_back(0);//neuron[1]: neuron 2

	//count the number of train data
	int countDataTrain = 0;
	//count the number of test data
	int countDataTest = 0;

	//declare an input filestream to read data
	ifstream data_train, data_test;
	//Create an output filestream to write data
	ofstream output("output.txt");
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

	//initialize weight vectors to 0.5
	//initialize deltaWeight vectors to 0
	float randomNumber;
	for (int i = 0; i <= 1; i++) {//iterate over each neuron
		vector <float> row;//create an empty row weight
		for (int j = 0; j <= 4; j++) {//iterate over the weight values
			randomNumber = 0.5;
			if (j == 0) { row.push_back(-0.5); } //push back -theta=0.5 for bias weight
			else { row.push_back(randomNumber); }
			if (i == 0) { deltaWeights.push_back(0); }
		}
		weights.push_back(row);
	}

	cout << "Initial Weight values: " << endl;
	output << "Initial Weight values: " << endl;
	printWeights(weights, output);

	bool missed = false;
	//define variable to count number of correct classifications per round
	int correctClassification, missedClassification;


	//define the number of rounds to iterate
	int iterations = 70;
	//define a learning rate
	float learningRate = 0.01;


	for (int a = 1; a <= iterations; a++) {
		correctClassification = 0;
		missedClassification = 0;
		//iterates over data points in set
		for (int i = 0; i < countDataTrain; i++) {
			//reset neuron values
			neuron[0] = 0;
			neuron[1] = 0;
			//calculates sum of input-weight pair for each neuron
			for (int j = 0; j <= 4; j++) {
				//calculate the deltaweight in case of miscategorization
				deltaWeights[j] = input_train[i].data[j] * learningRate;
				//calculate the neuron values
				neuron[0] += weights[0][j] * input_train[i].data[j];
				neuron[1] += weights[1][j] * input_train[i].data[j];
			}
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
			if (input_train[i].type[0] == true) {
				if (neuron[0] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[0][w] = weights[0][w] - deltaWeights[w]; }
					missed = true;
				}
				if (neuron[1] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[1][w] = weights[1][w] - deltaWeights[w]; }
					missed = true;
				}

			}
			//check if versicolour
			else if (input_train[i].type[1] == true) {
				if (neuron[0] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[0][w] = weights[0][w] - deltaWeights[w]; }
					missed = true;
				}
				if (neuron[1] <= 0) {//less than outcome -> add
					for (int w = 0; w <= 4; w++) { weights[1][w] = weights[1][w] + deltaWeights[w]; }
					missed = true;
				}
			}
			//check if virginia
			else if (input_train[i].type[2] == true) {
				if (neuron[0] <= 0) {//less than outcome -> add
					for (int w = 0; w <= 4; w++) { weights[0][w] = weights[0][w] + deltaWeights[w]; }
					missed = true;
				}
				if (neuron[1] > 0) {//greater than outcome -> subtract
					for (int w = 0; w <= 4; w++) { weights[1][w] = weights[1][w] - deltaWeights[w]; }
					missed = true;
				}
			}
			if (missed) {
				missedClassification++;
			}
			else {
				correctClassification++;
			}
		}
		cout << "Round #" << a << ", correct classifications: " << correctClassification << ", missed classifications: " << missedClassification << endl;
	}

	cout << "Final Weight Values: " << endl;
	output << "Final Weight Values: " << endl;
	printWeights(weights, output);

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// TRAINING COMPLETE ///////////////////////////////////////////

	//setosa[0] versicolour[1] virginia[2]
	float TP[3] = { 0, 0, 0 };	//True positives
	float FP[3] = { 0, 0, 0 };	//False positives
	float FN[3] = { 0, 0, 0 };	//True negatives


	cout << "Testing over " << countDataTest << " data points" << endl;
	//iterates over data points in set
	correctClassification = 0;
	missedClassification = 0;

	for (int i = 0; i < countDataTest; i++) {
		if (input_test[i].type[0] == true) {
			cout << "[" << i + 1 << "] " << "classifying a setosa ";
			output << "[" << i + 1 << "] " << "classifying a setosa ";
		}
		else if (input_test[i].type[1] == true) {
			cout << "[" << i + 1 << "] " << "classifying a versicolour ";
			output << "[" << i + 1 << "] " << "classifying a versicolour ";

		}
		else {
			cout << "[" << i + 1 << "] " << "classifying a virginia ";
			output << "[" << i + 1 << "] " << "classifying a virginia ";
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

		//print predicted class 
		if (neuron[0] <= 0 && neuron[1] <= 0) {//predicted a setosa
			cout << "->> Predicted a setosa" << endl;
			output << "->> Predicted a setosa" << endl;
			if (input_test[i].type[0] == true) { TP[0]++; }
			else { FP[0]++; }
		}
		else if (neuron[0] <= 0 && neuron[1] > 0) {//predicted a versicolour
			cout << "->> Predicted a versicolour" << endl;
			output << "->> Predicted a versicolour" << endl;
			if (input_test[i].type[1] == true) { TP[1]++; }
			else { FP[1]++; }
		}
		else if (neuron[0] > 0 && neuron[1] <= 0) {//predicted a virginia
			cout << "->> Predicted a virginia" << endl;
			output << "->> Predicted a virginia" << endl;
			if (input_test[i].type[2] == true) { TP[2]++; }
			else { FP[2]++; }
		}
		else {
			cout << "-> Predicted garbage" << endl;
			output << "-> Predicted garbage" << endl;
		}
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
			if (!(neuron[0] <= 0 && neuron[1] <= 0)) { FN[0]++; }//false negative
			if (neuron[0] > 0) {//greater than outcome -> subtract
				missed = true;
			}
			if (neuron[1] > 0) {//greater than outcome -> subtract
				missed = true;
			}
		}
		//check if versicolour
		else if (input_test[i].type[1] == true) {
			if (!(neuron[0] <= 0 && neuron[1] > 0)) { FN[1]++; }//false negative
			if (neuron[0] > 0) {//greater than outcome -> subtract
				missed = true;
			}
			if (neuron[1] <= 0) {//less than outcome -> add
				missed = true;
			}
		}
		else if (input_test[i].type[2] == true) {
			if (!(neuron[0] > 0 && neuron[1] <= 0)) { FN[2]++; }//false negative
			if (neuron[0] <= 0) {//less than outcome -> add
				missed = true;
			}
			if (neuron[1] > 0) {//greater than outcome -> subtract
				missed = true;
			}
		}
		if (missed) { missedClassification++; /*cout << " -> missed " << "neuron[0]: " << neuron[0] << " neuron[1]: " << neuron[1] << endl;*/ }
		else { correctClassification++; /*cout << " -> correct" << "neuron[0]: " << neuron[0] << " neuron[1]: " << neuron[1] << endl;*/ }

	}
	cout << "number of correct classifications: " << correctClassification << endl;
	cout << "number of incorrect classifications: " << missedClassification << endl;

	cout << "Total classification error: " << missedClassification << endl;
	output << "\nTotal classification error: " << missedClassification << endl;
	output << "Explanation: each missed classification is 1 output value away (no observed instances of a garbage prediction).\nTherefore can just add up the number of missed classifications" << endl;

	output << "[Setosa] Precision: " << (TP[0] / (TP[0] + FP[0] )) << " Recall: " << (TP[0] / (TP[0] + FN[0])) << endl;
	output << "[Versicolour] Precision: " << (TP[1] / (TP[1] + FP[1] )) << " Recall: " << (TP[1] / (TP[1] + FN[1])) << endl;
	output << "[Virginia] Precision: " << (TP[2] / (TP[2] + FP[2] )) << " Recall: " << (TP[2] / (TP[2] + FN[2])) << endl;


	//add parameters to output file
	output << "\n Number of iterations: " << iterations << endl;
	output << "The number of iterations was used as a terminating criteria.\nThis value was chosen empirically as around the minimum number of iterations needed to converge on acceptable weight values.\nA higher number of iterations did not result in any more correct classifications." << endl;

    return 0;
}


void printWeights(vector< vector<float> > Weights, ofstream &output) {
	int dim1 = Weights.size();
	int dim2 = Weights[0].size();//assume square

	for (int i = 0; i < dim1; i++) {
		cout << "<";
		output << "<";
		for (int j = 0; j < dim2; j++) {
			cout << Weights[i][j] << ", ";
			output << Weights[i][j] << ", ";
		}
		cout << ">" << endl;
		output << ">" << endl;
	}
}