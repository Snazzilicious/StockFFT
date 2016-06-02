/*
 * Prediction.cpp
 *
 *  Created on: 1 Jun 2016
 *      Author: osboxes
 */



#include "Prediction.h"
#include <ctime>

Prediction::Prediction( std::vector<Profile> profiles  ) : offset(0),
	score(0.0)
{

	if ( gen == 0 ){
		gen = new std::mt19937_64( std::time(NULL) );
	}

	std::uniform_int_distribution<int> dist(0, profiles.size() );

	Profile builtProfile;

	for (unsigned int i = 0; i < profiles[0].size(); i++){

		int cosIndex = dist( gen );
		int sinIndex = dist( gen );

		//select sin and cosine coefficients
		double cosCoeff = profiles[cosIndex][i][0];
		double sinCoeff = profiles[sinIndex][i][1];

		//store them
		builtProfile.addTerm( cosCoeff, sinCoeff );
	}
	values = builtProfile.inverse();
}



Prediction::~Prediction(){
	if (gen != 0 ){
		delete gen;
	}
}



double Prediction::getScore(){
	return score;
}

int Prediction::getOffset(){
	return offset;
}

std::vector<double> Prediction::getValues(){
	return values;
}
