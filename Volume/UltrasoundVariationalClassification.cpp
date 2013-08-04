#include "UltrasoundVariationalClassification.h"

#include "VectorField.h"

#include <Util\TmpPointer.h>
#include <Util\Logger.h>

#include <Eigen\Sparse>
#include <Eigen\IterativeLinearSolvers>

inline glm::vec3 _grad(const int &x,const int &y,const int &z,const ScalarField *v,const glm::vec3 &s){
	if(x==0||y==0||z==0||x==v->getDimensions().x-1||y==v->getDimensions().y-1||z==v->getDimensions().z-1)
		return glm::vec3(0,0,0);
	glm::vec3 g;
	
	glm::ivec3 p(x,y,z);
	glm::ivec3 X(1,0,0);
	glm::ivec3 Y(0,1,0);
	glm::ivec3 Z(0,0,1);

	g.x = (v->get(p+X) - v->get(p-X))/(2.0f*s.x);
	g.y = (v->get(p+Y) - v->get(p-Y))/(2.0f*s.y);
	g.z = (v->get(p+Z) - v->get(p-Z))/(2.0f*s.z);
	return g;
}

glm::vec3 _curl(const int &x,const int &y,const int &z,const ScalarField *v,const glm::vec3 &s,const std::vector<glm::vec3> &grads){
	if(x<=1||y<=1||z<=1||x>=v->getDimensions().x-2||y>=v->getDimensions().y-2||z>=v->getDimensions().z-2)
		return glm::vec3(0,0,0);
	glm::vec3 c(0,0,0),Fx,Fy,Fz;
	
	int id = x + (y * v->getDimensions().x) + (z * v->getDimensions().x * v->getDimensions().y);

	int dz =  v->getDimensions().x * v->getDimensions().y;

	Fx = (grads[id+1] - grads[id-1]);
	Fy = (grads[id+v->getDimensions().x] - grads[id-v->getDimensions().x]);
	Fz = (grads[id+dz] - grads[id-dz]);
	
	c.x = Fy.z/s.y - Fz.y/s.z; 
	c.y = Fz.x/s.z - Fx.z/s.x; 
	c.z = Fx.y/s.x - Fy.x/s.y; 
	
	return c;
}

void UltrasoundVariationalClassification::init(ScalarField *v,float alpha,float beta,float gamma,float w,float iso,float uind,float xi)
{
	float epsilon = Eigen::NumTraits<float>::epsilon();

    LOG_DEBUG("Bluring input volume");
	TmpPointer<ScalarField> vb = v->blur();
	
	auto dim = v->_dimensions;
	int size = dim.x * dim.y * dim.z;
	
    LOG_DEBUG("Defining Matrix A");
	Eigen::SparseMatrix<float> A(size,size);
    LOG_DEBUG("Reserving memory for " << (size*7) << " entries in Matrix A");
	A.reserve(Eigen::VectorXi::Constant(size,7));
	LOG_DEBUG("Creating Vector b and u");
    Eigen::VectorXf b(size), u;

    LOG_DEBUG("Reserving memory for " << size << " gradients");
    blurred_gradints.reserve(size);
	glm::vec3 spacing = v->_delta;
	for(int z = 0;z<dim.z;z++)for(int y = 0;y<dim.y;y++)for(int x = 0;x<dim.x;x++){
		blurred_gradints.push_back(_grad(x,y,z,vb.get(),spacing));
	}
	{
		int ix,iy,iz;
		ix = 1;
		iy = dim.x;
		iz = dim.y*dim.x;
		
		float a,g;
		glm::vec3 grad,blured_grad,curl;

        LOG_DEBUG("Starting to build A");
		FOR(dim){
			auto iv = glm::ivec3(x,y,z);
			int id = _index(iv);
			
			grad = _grad(x,y,z,v,glm::vec3(1,1,1));
			blured_grad = blurred_gradints[id];
			curl = _curl(x,y,z,vb.get(),spacing,blurred_gradints) * beta;

			a = w*std::powf(v->_data[id] - iso,2);
			a += (1-w)*std::powf(vb->_data[id] - iso,2);
			a *= alpha;
			a /= glm::dot(grad,grad)+epsilon;

			g = xi+(1-xi)*glm::length(grad);
			g *= gamma;

			b(id) = g*uind;

			if(x==0||y==0||z==0||x==dim.x-1||y==dim.y-1||z==dim.z-1){
				A.insert(id,id) = a + g;
			}else{
				A.insert(id,id) = a + g;
				A.insert(id,id+ix) = curl.x;
				A.insert(id,id-ix) = -curl.x;
				A.insert(id,id+iy) = curl.y;
				A.insert(id,id-iy) = -curl.y;
				A.insert(id,id+iz) = curl.z;
				A.insert(id,id-iz) = -curl.z;
			}
		}
    }
    LOG_DEBUG("Completed building A");
	Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > cg;
	LOG_DEBUG("Compute solver");
	cg.compute(A);
	switch(cg.info()){
    case Eigen::Success:
        LOG_DEBUG("Compute solver Success");
		break;
    case Eigen::NumericalIssue:
        LOG_ERROR("Compute solver failed: " << "NumericalIssue, The provided data did not satisfy the prerequisites.");
		break;
	case Eigen::NoConvergence:
		LOG_ERROR("Compute solver failed: " << "NoConvergence, Iterative procedure did not converge.");
		break;
	case Eigen::InvalidInput:
		LOG_ERROR("Compute solver failed: " << "InvalidInput, The inputs are invalid, or the algorithm has been improperly called. When assertions are enabled, such errors trigger an assert.");
		break;
	}
	
	cg.setMaxIterations(15);
	cg.setTolerance(Eigen::NumTraits<float>::epsilon()*10);
	u = cg.solve(b);

    LOG_INFO("VariationalClassification completed after " << cg.iterations() << " with an estimated error of " << cg.error());

	int a1=0,a2=0,a3=0,a4=0,a5=0,a6=0,a7=0,a8=0,a9=0;

	float ma = u.maxCoeff();
	float mi = u.minCoeff();

	FOR(dim){
		int id = INDEX(this,x,y,z);
		_data[id] = (u(id)-mi)/(ma-mi);
		if(_data[id]>=0.9)
			a9++;
		if(_data[id]>=0.8)
			a8++;
		if(_data[id]>=0.7)
			a7++;
		if(_data[id]>=0.6)
			a6++;
		if(_data[id]>=0.5)
			a5++;
		if(_data[id]>=0.4)
			a4++;
		if(_data[id]>=0.3)
			a3++;
		if(_data[id]>=0.2)
			a2++;
		if(_data[id]>=0.1)
			a1++;
		if(!(u(id) == u(id))){
			_data[id] = 0;
		}
	}
	float dsize = (float)size;
	LOG_DEBUG (a1 << " " << a2 << " " << a3 << " " << a4 << " " << a5 << " " << a6 << " " << a7 << " " << a8 << " " << a9);
	LOG_DEBUG (a1/dsize << " " << a2/dsize << " " << a3/dsize << " " << a4/dsize << " " << a5/dsize << " " << a6/dsize << " " << a7/dsize << " " << a8/dsize << " " << a9/dsize);
	LOG_DEBUG ("Min/max value of volume: " << this->getMin() << "/" << this->getMax());

}


UltrasoundVariationalClassification::~UltrasoundVariationalClassification(){


}


 