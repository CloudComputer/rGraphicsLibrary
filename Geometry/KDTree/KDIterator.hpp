#ifndef _KDITERATOR_HPP_
#define _KDITERATOR_HPP_
#include "KDTree.h"

KD_TEMPLATE
class KDNodeIterator : 
	public boost::iterator_facade< KD_NODE_IT, KD_NODE,boost::forward_traversal_tag> , public Object
{
	KD_NODE *current;
public:
	KDNodeIterator() : current(0) {}
    explicit KDNodeIterator(KD_NODE* node) : current(node) {}

	void increment(){
		if(current == 0) //already at end
			return;
		if(!current->isLeftLeaf()){
			current = current->_left;
			return;
		}
		if(!current->isRightLeaf()){
			current = current->_right;
			return;
		}

		if(current->isLeaf()){
			KD_NODE *p;
			do{
				p = current->_parent;
				if(p->_left == current && !p->isRightLeaf()){
					current = p->_right;
					return;
				}
				current = p;
			}while(current->_parent!=0);
			current = 0;
		}else{
			std::cerr << "This should not be runned " << __FILE__ << "@" << __LINE__ << std::endl;
		}

	}
	bool equal(const KDNodeIterator &it)const{
		return it.current == current;
	}

	KD_NODE& dereference() const{return *current;}

	virtual std::string toString() const{
		return current->toString();
	}
};



KD_TEMPLATE
class KDDataIterator : 
	public boost::iterator_facade< KD_DATA_IT, dataType,boost::forward_traversal_tag> , public Object
{
	KD_NODE *current;
public:
	KDDataIterator() : current(0) {}
    explicit KDDataIterator(KD_NODE* node) : current(node) {}

	void increment(){
		if(current == 0) //already at end
			return;
		if(!current->isLeftLeaf()){
			current = current->_left;
			return;
		}
		if(!current->isRightLeaf()){
			current = current->_right;
			return;
		}

		if(current->isLeaf()){
			KD_NODE *p;
			do{
				p = current->_parent;
				if(p->_left == current && !p->isRightLeaf()){
					current = p->_right;
					return;
				}
				current = p;
			}while(current->_parent!=0);
			current = 0;
		}else{
			std::cerr << "This should not be runned " << __FILE__ << "@" << __LINE__ << std::endl;
		}

	}
	bool equal(const KDDataIterator &it)const{
		return it.current == current;
	}

	dataType& dereference() const{return current->get();}

	virtual std::string toString() const{
		return current->toString();
	}
};


KD_TEMPLATE
class KDPositionIterator : 
	public boost::iterator_facade< KD_POS_IT, floatPrecision[dimmensions],boost::forward_traversal_tag> , public Object
{
	KD_NODE *current;
public:
	KDPositionIterator() : current(0) {}
    explicit KDPositionIterator(KD_NODE* node) : current(node) {}

	void increment(){
		if(current == 0) //already at end
			return;
		if(!current->isLeftLeaf()){
			current = current->_left;
			return;
		}
		if(!current->isRightLeaf()){
			current = current->_right;
			return;
		}

		if(current->isLeaf()){
			KD_NODE *p;
			do{
				p = current->_parent;
				if(p->_left == current && !p->isRightLeaf()){
					current = p->_right;
					return;
				}
				current = p;
			}while(current->_parent!=0);
			current = 0;
		}else{
			std::cerr << "This should not be runned " << __FILE__ << "@" << __LINE__ << std::endl;
		}

	}
	bool equal(const KDPositionIterator &it)const{
		return it.current == current;
	}

	float* dereference() const{return current->getPosition();}

	virtual std::string toString() const{
		return current->toString();
	}
};

#endif
