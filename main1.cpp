

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>

#include "DFT.h"
#include "MathVector.h"
#include "Profile.h"
#include "Prediction.h"

void readIn(std::vector<double>& fillWithData, std::string fileName);
double average(std::vector<double> data);
void reverse( std::vector<double>& data );
std::vector<double> getSubVect(const std::vector<double>& vect, int start, int length);
void printToFile(std::vector<double> data, Prediction pred, std::string fileName );


int main() {

	const unsigned int PROFILE_SIZE = 30; //must be >= 4
	const unsigned int NUM_PREDICTIONS = 50;


	std::string inputFile = "verizon.txt";
	std::vector<double> inData;
	//load in data and reverse
	readIn(inData, inputFile);
	std::cout << "ALL " << inData.size() << " DATA ACQUIRED" << std::endl;

	reverse( inData );
	double avg = average( inData );
	std::cout << "DATA REVERSED" << std::endl;

	//subtract average
	for(size_t i=0; i < inData.size(); i++){
		inData[i] -= avg;
		//std::cout << inData[i] << std::endl;
	}
	std::cout << "AVERAGE SUBTRACTED" << std::endl;


	std::cout << "STARTING ANALYSIS" << std::endl;
	std::vector< Profile > profiles;
	//get a profile from all subsets of the data
	for (unsigned int i = 0; i + PROFILE_SIZE <= inData.size(); i += 1){
		std::vector<double> section = getSubVect(inData, i, PROFILE_SIZE);
		Profile newProf(section);
		profiles.push_back( newProf );
		std::cout << i << " DONE" << std::endl;
	}
	std::cout << "ANALYSIS DONE" << std::endl;

	std::cout << "BUILDING PREDICTIONS" << std::endl;

	std::vector< Prediction > allPredictions;
	//creates predictions from all of the profiles
	for (unsigned int i = 0; i < NUM_PREDICTIONS; i++){
		Prediction newPred( profiles );
		std::cout << i << " BUILT" << std::endl;
		newPred.fitToData( inData );
		allPredictions.push_back( newPred );
		std::cout << i << " FIT" << std::endl;
	}
	std::cout << "ALL PREDICTIONS BUILT" << std::endl;

	std::cout << "AVERAGING PREDICTIONS" << std::endl;
	//do the averaging of the considering the offsets of each prediction
	Prediction final = Prediction::weightedAvg( allPredictions );
	std::cout << "FITTING FINAL" << std::endl;
	final.fitToData( inData );

	//print then view with soffice
	std::cout << "PRINTING" << std::endl;
	printToFile(inData, final, "PRED_OUT.txt");
	std::cout << "DONE" << std::endl;


	return 0;
}



void readIn(std::vector<double>& fillWithData, std::string fileName){

	std::ifstream read(fileName, std::ifstream::in);

	std::string buff;
	getline(read, buff, '\n');	//chop off header

	while (!read.eof()){

		getline(read, buff, '\n');
		if (buff == "") break;

		double temp;
		std::stringstream(buff) >> temp;

		fillWithData.push_back(temp);
	}

	read.close();
}




double average(std::vector<double> data){

	double sum = 0.0;

	for (unsigned int i = 0; i < data.size(); i++){
		sum += data[i];
	}

	return sum / data.size();
}


void reverse( std::vector<double>& data ){

	int i,j;
	for (i=0,j=data.size()-1; i < j; i++, j--){
		double temp = data[i];
		data[i] = data[j];
		data[j] = temp;
	}

}



std::vector<double> getSubVect(const std::vector<double>& vect, int start, int length){
	std::vector<double>::const_iterator first = vect.begin() + start;
	std::vector<double>::const_iterator last = vect.begin() + start + length;
	return std::vector<double>(first, last);
}




void printToFile(std::vector<double> data, Prediction pred, std::string fileName ){

	std::ofstream write(fileName, std::ifstream::out);
	unsigned int dataB4Pred = data.size()-pred.getShift();
	std::vector<double> predData = pred.getValues();

	for (unsigned int i = 0; i < dataB4Pred; i++){
		write << i << "\t" << data[i] << "\t" << "" << std::endl;
	}

	for (unsigned int i = 0; i < pred.getShift(); i++){
		write << i+dataB4Pred << "\t" << data[i+dataB4Pred] << "\t" << predData[i] << std::endl;
	}

	for (unsigned int i = pred.getShift(); i < predData.size(); i++){
		write << i+dataB4Pred << "\t" << "" << "\t" << predData[i] << std::endl;
	}
}
