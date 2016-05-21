/*
 * MathVector.h
 *
 *  Created on: Feb 22, 2016
 *      Author: Ian
 */

#ifndef MATHVECTOR_H_
#define MATHVECTOR_H_

#include <vector>
#include <math.h>

class MathVector: public std::vector< double > {

public:

	MathVector() : vector<double>() {}
	MathVector(int num, double what) : vector<double>(num, what) {}
	MathVector(const vector< double >& v) : vector<double>(v) {}

	MathVector normalize(){

		double magnitude = this->getMagnitude();

		for (size_t i = 0; i < this->size(); i++){
			this->operator[](i) = this->operator[](i) / magnitude;
		}

		return *this;
	}


	double getMagnitude(){

		double sum = 0.0;

		for (size_t i = 0; i < this->size(); i++){
			sum += this->operator[](i)*this->operator[](i);
		}

		return sqrt(sum);
	}


	MathVector operator +(const MathVector& v) const {

		MathVector retVect;

		for (unsigned int i = 0; ( i < v.size() ) && ( i < this->size() ); i++){
			retVect.push_back( this->operator[](i) + v[i] );
		}
		return retVect;
	}


	MathVector operator *(double c){

		MathVector retVect;

		for (unsigned int i = 0; i < this->size(); i++){
			retVect.push_back( this->operator[](i) * c );
		}
		return retVect;
	}


};



#endif /* MATHVECTOR_H_ */
