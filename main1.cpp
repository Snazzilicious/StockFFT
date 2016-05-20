

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <ctime>

void readIn(std::vector<double>& fillWithData, std::string fileName);

int main() {



	//load in data
	std::string inputFile = "input.txt";
	std::vector<double> inData;

	readIn(inData, inputFile);

	int inSize = inData.size();


	//DFT 32? points at a time to get 10-20 profiles

	//randomly select coeficients
	std::uniform_int_distribution<int> dist(0,45);
	std::mt19937_64 gen( std::time(NULL) );


	std::cout << dist( gen );
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
