/*
 * DFT.h
 *
 *  Created on: 24 May 2016
 *      Author: osboxes
 */

#ifndef DFT_H_
#define DFT_H_

#include"MathVector.h"

namespace DFT{

std::vector<MathVector> DFT(std::vector<double> data);


std::vector<double> IDFT(std::vector<MathVector> coeffs);

}


#endif /* DFT_H_ */
