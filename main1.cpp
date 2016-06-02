

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <ctime>

#include "DFT.h"
#include "MathVector.h"

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



	std::vector< std::vector<MathVector> > profiles;
	//analyze all data one subvector at a time
	for (unsigned int i = 0; i + PROFILE_SIZE <= inData.size(); i += 1){
		std::vector<double> section = getSubVect(inData, i, PROFILE_SIZE);

		profiles.push_back( DFT::DFT( section ) );
	}


	std::uniform_int_distribution<int> dist(0, profiles.size() );
	std::mt19937_64 gen( std::time(NULL) );

	std::vector< std::vector<double> > allPredictions;

	//builds random profiles to inverse
	for (unsigned int i = 0; i < NUM_PREDICTIONS; i++){

		std::vector<MathVector> builtProfile;

		for (unsigned int j = 0; j < PROFILE_SIZE; j++){
			int cosIndex = dist( gen );
			int sinIndex = dist( gen );
			//select sin and cosine coefficients
			MathVector cosSin(2,0.0);
			cosSin[0] = profiles[cosIndex][j][0];
			cosSin[1] = profiles[sinIndex][j][1];

			//store them
			builtProfile.push_back( cosSin );
		}
		//stores inverse transform of the constructed profile
		allPredictions.push_back( DFT::IDFT(builtProfile) );
		builtProfile.clear();
	}

	std::vector<double> scores;
	std::vector<int> offsets;
	//fit allPredictions to original data
	for (unsigned int i = 0; i < NUM_PREDICTIONS; i++) {
		//select an offset
		double score = 0.0;
		int bestOffset = 5;
		for (unsigned int offset = 5; offset < PROFILE_SIZE - 5; offset++){

			//get subvectors
			std::vector<double>::const_iterator Data1 = inData.end();
			std::vector<double>::const_iterator Data2 = inData.end() - offset;
			std::vector<double> trueVals(Data1, Data2);

			std::vector<double>::const_iterator Pred1 = allPredictions[i].end();
			std::vector<double>::const_iterator Pred2 = allPredictions[i].end() - offset;
			std::vector<double> predVals(Pred1, Pred2);

			//get y offset variable
			double a = yOffset(trueVals, predVals);
			for (int j=0; j<predVals.size(); j++) predVals[j] += a;

			//get absolute average error to compare among offsets
			double tempScore = OffsetScore(trueVals, predVals);
			if (tempScore < score) {
				score = tempScore;
				bestOffset = offset;
			}

		}

		//find best offset value and associate with respective prediction
		scores.push_back( score );
		offsets.push_back( bestOffset );
	}

	//calculate average prediction weighted by inverse of error
	double totalScore = 0.0;
	for (size_t i = 0; i < scores.size(); i++){
		scores[i] = 1.0/scores[i];
		totalScore += scores[i];
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

