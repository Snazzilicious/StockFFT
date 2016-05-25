

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

int main() {

	const unsigned int PROFILE_SIZE = 4;
	const unsigned int NUM_PREDICTIONS = 100;

	//load in data
	std::string inputFile = "input.txt";
	std::vector<double> inData;

	readIn(inData, inputFile);
	reverse( inData );
	double avg = average( inData );

	//subtract average
	for(size_t i=0; i < inData.size(); i++){
		inData[i] -= avg;
		//std::cout << inData[i] << std::endl;
	}


	//DFT 32? points at a time to get 10-20 profiles
	std::vector< std::vector<MathVector> > profiles;

	for (unsigned int i = 0; i + PROFILE_SIZE < inData.size() + 1; i += 1){
		std::vector<double>::const_iterator first = inData.begin() + i;
		std::vector<double>::const_iterator last = inData.begin() + i + PROFILE_SIZE;
		std::vector<double> section(first, last);

		profiles.push_back( DFT::DFT( section ) );
	}

	//randomly select coefficients
	std::uniform_int_distribution<int> dist(0, profiles.size() );
	std::mt19937_64 gen( std::time(NULL) );

	std::vector< std::vector<double> > allPredictions;

	for (unsigned int i = 0; i < NUM_PREDICTIONS; i++){

		std::vector<MathVector> builtProfile;

		for (unsigned int j = 0; j < PROFILE_SIZE; j++){
			int cosIndex = dist( gen );
			int sinIndex = dist( gen );
			//select sin and cosine coefficients
			MathVector cosSin(2,0.0);
			cosSin[0] = profiles[cosIndex][j][0];
			cosSin[0] = profiles[sinIndex][j][1];

			//store them
			builtProfile.push_back( cosSin );
		}
		//stores inverse transform of the constructed
		allPredictions.push_back( DFT::IDFT(builtProfile) );
		builtProfile.clear();
	}

	std::cout << dist( gen ) << std::endl;


	//rebuild curves then average


	//fit to original data


	return 0;
}



void readIn(std::vector<double>& fillWithData, std::string fileName){

	std::ifstream read(fileName, std::ifstream::in);


	while (!read.eof()){
		std::string buff;
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
