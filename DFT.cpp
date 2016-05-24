/*
 * DFT.cpp
 *
 *  Created on: 24 May 2016
 *      Author: osboxes
 */

#include "DFT.h"
#include <cmath>

#define USE_MATH_DEFINES


std::vector<MathVector> DFT::DFT(std::vector<double> data) {

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



std::vector<double> DFT::IDFT(std::vector<MathVector> coeffs) {

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
