/*
 * Prediction.cpp
 *
 *  Created on: 1 Jun 2016
 *      Author: osboxes
 */



#include "Prediction.h"
#include <ctime>
#include <limits>


//std::mt19937_64* gen = 0;


Prediction::Prediction( std::vector<Profile> profiles  ) : shift(0),
	score( 0.0 )
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
		double cosCoeff = profiles[cosIndex].getCos(i);
		double sinCoeff = profiles[sinIndex].getSin(i);

		//store them
		builtProfile.addTerm( cosCoeff, sinCoeff );
	}
	values = builtProfile.inverse();
}

Prediction::Prediction(std::vector<double> useTheseValues) : shift(0),
		score( 0.0 ),
		values( useTheseValues )
{

}

Prediction::~Prediction(){
	if (gen != 0 ){
		delete gen;
	}
}




void Prediction::fitToData( const std::vector<double>& trueData ){

	const int INIT_SHIFT = 5;
	const int FINAL_SHIFT = this->size() - 5;
	double bestScore = std::numeric_limits<double>::max();
	int bestShift = INIT_SHIFT;
	double bestYOffset = 0.0;

	for (int currentShift = INIT_SHIFT; currentShift < FINAL_SHIFT; currentShift++){
		//get subvectors
		std::vector<double> predVals = getSubVect(values, 0, currentShift);
		std::vector<double> truVals = getSubVect(trueData, trueData.size()-currentShift, currentShift);

		double yOffset = getYOffset(truVals, predVals);
		predVals = addConstant(predVals, yOffset);

		double currentScore = avgAbsErr(truVals, predVals);

		if(currentScore < bestScore){
			bestScore = currentScore;
			bestShift = currentShift;
			bestYOffset = yOffset;
		}

	}
	score = 1.0 / bestScore;
	shift = bestShift;
	values = addConstant(values, bestYOffset);
}



double getYOffset(const std::vector<double>& x, const std::vector<double>& y){

	double sum = 0.0;
	for (unsigned int i = 0; i < x.size(); i++){
		sum += x[i] - y[i];
	}
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


std::vector<double> Prediction::addConstant (std::vector<double> vect, double constant){
	std::vector<double> returnVect;
	for (unsigned int i = 0; i < vect.size(); i++){
		returnVect.push_back( vect[i] + constant );
	}
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



Prediction Prediction::weightedAvg(const std::vector<Prediction>& allPreds){
	int max = getMaxShift(allPreds);
	int min = getMinShift(allPreds);
	int predSize = allPreds[0].size();
	int blockLength = (max-min) + predSize;

	std::vector< std::vector<double> > allValues = build0Block( allPreds.size(), blockLength );
	std::vector< std::vector<double> > weights = allValues;

	//build matrix of values aligned ready for weighted summing
	for (unsigned int i = 0; i < allPreds.size(); i++){
		std::vector<double> rowOfValues = allPreds[i].getValues();
		int rowShift = max - allPreds[i].getShift();

		for (unsigned int j = 0; j < rowOfValues.size(); j++){
			allValues[i][j + rowShift] = rowOfValues[j];
			weights[i][j + rowShift] = allPreds[i].getScore();
		}

	}

	Prediction returnVal( weightedSum(allValues, weights) );
	return returnVal;
}

int Prediction::getMaxShift( const std::vector<Prediction>& allPreds ){
	int max = allPreds[0].getShift();
	for (unsigned int i = 1; i < allPreds.size(); i++){
		int temp = allPreds[i].getShift();
		if (max > temp) max = temp;
	}
	return max;
}

int Prediction::getMinShift( const std::vector<Prediction>& allPreds ){
	int min = allPreds[0].getShift();
	for (unsigned int i = 1; i < allPreds.size(); i++){
		int temp = allPreds[i].getShift();
		if (min > temp) min = temp;
	}
	return min;
}



std::vector< std::vector<double> > Prediction::build0Block(int rows, int columns){
	std::vector<double> row(columns, 0.0);
	std::vector< std::vector<double> > block(rows, row);

	return block;
}


std::vector<double> Prediction::weightedSum(std::vector< std::vector<double> > allValues, std::vector< std::vector<double> > weights){

	std::vector<double> finalSum;

	for (unsigned int i = 0; i < allValues[0].size(); i++){
		double sum = 0.0;
		double totalWeight = 0.0;

		for (unsigned int j = 0; j < allValues.size(); j++){

			totalWeight += weights[j][i];
			sum += allValues[j][i] * weights[j][i];

		}

		finalSum.push_back( sum / totalWeight );
	}
	return finalSum;
}

