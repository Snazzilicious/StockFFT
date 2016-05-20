

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

void readIn(std::vector<double>& fillWithData, std::string fileName);

int main() {



	//load in data
	std::string inputFile = "input.txt";
	std::vector<double> inData;

	readIn(inData, inputFile);

	int inSize = inData.size();

	for (int i = 0; i < inSize; i++){
		std::cout << inData[i] << std::endl;
	}

	//DFT 32? points at a time to get 10-20 profiles

	//randomly select coeficients

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
