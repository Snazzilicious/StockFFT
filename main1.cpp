

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


int main() {



	//load in data
	std::string inputFile = "input.txt";
	std::vector<double> inData;

	readIn(inData, inputFile);





	//DFT 32? points at a time to get 10-20 profiles
	std::vector<MathVector> DFTData = DFT::DFT( inData );


	//randomly select coefficients
	std::uniform_int_distribution<int> dist(0,45);
	std::mt19937_64 gen( std::time(NULL) );


	std::cout << dist( gen ) << std::endl;


	//rebuild curves then average
	std::vector<double> inverse = DFT::IDFT(DFTData);

	for (unsigned int i = 0; i < inverse.size(); i++){
		std::cout << inverse[i] << std::endl;
	}

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
