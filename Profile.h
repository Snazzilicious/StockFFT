/*
 * Profile.h
 *
 *  Created on: 1 Jun 2016
 *      Author: osboxes
 */

#ifndef PROFILE_H_
#define PROFILE_H_


#include "DFT.h"
#include "MathVector.h"


class Profile {

private:
	std::vector<MathVector> coeffs;

public:
	Profile();
	Profile(std::vector<double> data);

	std::vector<double> inverse();

	unsigned int size();

	void addTerm(double cos, double sin);

};


#endif /* PROFILE_H_ */
