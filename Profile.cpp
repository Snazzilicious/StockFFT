/*
 * Profile.cpp
 *
 *  Created on: 1 Jun 2016
 *      Author: osboxes
 */

#include "Profile.h"


Profile::Profile(){};

Profile::Profile(const std::vector<double>& data){
	coeffs = DFT::DFT(data);
}

std::vector<double> Profile::inverse(){
	return DFT::IDFT(coeffs);
}

unsigned int Profile::size(){
	return coeffs.size();
}

void Profile::addTerm(double cos, double sin){
	MathVector newTerm(2, 0.0);
	newTerm[0] = cos;
	newTerm[1] = sin;

	coeffs.push_back( newTerm );
}

double Profile::getCos(int index){
	return coeffs[index][0];
}

double Profile::getSin(int index){
	return coeffs[index][1];
}

