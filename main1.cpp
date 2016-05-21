

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <ctime>

#include "MathVector.h"

void readIn(std::vector<double>& fillWithData, std::string fileName);
std::vector<MathVector> DFT(std::vector<double> data);
std::vector<double> IDFT(std::vector<MathVector> coeffs);


int main() {



	//load in data
	std::string inputFile = "input.txt";
	std::vector<double> inData;

	readIn(inData, inputFile);

	for (unsigned int i = 0; i < inData.size(); i++){
		std::cout << inData[i] << std::endl;
	}




	//DFT 32? points at a time to get 10-20 profiles
	std::vector<MathVector> DFTData = DFT( inData );


	for (unsigned int i = 0; i < DFTData.size(); i++){
		std::cout << DFTData[i][0] << " , " << DFTData[i][1] << std::endl;
	}

	//randomly select coefficients
	std::uniform_int_distribution<int> dist(0,45);
	std::mt19937_64 gen( std::time(NULL) );


	std::cout << dist( gen ) << std::endl;


	//rebuild curves then average
	std::vector<double> inverse = IDFT(DFTData);

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


std::vector<MathVector> DFT(std::vector<double> data) {

	int N = data.size();

	std::vector<MathVector> DFT_result;

	for (int n=0; n<N; n++) {
		MathVector sum(2, 0.0);

		for (int m=0; m<N; m++){

			MathVector complex(2, 0.0);
			complex[0] = std::cos( -(2*M_PI*m*n)/N ) ;
			complex[1] = std::sin( -(2*M_PI*m*n)/N ) ;

			//std::cout << complex[0] << " , " << complex[1] << std::endl;
			//std::cout << ( complex * inData[m] )[1] << std::endl;
			sum = sum + ( complex * data[m] );
			//std::cout << sum[0] << " , " << sum[1] << std::endl;
			complex.clear();
		}

		DFT_result.push_back(sum);
		sum.clear();
	}

	return DFT_result;
}



std::vector<double> IDFT(std::vector<MathVector> coeffs) {

	int N = coeffs.size();

	std::vector<double> IDFT_result;

	for (int n=0; n<N; n++) {
		double sum = 0.0;

		for (int m=0; m<N; m++){

			sum += coeffs[m][0] * std::cos( (2*M_PI*m*n)/N ) ;
			sum += - coeffs[m][1] * std::sin( (2*M_PI*m*n)/N ) ;

			//std::cout << complex[0] << " , " << complex[1] << std::endl;
			//std::cout << ( complex * inData[m] )[1] << std::endl;
			//std::cout << sum[0] << " , " << sum[1] << std::endl;
		}

		IDFT_result.push_back(sum);
	}

	for (unsigned int i = 0; i < IDFT_result.size(); i++){
		IDFT_result[i] *= (1.0 / N);
	}

	return IDFT_result;
}
