#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#define POLY_TYPES	typename dataType,typename floatPrecision  
#define POLY		Polynomial<dataType,floatPrecision>
#define POLY_TMP	template <POLY_TYPES>	


template <typename dataType = double,typename floatPrecision = double>
class Polynomial{
	floatPrecision *_terms;
	unsigned int _degree;
public:
	Polynomial(unsigned int degree , floatPrecision *terms);
	Polynomial(unsigned int degree );

	dataType evaluate(dataType value);

	Polynomial &derivative()const;
	
};



#ifndef _POLYNOMIAL_HPP_
#include "Polynomial.hpp"
#endif

#endif
