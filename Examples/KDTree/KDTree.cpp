#include <iostream>


#include <Util\StopClock.h>

#include <Geometry\KDTree.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <algorithm>
#include <random>

//#include "C:\Program Files (x86)\Visual Leak Detector\include\vld.h"


StopClock s;

void test1(){
	KDTree<int,3> tree;
	std::vector<glm::dvec3> data;
	std::vector<KDTree<int,3>::Node*> nodes;
	std::vector<glm::dvec3>::iterator it;
	std::vector<KDTree<int,3>::Node*>::iterator node;

	unsigned long SIZE = 0xFFFF;
	std::cout << "Creating tree of size " << SIZE << std::endl;
	//for(int i = 0;i<100;i++){/*
	s.restart();
	for(unsigned long i = 0;i<SIZE;i++){//*/
		double pos[3];
		pos[0] = rand() / double(RAND_MAX);
		pos[1] = rand() / double(RAND_MAX);
		pos[2] = rand() / double(RAND_MAX);
		pos[0] *= 4;
		pos[1] *= 4;
		pos[2] *= 4;
		pos[0] -= 2;
		pos[1] -= 2;
		pos[2] -= 2;
		nodes.push_back(tree.insert(pos,0));
		data.push_back(glm::dvec3(pos[0],pos[1],pos[2]));
	}
	s.stop();
	assert(tree.isOk());
	std::cout << "Tree created, size: " << tree.size() << " , depth: " << tree.depth() << std::endl; 
	std::cout << "Creation time: " << s.getFractionElapsedSeconds()  << "sec" << std::endl;

	for(it = data.begin();it != data.end();it++){
		KDTree<int,3>::Node *node = tree.find(glm::value_ptr(*it));
		if(node == 0){
			std::cout << "NOOO" << std::endl;
		}
		else if(node->getPosition()[0] != it->x || node->getPosition()[1] != it->y || node->getPosition()[2] != it->z){
			std::cout << "CRAP" << std::endl;
		}
	}
	
	int test = 0;
	KDTree<int,3>::Iterator tIt;
	for(tIt = tree.begin() ; tIt != tree.end() ;++tIt){
		tIt->get()++;
		assert(tIt->get()==1);
		test++;
	}

	assert(test == data.size());
	
	KDTree<int,3>::Node *_min0 = tree.findMin(0);
	KDTree<int,3>::Node *_min1 = tree.findMin(1);
	KDTree<int,3>::Node *_min2 = tree.findMin(2);
	std::cout << "Min " << _min0->getPosition()[0] << " " << _min1->getPosition()[1] << " " << _min2->getPosition()[2] << std::endl;
	
	double pos[] = {-5.0,-5.0,-5.0};
	double posC[] = {-0.5,-0.5,-0.5};
	tree.insert(pos,1);
	
	std::vector<KDTree<int,3>::Node*> closeNodes = tree.findCloseTo(posC,0.1);

	KDTree<int,3>::Node *close = tree.findNearest(posC);
	std::cout << "close " << close->getPosition()[0] << " " << close->getPosition()[1] << " " << close->getPosition()[2] << std::endl;
	

	_min0 = tree.findMin(0);
	_min1 = tree.findMin(1);
	_min2 = tree.findMin(2);
	
	std::cout << "Min " << _min0->getPosition()[0] << " " << _min1->getPosition()[1] << " " << _min2->getPosition()[2] << std::endl;
	
	
	_min0 = tree.findMax(0);
	_min1 = tree.findMax(1);
	_min2 = tree.findMax(2);
	std::cout << "Max " << _min0->getPosition()[0] << " " << _min1->getPosition()[1] << " " << _min2->getPosition()[2] << std::endl;
	
	double pos2[] = {5.0,5.0,5.0};
	tree.insert(pos2,1);
	
	_min0 = tree.findMax(0);
	_min1 = tree.findMax(1);
	_min2 = tree.findMax(2);
	
	std::cout << "Max " << _min0->getPosition()[0] << " " << _min1->getPosition()[1] << " " << _min2->getPosition()[2] << std::endl;
	
	KDTree<int,3>::Node *last = nodes.back();
	nodes.pop_back();
	data.pop_back();
	std::random_shuffle(data.begin(),data.end());

	int size = tree.size();
	std::cout << __LINE__ << std::endl;

	tree.erase(last);
	size--;
	assert(size == tree.size());
	assert(tree.isOk());
	
	s.restart();
	for(it = data.begin();it != data.end();it++){
		KDTree<int,3>::Node *node = tree.find(glm::value_ptr(*it));
		if(node==0){
			std::cout << "asdf" << std::endl;
			continue;
		}
		tree.erase(node);
		//size--;
		//unsigned long s = tree.size();
		////std::cout << size << " " << s << std::endl;
		//if(!tree.isOk()){
		//	std::cout << "isOk: " << size  << " " << s << std::endl;
		//	break;
		//}
		//if(size != s){
		//	std::cout << "size: " << size << " " << s << std::endl;
		//	break;
		//}
	}
	s.stop();
	std::cout << "Remove time: " << s.getFractionElapsedSeconds()  << "sec" << std::endl;


	std::cout << "size:" << size << std::endl;
	std::cout << "size:" << tree.size() << std::endl;

}

template<typename F>
void test2(unsigned long SIZE){
	KDTree<int,3,F> tree;
	std::cout << "Creating tree of size " << SIZE << std::endl;
	s.restart();
	for(unsigned long i = 0;i<SIZE;i++){
		F pos[3];
		pos[0] = rand() / F(RAND_MAX);
		pos[1] = rand() / F(RAND_MAX);
		pos[2] = rand() / F(RAND_MAX);
		pos[0] *= 20;
		pos[1] *= 20;
		pos[2] *= 20;
		pos[0] -= 10;
		pos[1] -= 10;
		pos[2] -= 10;
		tree.insert(pos,0);
	}
	s.stop();
	assert(tree.isOk());
	std::cout << "Tree created, size: " << tree.size() << " , depth: " << tree.depth() << std::endl; 
	std::cout << "Creation time: " << s.getFractionElapsedSeconds()  << "sec" << std::endl << std::endl;
}

int main(int argc,char **argv){
	test1();
	/*test2<float>(0xFFFFF);
	test2<double>(0xFFFFF);
	test2<float>(40000000);
*/
	return 0;
}

