#ifndef _KDNODE_HPP_
#define _KDNODE_HPP_
#include "KDTree.h"

#include <algorithm>

KD_TEMPLATE KD_NODE::KDNode(KD_TREE *tree,const floatPrecision pos[dimmensions],const dataType &data,KD_NODE *parent):
_data(data),
_parent(parent),
_left(0),
_right(0)
{
	for(int i = 0;i<dimmensions;i++){
		_pos[i] = pos[i];
	}
	_dimmension = _parent == 0 ? 0 : (_parent->_dimmension+1)%dimmensions;
}

KD_TEMPLATE KD_NODE::~KDNode(){
	//if(_left != 0)
		delete _left;
	//if(_right != 0)
		delete _right;
}

KD_TEMPLATE bool KD_NODE::isOk()const{
	if(isLeaf())
		return true;
	if(!isLeftLeaf() && _goRight(_left->_pos)){
		std::cout << "This: " << this << std::endl;
		std::cout << "_left: " << _left << std::endl;
		return false;
	}
	if(!isRightLeaf() && !_goRight(_right->_pos)){
		std::cout << "This: " << this << std::endl;
		std::cout << "_right: " << _right << std::endl;
		return false;
	}

	if(!isLeftLeaf() && !_left->isOk())
		return false;

	if(!isRightLeaf() && !_right->isOk())
		return false;

	return true;
}

KD_TEMPLATE bool KD_NODE::isLeaf()const{
	return isRightLeaf() && isLeftLeaf();
}
KD_TEMPLATE bool KD_NODE::isRightLeaf()const{
	return _right == 0;
}
KD_TEMPLATE bool KD_NODE::isLeftLeaf()const{
	return _left == 0;
}


KD_TEMPLATE unsigned long KD_NODE::depth()const{
	unsigned long l = 0,r = 0;
	if(!isRightLeaf())
		r = _right->depth();
	if(!isLeftLeaf())
		l = _left->depth();
	return 1 + std::max(l,r);

}

KD_TEMPLATE unsigned long KD_NODE::size()const{
	unsigned long l = 0,r = 0;
	if(!isRightLeaf())
		r = _right->size();
	if(!isLeftLeaf())
		l = _left->size();
	return 1 + l + r;
}

KD_TEMPLATE dataType &KD_NODE::get(){
	return _data;
}

KD_TEMPLATE floatPrecision* KD_NODE::getPosition(){
	return _pos;
}

KD_TEMPLATE
KD_NODE* KD_NODE::insert(const floatPrecision pos[dimmensions], const dataType &data){
	bool right = _goRight(pos);
	if(right){
		if(_right == 0){
			_right = new KDNode(0,pos,data,this);	
			return _right;
		}
		return _right->insert(pos,data);
	}
	else{
		if(_left == 0){
			_left = new KDNode(0,pos,data,this);	
			return _left;
		}
		return _left->insert(pos,data);
	}
}

KD_TEMPLATE bool KD_NODE::_goRight(const floatPrecision pos[dimmensions])const{
	return pos[_dimmension] >= _pos[_dimmension];
}

KD_TEMPLATE bool KD_NODE::_compare(const floatPrecision pos[dimmensions])const{
	for(unsigned int i = 0;i<dimmensions;i++){
		if(pos[i] != _pos[i])
			return false;
	}
	return true;
}

KD_TEMPLATE KD_NODE* KD_NODE::findMin(unsigned int d){
	if(_dimmension == d){
		return isLeftLeaf() ? this : _left->findMin(d);
	}
	if(isLeaf()) // leaf node
		return this;

	KDNode *l,*r,*minChild;
	if(isLeftLeaf()){ //if left is empty, minimum value of dimension d is either this node or in right subtree
		minChild = _right->findMin(d);
	}else if(isRightLeaf())
		minChild = _left->findMin(d);
	else{
		l = _left->findMin(d);
		r = _right->findMin(d);
		minChild = (l->_pos[d] <= r->_pos[d]) ? l : r;
	}
	return (minChild->_pos[d] <= _pos[d]) ? minChild : this;
}


KD_TEMPLATE KD_NODE* KD_NODE::findMax(unsigned int d){
	if(_dimmension == d){
		return isRightLeaf() ? this : _right->findMax(d);
	}
	if(isLeaf())
		return this;
	

	KDNode *l,*r,*maxChild;
	if(isLeftLeaf()){ //if left is empty, minimum value of dimension d is either this node or in right subtree
		maxChild = _right->findMax(d);
	}else if(isRightLeaf())
		maxChild = _left->findMax(d);
	else{
		l = _left->findMax(d);
		r = _right->findMax(d);
		maxChild = (l->_pos[d] >= r->_pos[d]) ? l : r;
	}
	return (maxChild->_pos[d] >= _pos[d]) ? maxChild : this;
}

KD_TEMPLATE KD_NODE* KD_NODE::find(const floatPrecision pos[dimmensions]){
	if(_compare(pos))
		return this;
	if(_goRight(pos)){
		return isRightLeaf() ? 0 : _right->find(pos);
	}else{
		return isLeftLeaf()  ? 0 : _left->find(pos);
	}
}


KD_TEMPLATE KD_NODE* __closestTo(const floatPrecision pos[dimmensions],KD_NODE *n0,KD_NODE *n1){
	if(n0 == 0)
		return n1;
	if(n1 == 0 || n0 == n1)
		return n0;
	float d0 = 0,d1 = 0,dx;
	for(int i = 0;i<dimmensions;i++){
		dx = pos[i] - n0->getPosition()[i];
		d0 += dx*dx;
		
		dx = pos[i] - n1->getPosition()[i];
		d1 += dx*dx;
	}
	/* 
	this is not needed
	d0 = std::sqrt(d0);
	d1 = std::sqrt(d1);
	*/
	return (d0 <= d1) ? n0 : n1;
}


KD_TEMPLATE KD_NODE* KD_NODE::findNearest(const floatPrecision pos[dimmensions],KD_NODE *nearest){
	if(isLeaf()){
		nearest = __closestTo<KD_TYPE>(pos,this,nearest);
		return nearest;
	}

	float d0,d1;
	if(_goRight(pos)){  //pos is within right sub tree
		if(!isRightLeaf()){ //Look forit in right sub tree
			nearest = __closestTo<KD_TYPE>(pos,nearest,_right->findNearest(pos,nearest));
		}
		nearest = __closestTo<KD_TYPE>(pos,nearest,this);
		if(nearest == this && !isLeftLeaf()){//if current node is the closest in 
			nearest = __closestTo<KD_TYPE>(pos,this,_left->findNearest(pos,nearest));
		}
		else if(!isLeftLeaf()){ //can it 
			d0 = pos[_dimmension] - this->_pos[_dimmension];
			d1 = pos[_dimmension] - nearest->_pos[_dimmension];
			d0 = d0*d0;
			d1 = d1*d1;

			if(d0<=d1){
				nearest = __closestTo<KD_TYPE>(pos,nearest,_left->findNearest(pos,nearest));
			}	
		}
	}
	else{ 
		if(!isLeftLeaf()){ //Look forit in right sub tree
			nearest = __closestTo<KD_TYPE>(pos,nearest,_left->findNearest(pos,nearest));
		}
		nearest = __closestTo<KD_TYPE>(pos,nearest,this);
		if(nearest == this && !isRightLeaf()){//if current node is the closest in 
			nearest = __closestTo<KD_TYPE>(pos,this,_right->findNearest(pos,nearest));
		}
		else if(!isRightLeaf()){ //can it 
			d0 = pos[_dimmension] - this->_pos[_dimmension];
			d1 = pos[_dimmension] - nearest->_pos[_dimmension];
			d0 = d0*d0;
			d1 = d1*d1;

			if(d0<d1){
				nearest = __closestTo<KD_TYPE>(pos,nearest,_right->findNearest(pos,nearest));
			}	
		}
	}
	return nearest;
}

KD_TEMPLATE floatPrecision __sqDist(const floatPrecision p0[dimmensions],const floatPrecision p1[dimmensions]){
	floatPrecision d = 0,a;
	for(int i = 0;i<dimmensions;i++){
		a = p0[i] - p1[i];
		d += a*a;
	}
	return d;
}

KD_TEMPLATE void KD_NODE::findCloseTo(const floatPrecision pos[dimmensions],const floatPrecision sqDist,std::vector<KDNode*> &nodes){
	if(__sqDist<KD_TYPE>(pos,_pos) < sqDist){
		nodes.push_back(this);
	}
	float d;
	bool right = false,left = false;
//BUG ??
	if(_goRight(pos)){
		if(!isRightLeaf()){
			right = true;
		}
		if(!isLeftLeaf()){
			d = _pos[_dimmension] - pos[_dimmension];
			if(d*d<=sqDist)
				left = true;
		}
	}else{
		if(!left && !isLeftLeaf()){
			left = true;
		}
		if(!right && !isRightLeaf()){
			d = _pos[_dimmension] - pos[_dimmension];
			if(d*d<=sqDist)
				right = true;
		}
	}
	if(right){
		_right->findCloseTo(pos,sqDist,nodes);
	}
	if(left){
		_left->findCloseTo(pos,sqDist,nodes);
	}
}



#define __FF__ std::cout << "Failed at " << __FILE__ << "@" << __LINE__ << std::endl
KD_TEMPLATE void KD_NODE::swap(KD_NODE* n0,KD_NODE* n1){
	if(n0 == n1  || n0 == 0 || n1 == 0 )
		return;

	KD_NODE* r0 = n0->_right;
	KD_NODE* l0 = n0->_left;
	KD_NODE* p0 = n0->_parent;
	KD_NODE* r1 = n1->_right;
	KD_NODE* l1 = n1->_left;
	KD_NODE* p1 = n1->_parent;

	if(p0 == n1){ //if n0 is a child to n1
		swap(n1,n0); //swap in "other" direction
		return;
	}
	if(p1 == n0){ //n1 is a child to n0
		bool left = l0 == n1;
		n0->_parent = n1;
		n0->_left = l1;
		n0->_right = r1;

		n1->_parent = p0;
		n1->_left  = left ? n0 : l0;
		n1->_right = left ? r0 : n0;

		if(r1 != 0) r1->_parent = n0;
		if(l1 != 0) l1->_parent = n0;

		if(p0 != 0){
			if(p0->_left  == n0) p0->_left = n1; //
			if(p0->_right == n0) p0->_right = n1; //
		}
		
		if(left && r0 != 0){ //since l0 == n1 we've already set the parent above
			r0->_parent = n1;
		}
		else if(!left && l0 != 0){
			l0->_parent = n1;
		}
		
	}else{
		if(r0 != 0) r0->_parent = n1;
		if(l0 != 0) l0->_parent = n1;
		if(r1 != 0) r1->_parent = n0;
		if(l1 != 0) l1->_parent = n0;
		n0->_left = l1;
		n0->_right = r1;
		n0->_parent = p1;
		n1->_left = l0;
		n1->_right = r0;
		n1->_parent = p0;

		if(p0 != 0 && p0->_left == n0){
			p0->_left = n1;
		}else if(p0 != 0 && p0->_right == n0){
			p0->_right = n1;
		}else if(p0 != 0){
			__FF__;	
		}
			
		if(p1->_left == n1){
			p1->_left = n0;
		}else if(p1->_right == n1){
			p1->_right = n0;
		}else{
			__FF__;	
		}
	}

	std::swap(n0->_dimmension,n1->_dimmension);
}


KD_TEMPLATE std::string KD_NODE::toString()const{
	std::stringstream ss;
	ss << typeid(*this).name() << " " << (void*)this << " ";
	//ss << _tree << " ";
	//ss << _data << " [";
	for(int i = 0;i<dimmensions;i++){
		ss << " "<< _pos[i];
	}
	ss << " ] " << _dimmension;

	ss << (void*)_parent << " ";
	ss << (void*)_left << " ";
	ss << (void*)_right << " ";
	return ss.str();
}

KD_TEMPLATE void KD_NODE::findNNearest(const floatPrecision pos[dimmensions],int amount,std::vector<__KD_NEAR_NODE__<KD_TYPE>> &current){
	float sqDist = __sqDist<KD_TYPE>(pos,_pos);
	if(current.size() < amount){
		__KD_NEAR_NODE__<KD_TYPE> holder;
		holder.node = this;
		holder.sqDist = sqDist;
		current.push_back(holder);
		std::sort(current.begin(),current.end());
	}
	else if(sqDist<=current[0].sqDist){
		__KD_NEAR_NODE__<KD_TYPE> holder;
		holder.node = this;
		holder.sqDist = sqDist;
		current.push_back(holder);
		std::sort(current.begin(),current.end());
		while(current.size()>amount){
			current.erase(current.begin());
		}
	}
	
	float d;
	bool right = false,left = false;
	if(current.size()<amount){
		left = !isLeftLeaf();
		right = !isRightLeaf();
	}
	else if(_goRight(pos)){
		if(!isRightLeaf()){
			right = true;
		}
		if(!isLeftLeaf()){
			d = _pos[_dimmension] - pos[_dimmension];
			if(d*d<=current[0].sqDist)
				left = true;
		}
	}else{
		if(!left && !isLeftLeaf()){
			left = true;
		}
		if(!right && !isRightLeaf()){
			d = _pos[_dimmension] - pos[_dimmension];
			if(d*d<=current[0].sqDist)
				right = true;
		}
	}
	if(right){
		_right->findNNearest(pos,amount,current);
	}
	if(left){
		_left->findNNearest(pos,amount,current);
	}

}
	

#endif
