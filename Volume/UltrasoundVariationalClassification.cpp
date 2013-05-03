#include "UltrasoundVariationalClassification.h"

#include "VectorField.h"

#include <Util\TmpPointer.h>

#include <Eigen\Sparse>

#include <Eigen\IterativeLinearSolvers>

UltrasoundVariationalClassification::UltrasoundVariationalClassification(ScalarField *v,float alpha,float beta,float w,float iso,float uind,float xi) : 
ScalarField(v->getDimensions(),v->getBoundingAABB())
{
	float gamma = 1 - alpha - beta;
	float epsilon = Eigen::NumTraits<float>::epsilon();

	
	auto dim = v->getDimensions();
	int size = dim.x * dim.y * dim.z;
	
	Eigen::SparseMatrix<float> A(size,size);DBG("A");
	A.reserve(Eigen::VectorXi::Constant(size,7));DBG("A.reserve");
	Eigen::VectorXf b(size), u ;DBG("b u");
	
	{
		TmpPointer<ScalarField> v_blur = v->blur();DBG("v_blur"); //45 Mb									//keep this
		//TmpPointer<VectorField> v_grad = v->getGradientField();DBG("v_grad"); //137mb						//remove this
		//TmpPointer<VectorField> v_blur_grad = v_blur->getGradientField();DBG("v_blur_grad");//137mb			//remove this
		//TmpPointer<VectorField> curl = v_blur_grad->getCurlField();DBG("curl");  //137mb					//remove this by implementing gradientCurl on scalar field
		
		
		int ix,iy,iz;
		ix = 1;
		iy = dim.x;
		iz = dim.y*dim.x;
		//Build A start
		std::vector<Eigen::Triplet<float>> a_val;
		//a_val.reserve(7*size);
		FOR(dim){
			auto iv = glm::ivec3(x,y,z);
			int id = _index(iv);
			float a = 0,g = 0;
			a += w*std::powf(v->_data[id] - iso,2);
			a += (1-w)*std::powf(v_blur->_data[id] - iso,2);
			a *= alpha;
			auto grad = v->getGradient(iv);
			//auto grad = v_grad->_data[id];
			a /= glm::dot(grad,grad)+epsilon;

			g = xi+(1-xi)*glm::length(grad);
			g *= gamma;

		
			auto V = v_blur->getGradient(iv);
			auto c = v->getGetGradientCurl(iv);// curl->_data[id];

			float p1,p2,p3;
			p1  = c.z*V.y - c.y*V.z;
			p2  = c.x*V.z - c.z*V.x;		
			p3  = c.y*V.x - c.x*V.y;
		
			p1 /= _delta.x;
			p2 /= _delta.y;
			p3 /= _delta.z;
		
			p1 *= beta;
			p2 *= beta;
			p3 *= beta;

			//a_val.push_back(Eigen::Triplet<float>(id,id, a+g));
			////
			//a_val.push_back(Eigen::Triplet<float>(id,id+ix,upx * beta));
			//a_val.push_back(Eigen::Triplet<float>(id,id-ix,umx * beta));
			////
			//a_val.push_back(Eigen::Triplet<float>(id,id+iy,upy * beta));
			//a_val.push_back(Eigen::Triplet<float>(id,id-iy,umy * beta));
			////
			//a_val.push_back(Eigen::Triplet<float>(id,id+iz,upz * beta));
			//a_val.push_back(Eigen::Triplet<float>(id,id-iz,umz * beta));

			//*

			b(id) = g*uind;

			A.insert(id,id) = a + g;
		
			if(x==0||y==0||z==0||x==dim.x-1||y==dim.y-1||z==dim.z-1)
				continue;

			A.insert(id,id+ix) = p1;
			A.insert(id,id-ix) = -p1;
		
			A.insert(id,id+iy) = p2;
			A.insert(id,id-iy) = -p2;
		
			A.insert(id,id+iz) = p3;
			A.insert(id,id-iz) = -p3;
			//*/
		}
		/*std::cout << "Filling matrix" << std::endl;
		A.setFromTriplets(a_val.begin(),a_val.end());*/
		//Build A end
	}
	Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > cg;
	std::cout << "Compute solver" << std::endl;
	cg.compute(A);
	std::cout << "Compute solver done" << std::endl;
	switch(cg.info()){
	case Eigen::Success:
		break;
	case Eigen::NumericalIssue:
		std::cerr << "NumericalIssue, The provided data did not satisfy the prerequisites." << std::endl;
		break;
	case Eigen::NoConvergence:
		std::cerr << "NoConvergence, Iterative procedure did not converge." << std::endl;
		break;
	case Eigen::InvalidInput:
		std::cerr << "InvalidInput, The inputs are invalid, or the algorithm has been improperly called. When assertions are enabled, such errors trigger an assert." << std::endl;
		break;
	}
	
	cg.setMaxIterations(15);
	cg.setTolerance(Eigen::NumTraits<float>::epsilon()*10);
	u = cg.solve(b);

	std::cout << "#iterations: " << cg.iterations() << std::endl;
	std::cout << "estimated error: " << cg.error() << std::endl;

	FOR(dim){
		int id = INDEX(this,x,y,z);
		_data[id] = u(id);
		if(!(u(id) == u(id))){
			_data[id] = 0;
		}
	}

	std::cout << "Min/max value of volume: " << this->getMin() << "/" << this->getMax() << std::endl;

}


UltrasoundVariationalClassification::~UltrasoundVariationalClassification(){


}


 