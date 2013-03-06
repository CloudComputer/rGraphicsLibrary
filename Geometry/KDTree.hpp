#ifndef _KDTREE_HPP_
#define _KDTREE_HPP_
#include "KDTree.h"

#include <assert.h>

KD_TEMPLATE KD_TREE::KDTree(){
	root = 0;
}

KD_TEMPLATE KD_TREE::~KDTree(){
	if(root != 0)
		delete root;
}

KD_TEMPLATE KD_IT KD_TREE::begin(){
	return Iterator(root);
}
KD_TEMPLATE KD_IT KD_TREE::end(){
	return Iterator();
}

KD_TEMPLATE unsigned long KD_TREE::depth()const{
	if(root == 0)
		return 0;
	return root->depth();
}

KD_TEMPLATE unsigned long KD_TREE::size()const{
	if(root == 0)
		return 0;
	return root->size();
}

KD_TEMPLATE bool KD_TREE::isOk()const{
	if(root == 0)
		return true;
	return root->isOk();
}

KD_TEMPLATE KD_NODE* KD_TREE::findMin(unsigned int d){
	if(root == 0)
		return 0;
	return root->findMin(d);
}

KD_TEMPLATE KD_NODE* KD_TREE::findMax(unsigned int d){
	if(root == 0)
		return 0;
	return root->findMax(d);
}

KD_TEMPLATE KD_NODE* KD_TREE::insert(floatPrecision pos[dimmensions], const dataType &data){
	if(root == 0){
		root = new KD_NODE(this,pos,data);
		return root;
	}

	return root->insert(pos,data);
}

KD_TEMPLATE KD_NODE* KD_TREE::find(floatPrecision pos[dimmensions]){
	if(root == 0)
		return 0;
	return root->find(pos);
}

KD_TEMPLATE void KD_TREE::erase(KD_NODE *node){
	if(node == 0){
		std::cerr << "Trying to delete a null node" << std::endl;
		assert(node != 0);
		return;
	}
	if(node->_tree != this){
		std::cerr << "Trying to delete a node from another tree" << std::endl;
		assert(node->_tree == this);
		return;
	}
	if(node->isLeaf()){
		KD_NODE* parent = node->_parent;
		if(parent->_left == node){
			parent->_left = 0;
		}else if(parent->_right == node){
			parent->_right = 0;
		}else{
			std::cerr << "Incorrect parent or something" << __FILE__ << "@" << __LINE__ << std::endl;
			assert(false);
			return;
		}
		delete node;
		return;
	}
	
	KD_NODE* max;
	if(!node->isRightLeaf()){
		max = node->_right->findMin(node->_dimmension);
		KD_NODE::swap(node,max);
		if(root == node) root = max;
		erase(node);
		return;
	}
	else{
		max = node->_left->findMin(node->_dimmension);

		KD_NODE::swap(node,max);
		std::swap(max->_right , max->_left);
		if(root == node) root = max;
		erase(node);
		return;
	}
}

KD_TEMPLATE KD_NODE * KD_TREE::findNearest(floatPrecision pos[dimmensions]){
	if(root == 0)
		return 0;	
	return root->findNearest(pos,0);
}

KD_TEMPLATE std::vector<KD_NODE*> KD_TREE::findCloseTo(floatPrecision pos[dimmensions],floatPrecision distance){
	std::vector<KD_NODE*> nodes;
	if(root == 0)
		return nodes;
	root->findCloseTo(pos,distance*distance,nodes);
	return nodes;
}

KD_TEMPLATE std::string KD_TREE::toString()const{
	std::stringstream ss;
	ss << typeid(*this).name() << " " << ((void *)this);
	return ss.str();
}
//KD_TEMPLATE std::vector<KD_NODE*> KD_TREE::findCloseTo(floatPrecision pos[dimmensions],floatPrecision distance);{}



#endif
