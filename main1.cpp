

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
double yOffset(std::vector<double> x, std::vector<double> y);
double OffsetScore(std::vector<double> x, std::vector<double> y);
std::vector<double> getSubVect(const std::vector<double>& vect, int start, int length);

int main() {

	const unsigned int PROFILE_SIZE = 4;
	const unsigned int NUM_PREDICTIONS = 100;


	std::string inputFile = "input.txt";
	std::vector<double> inData;
	//load in data and reverse
	readIn(inData, inputFile);
	reverse( inData );
	double avg = average( inData );

	//subtract average
	for(size_t i=0; i < inData.size(); i++){
		inData[i] -= avg;
		//std::cout << inData[i] << std::endl;
	}



	std::vector< Profile > profiles;
	//get a profile from all subsets of the data
	for (unsigned int i = 0; i + PROFILE_SIZE <= inData.size(); i += 1){
		std::vector<double> section = getSubVect(inData, i, PROFILE_SIZE);
		Profile newProf(section);
		profiles.push_back( newProf );
	}



	std::vector< Prediction > allPredictions;
	//creates predictions from all of the profiles
	for (unsigned int i = 0; i < NUM_PREDICTIONS; i++){
		Prediction newPred( profiles );

		allPredictions.push_back( newPred );
	}


	//do the averaging of the considering the offsets of each prediction

	//print with gnuplot somehow

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


double yOffset(std::vector<double> x, std::vector<double> y){

	double sum = 0.0;
	for (unsigned int i = 0; i < x.size(); i++){
		sum += x[i] - y[i];
	}
	return sum / x.size();
}

double OffsetScore(std::vector<double> x, std::vector<double> y){

	double sum = 0.0;
	for (unsigned int i = 0; i < x.size(); i++){
		double diff = x[i] - y[i];
		if (diff < 0.0) diff = -diff;	//abs value
		sum += diff;
	}
	return sum / x.size();
}

