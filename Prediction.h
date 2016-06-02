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


class Prediction {

private:
	std::vector<double> values;
	int offset;
	double score;

	static std::mt19937_64* gen;

public:
	Prediction( std::vector<Profile> profiles );
	~Prediction();

	void findBestOffset( const std::vector<double> trueData );

	double getScore();
	int getOffset();
	std::vector<double> getValues();



};




#endif /* PREDICTION_H_ */
