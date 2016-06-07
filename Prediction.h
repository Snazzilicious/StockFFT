/*
 * Prediction.h
 *
 *  Created on: 1 Jun 2016
 *      Author: osboxes
 */

#ifndef PREDICTION_H_
#define PREDICTION_H_


#include "Profile.h"
#include <vector>
#include <random>

class Prediction {

private:
	int shift;
	double score;
	std::vector<double> values;

	static std::mt19937_64* gen;

	double getYOffset(const std::vector<double>& x, const std::vector<double>& y);
	std::vector<double> getSubVect(const std::vector<double>& vect, int start, int length);
	std::vector<double> addConstant (const std::vector<double>& vect, double constant);
	double avgAbsErr(const std::vector<double>& x, const std::vector<double>& y);

	static int getMaxShift( const std::vector<Prediction>& allPreds );
	static int getMinShift( const std::vector<Prediction>& allPreds );

	static std::vector< std::vector<double> > build0Block(int rows, int columns);
	static std::vector<double> weightedSum(std::vector< std::vector<double> > allValues, std::vector< std::vector<double> > weights);

public:
	Prediction();
	Prediction( std::vector<Profile> profiles );
	Prediction(std::vector<double> useTheseValues);
	~Prediction();

	void fitToData( const std::vector<double>& trueData );

	double getScore() const ;
	int getShift() const ;
	std::vector<double> getValues() const ;

	unsigned int size() const ;

	static Prediction weightedAvg(const std::vector<Prediction>& allPreds);
	Prediction operator=(const Prediction& set);

};




#endif /* PREDICTION_H_ */
