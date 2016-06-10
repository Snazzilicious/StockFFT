/*
 * Prediction.cpp
 *
 *  Created on: 1 Jun 2016
 *      Author: osboxes
 */



#include "Prediction.h"
#include <ctime>
#include <limits>

#include <iostream>
#include <exception>

std::mt19937_64* Prediction::gen = 0;
void printVect(std::vector<double> v);

Prediction::Prediction() : shift(0),
		score(1.0)
{

}

Prediction::Prediction( std::vector<Profile> profiles  ) : shift(0),
	score( 1.0 )
{
	//TODO fix this

	if ( gen == 0 ){
		gen = new std::mt19937_64( std::time(NULL) );
	}

	std::uniform_int_distribution<int> dist(0, profiles.size()-1 );

	Profile builtProfile;

	for (unsigned int i = 0; i < profiles[0].size(); i++){

		int cosIndex = dist( *gen );
		int sinIndex = dist( *gen );

		//select sin and cosine coefficients
		double cosCoeff = profiles[cosIndex].getCos(i);
		double sinCoeff = profiles[sinIndex].getSin(i);

		//store them
		builtProfile.addTerm( cosCoeff, sinCoeff );
	}

	values = builtProfile.inverse();
}

Prediction::Prediction(std::vector<double> useTheseValues) : shift(0),
		score( 1.0 ),
		values( useTheseValues )
{

}

Prediction::~Prediction(){

}


Prediction Prediction::operator=(const Prediction& set){
	shift = set.getShift();
	score = set.getScore();
	values = set.getValues();

	return *this;
}


void Prediction::fitToData( const std::vector<double>& trueData ){

	const int INIT_SHIFT = 5;
	const int FINAL_SHIFT = this->size() - 5;
	double bestScore = std::numeric_limits<double>::max();
	int bestShift = INIT_SHIFT;
	double bestYOffset = 0.0;
//std::cout << "Final: " << FINAL_SHIFT << std::endl;
	for (int currentShift = INIT_SHIFT; currentShift < FINAL_SHIFT; currentShift++){
		//std::cout << "i=" << currentShift << std::endl;
		//get subvectors
		std::vector<double> predVals = getSubVect(values, 0, currentShift);
		std::vector<double> truVals = getSubVect(trueData, trueData.size()-currentShift, currentShift);
		//std::cout << "retrieved subVects, pred, tru" << std::endl;
		//printVect(predVals);
		//printVect(truVals);

		double yOffset = getYOffset(truVals, predVals);
		predVals = addConstant(predVals, yOffset);
		//std::cout << "got and added offset: " << yOffset << std::endl;
		//printVect(predVals);

		double currentScore = avgAbsErr(truVals, predVals);
		//std::cout << "Got score: " << currentScore << std::endl;

		if(currentScore < bestScore){
			bestScore = currentScore;
			bestShift = currentShift;
			bestYOffset = yOffset;
		}

	}

	if (bestScore == 0.0){
		score = 999;	//hopefully this never really happens
	}else {
		score = 1.0 / bestScore;
	}
	shift = bestShift;
	values = addConstant(values, bestYOffset);
	//std::cout << "loop ends" << std::endl;
}



double Prediction::getYOffset(const std::vector<double>& x, const std::vector<double>& y){
//std::cout << "y-off start" << std::endl;
	double sum = 0.0;
	for (unsigned int i = 0; i < x.size(); i++){
		sum += x[i] - y[i];
	}
//	std::cout << "y-off stop" << std::endl;
	return sum / x.size();
}


int Prediction::getShift() const {
	return shift;
}

std::vector<double> Prediction::getValues() const {
	return values;
}



double Prediction::getScore() const {
	return score;
}


unsigned int Prediction::size() const {
	return values.size();

}


std::vector<double> Prediction::getSubVect(const std::vector<double>& vect, int start, int length){
	std::vector<double>::const_iterator first = vect.begin() + start;
	std::vector<double>::const_iterator last = vect.begin() + start + length;
	return std::vector<double>(first, last);
}


std::vector<double> Prediction::addConstant (const std::vector<double>& vect, double constant){
	//std::cout << "add start" << std::endl;
	std::vector<double> returnVect;
	//std::cout << "vect size: " << vect.size() << std::endl;
	for (unsigned int i = 0; i < vect.size(); i++){
		double temp = vect[i] + constant;
		//std::cout << i << "th sum: " << temp << std::endl;

		try {
			returnVect.push_back( temp );
		} catch (std::exception& e){
			std::cout << e.what() << std::endl;
		}
	}
//	std::cout << "add stop" << std::endl;
	return returnVect;
}


double Prediction::avgAbsErr(const std::vector<double>& x, const std::vector<double>& y){
	double sum = 0.0;
	unsigned int numPoints = x.size();
	for (unsigned int i = 0; i < numPoints; i++){
		double diff = x[i] - y[i];
		if (diff < 0.0) diff = -diff;
		sum += diff;
	}
	return sum / numPoints;
}



Prediction Prediction::Avg(const std::vector<Prediction>& allPreds){
	std::vector<double> sum(allPreds[0].size(), 0.0);

	for (unsigned int i = 0; i < allPreds.size(); i++){
		std::vector<double> vals = allPreds[i].getValues();

		for (unsigned int j = 0; j < vals.size(); j++){
			sum[j] += vals[j];
		}

	}

	for (unsigned int i = 0; i < sum.size(); i++){
		sum[i] /= allPreds.size();
	}

	return sum;
}




