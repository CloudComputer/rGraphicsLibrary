#include <lsolver\gmres.h>
#include <lsolver\cghs.h>
#include <Eigen\Dense>

#include <glm\glm.hpp>

#include <Util\StopClock.h>

#include <vector>

#include <iostream>
#include <iomanip>


#include <boost/timer.hpp>

#include "gmres\lin_op.hpp"
#include "gmres\orthogonal.hpp"
#include "gmres\precond.hpp"


typedef boost::numeric::ublas::compressed_vector< float > boost_cmp;
typedef boost::numeric::ublas::compressed_matrix< float, boost::numeric::ublas::column_major > boost_cmp_m;
typedef boost::numeric::ublas::matrix< float, boost::numeric::ublas::column_major > boost_mat_dense;
typedef boost::numeric::ublas::banded_matrix< float, boost::numeric::ublas::column_major > boost_band_matr;

typedef boost::numeric::ublas::vector<float> boost_Vector;


std::vector<glm::vec4> points;

#define size 4000

template <typename solver>
Eigen::VectorXd testEigen(const Eigen::MatrixXd &A,const Eigen::VectorXd &b){
	return solver(A).solve(b);
}

//
//boost_Vector testGMRES(const boost_mat_dense &A,const boost_Vector &b){
//	float tol = 1.0e-8;
//
//	LinOp< boost_mat_dense > T(A);
//	DiagonalPreconditioner< boost_mat_dense > prec(A);
//
//
//	boost_Vector x(size);
//	for(unsigned int i = 0;i<size;i++)
//		x(i) = b(i);
//	gmres_short<  boost_mat_dense >(T, x, b, prec , 100, 30 , tol);
//	//gmres<   boost_mat_dense , boost_Vector >(A, x, b, tol);
//	return x;
//}

float rbf(const float &r){
	return r;
}

struct Result{
	std::string name;
	float seconds;
	float error;
	float relError;

	virtual ~Result(){
		std::cout << std::setw(25) << name;
		std::cout << std::setw(15) << seconds << " sec";
		std::cout << std::setw(15) << error;
		std::cout << std::setw(15) << relError << std::endl;
	}
};


int main( int argc, char* argv[] )
{
	
	Eigen::MatrixXd A = Eigen::MatrixXd::Zero(size,size);
	Eigen::VectorXd b = Eigen::VectorXd::Zero(size);
	
	boost_mat_dense A2(size,size);
	boost_Vector b2(size);

	for(unsigned int i = 0;i<size;i++){
		glm::vec4 p(rand() / float(RAND_MAX),rand() / float(RAND_MAX),rand() / float(RAND_MAX),0.5);		
		p *= 2;
		p -= 1;
		float r = rand() / float(RAND_MAX);
		r = r * 0.2f - 0.1f;
		p = glm::normalize(p) * r;
		p.w = r;
		points.push_back(p);
	}

	for(unsigned int i = 0;i<size;i++){
		b(i)  = points[i].w;
		b2(i) = points[i].w;
		for(int j = i;j<size;j++){
			A(i,j)  = rbf(glm::distance(glm::vec3(points[i]),glm::vec3(points[j])));
			A(j,i) = A(i,j);
			A2(i,j) = A(i,j);
			A2(j,i) = A(i,j);
		}
	}

	std::vector<Result> res;
	
	/*
	{
	StopClock sc(true);
	auto v2 = testGMRES(A2,b2);
	sc.stop();
	Result r;
	r.name = "GMRES";
	r.seconds = sc.getFractionElapsedSeconds();
	boost_Vector result = boost::numeric::ublas::prod( A2,v2) - b2;
	r.error =  boost::numeric::ublas::norm_2(result);
	r.relError = r.error / b.norm();
	
	//res.push_back(r);
	} ///*/
	
	{
	StopClock sc(true);
	auto v = testEigen<Eigen::PartialPivLU<Eigen::MatrixXd>>(A,b);
	sc.stop();
	Result r;
	r.name = "Eigen::PartialPivLU";
	r.seconds = sc.getFractionElapsedSeconds();
	r.error = (A*v - b).norm();
	r.relError = r.error / b.norm();
	//res.push_back(r);
	}

	{
	StopClock sc(true);
	auto v = testEigen<Eigen::FullPivLU<Eigen::MatrixXd>>(A,b);
	sc.stop();
	Result r;
	r.name = "Eigen::FullPivLU";
	r.seconds = sc.getFractionElapsedSeconds();
	r.error = (A*v - b).norm();
	r.relError = r.error / b.norm();
	//res.push_back(r);
	}

	{
	StopClock sc(true);
	auto v = testEigen<Eigen::HouseholderQR<Eigen::MatrixXd>>(A,b);
	sc.stop();
	Result r;
	r.name = "Eigen::HouseholderQR";
	r.seconds = sc.getFractionElapsedSeconds();
	r.error = (A*v - b).norm();
	r.relError = r.error / b.norm();
	//res.push_back(r);
	}

	{
	StopClock sc(true);
	auto v = testEigen<Eigen::ColPivHouseholderQR<Eigen::MatrixXd>>(A,b);
	sc.stop();
	Result r;
	r.name = "Eigen::ColPivHouseholderQR";
	r.seconds = sc.getFractionElapsedSeconds();
	r.error = (A*v - b).norm();
	r.relError = r.error / b.norm();
	//res.push_back(r);
	}

	{
	StopClock sc(true);
	auto v = testEigen<Eigen::FullPivHouseholderQR<Eigen::MatrixXd>>(A,b);
	sc.stop();
	Result r;
	r.name = "Eigen::FullPivHouseholderQR";
	r.seconds = sc.getFractionElapsedSeconds();
	r.error = (A*v - b).norm();
	r.relError = r.error / b.norm();
	//res.push_back(r);
	}

	{
	StopClock sc(true);
	auto v = testEigen<Eigen::LLT<Eigen::MatrixXd>>(A,b);
	sc.stop();
	Result r;
	r.name = "Eigen::LLT";
	r.seconds = sc.getFractionElapsedSeconds();
	r.error = (A*v - b).norm();
	r.relError = r.error / b.norm();
	//res.push_back(r);
	}

	{
	StopClock sc(true);
	auto v = testEigen<Eigen::LDLT<Eigen::MatrixXd>>(A,b);
	sc.stop();
	Result r;
	r.name = "Eigen::LDLT";
	r.seconds = sc.getFractionElapsedSeconds();
	r.error = (A*v - b).norm();
	r.relError = r.error / b.norm();
	//res.push_back(r);
	}


	return 0;
}

