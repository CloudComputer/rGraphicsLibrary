#ifndef _POLYNOMIAL_HPP_
#define _POLYNOMIAL_HPP_

#ifndef _POLYNOMIAL_H_
#include "Polynomial.h"
#endif


POLY_TMP POLY::Polynomial(unsigned int degree ,floatPrecision *terms){
	_terms = new floatPrecision[degree+1];
	_degree = degree;
	for(int i = 0;i<=_degree;i++){
		_terms[i] = terms[i];
	}
}

POLY_TMP POLY::Polynomial(unsigned int degree ){
	_terms = new floatPrecision[degree+1];
	_degree = degree;
	for(int i = 0;i<=_degree;i++){
		_terms[i] = 0;
	}
}





POLY_TMP dataType POLY::evaluate(dataType value){
	dataType v(0);
	for(int i = 0;i<=_degree;i++){
		v += std::pow(value,(floatPrecision)(_degree-i)) * _terms[i];
	}

	return v;
}

POLY_TMP POLY& POLY::derivative()const;


#endif
