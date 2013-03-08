#ifndef _RBF_H_
#define _RBF_H_


#include "Math\Polynomial.h"
#include "Geometry\SurfacePoint.h"
#include "Geometry\KDTree.h"

#include "Math\MatrixInversion.h"

#include "UMFPACK\Include\umfpack.h"

#include <vector>

template <typename floatPrecision = double> class RBF{
	floatPrecision _weight;
	floatPrecision _center[3];
	virtual floatPrecision _eval(floatPrecision r2) const = 0;
public:
	RBF(floatPrecision cx,floatPrecision cy,floatPrecision cz,floatPrecision weight = 1):_weight(weight){
		_center[0] = cx;
		_center[1] = cy;
		_center[2] = cz;
	}
	floatPrecision eval(floatPrecision x,floatPrecision y,floatPrecision z) const{
		float r = 0,d;
		d = _center[0] - x;
		r += d*d;
		d = _center[1] - y;
		r += d*d;
		d = _center[2] - z;
		r += d*d;
		return _eval(r) * _weight;
	}
	
};

template <typename floatPrecision = double>
class ThinPlateSplineRBF : public RBF<floatPrecision>{
	virtual floatPrecision _eval(floatPrecision r2) const{
		return r*std::log(std::sqrt(r2));
	}
public:
	ThinPlateSplineRBF(floatPrecision cx,floatPrecision cy,floatPrecision cz,floatPrecision weight = 1):RBF(cx,cy,cz,weight){}
};

template <typename floatPrecision = double>
class Biharmonic : public RBF<floatPrecision>{
	virtual floatPrecision _eval(floatPrecision r2) const{
		return std::sqrt(r2);
	}
public:
	Biharmonic(floatPrecision cx,floatPrecision cy,floatPrecision cz,floatPrecision weight = 1):RBF(cx,cy,cz,weight){}
};

template <typename floatPrecision = double>
class Triharmonic : public RBF<floatPrecision>{
	virtual floatPrecision _eval(floatPrecision r2) const{
		return r2*std::sqrt(r2);
	}
public:
	Triharmonic(floatPrecision cx,floatPrecision cy,floatPrecision cz,floatPrecision weight = 1):RBF(cx,cy,cz,weight){}
};

template <typename floatPrecision = double>
class GausianRBF : public RBF<floatPrecision>{
	virtual floatPrecision _eval(floatPrecision r2) const{
		return std::exp(-_a*r2);
	}
	floatPrecision _a;
public:
	GausianRBF(floatPrecision cx,floatPrecision cy,floatPrecision cz,float a = 1.0,floatPrecision weight = 1.0):RBF<floatPrecision>(cx,cy,cz,weight), _a(a){}
};

template <typename floatPrecision = double>
class MultiQuadricRBF : public RBF<floatPrecision>{
	virtual floatPrecision _eval(floatPrecision r2) const{
		return std::sqrt(r2+_a);
	}
	floatPrecision _a;
public:
	MultiQuadricRBF(floatPrecision cx,floatPrecision cy,floatPrecision cz,float a = 1.0,floatPrecision weight = 1.0):RBF<floatPrecision>(cx,cy,cz,weight), _a(a*a){}
};


template <typename KernelType,typename floatPrecision = double>
class RBFSystem{
	struct TrendFunction{
		floatPrecision _c[4];
		floatPrecision eval(floatPrecision x,floatPrecision y,floatPrecision z){return _c[0] + x * _c[1] + y * _c[2] + _c[3];}
		TrendFunction(){_c[0] = _c[1] = _c[2] = _c[3] = 0;}
		TrendFunction(floatPrecision c0,floatPrecision c1,floatPrecision c2,floatPrecision c3){_c[0] = c0;_c[1] = c1;_c[2] = c2;_c[3] = c3;}
	}_trend;

	std::vector<KernelType*> _kernels;
public:

	static RBFSystem *CreateFromPoints(std::vector<NormalPoint> &tree);
};


template <typename KernelType,typename floatPrecision>
RBFSystem<KernelType,floatPrecision> *RBFSystem<KernelType,floatPrecision>::CreateFromPoints(std::vector<NormalPoint> &points){
	RBFSystem<KernelType,floatPrecision> *rbfs = new RBFSystem<KernelType,floatPrecision>();

	unsigned long size = points.size(); 




	int n = 5 ;
	int Ap [ ] = {0, 2, 5, 9, 10, 12} ;
	int Ai [ ] = { 0, 1, 0, 2, 4, 1, 2, 3, 4, 2, 1, 4} ;
	double Ax [ ] = {2., 3., 3., -1., 4., 4., -3., 1., 2., 2., 6., 1.} ;
	double b [ ] = {8., 45., -3., 3., 19.} ;
	double x [5] ;

	double *null = (double *) NULL ;
	int i ;
	void *Symbolic, *Numeric ;
	(void) umfpack_di_symbolic (n, n, Ap, Ai, Ax, &Symbolic, null, null) ;
	(void) umfpack_di_numeric (Ap, Ai, Ax, Symbolic, &Numeric, null, null) ;
	//umfpack_di_free_symbolic (&Symbolic) ;
	//(void) umfpack_di_solve (UMFPACK_A, Ap, Ai, Ax, x, b, Numeric, null, null) ;
	//umfpack_di_free_numeric (&Numeric) ;
	//for (i = 0 ; i < n ; i++) printf ("x [%d] = %g\n", i, x [i]) ;


	//boost::numeric::ublas::matrix<floatPrecision,row_major> A(size+4,size+4), Ai(size+4,size+4),x(1,size+4),p(1,size+4);
	//
	//
	////TODO Build a
	//InvertMatrix(A, Ai);
	//for(int i = 0;i<size;i++){
	//	KernelType *rbf= new KernelType(points[i].P.x,points[i].P.y,points[i].P.z);
	//	for(int j = 0;j<size;j++){
	//		floatPrecision v = rbf->eval(points[j].P.x,points[j].P.y,points[j].P.z);
	//		A.insert_element(i,j,v);
	//	}
	//}

	//TODO Build p




	return rbfs;
}

#endif
