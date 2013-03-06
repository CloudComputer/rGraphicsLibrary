#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <Base\Object.h>

#include <vector>

#include <boost\iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>

#define KD_TYPE dataType,dimmensions,floatPrecision
#define KD_TEMPLATE template <typename dataType,unsigned int dimmensions,typename floatPrecision>
#define KD_TREE KDTree<KD_TYPE>
#define KD_NODE KDNode<KD_TYPE>
#define KD_IT	KDIterator<KD_TYPE>

KD_TEMPLATE class KDTree;
KD_TEMPLATE class KDNode;

//KD_TEMPLATE
//class _KD_NN{
//	friend class KD_TREE;
//	friend class KD_NODE;
//
//	KD_NODE *nearest;
//};

KD_TEMPLATE
class KDIterator : 
	public boost::iterator_facade< KD_IT, KD_NODE,boost::forward_traversal_tag> , public Object
{
	KD_NODE *current;
public:
	KDIterator() : current(0) {}
    explicit KDIterator(KD_NODE* node) : current(node) {}

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
	bool equal(const KDIterator &it)const{
		return it.current == current;
	}

	KD_NODE& dereference() const{return *current;}

	virtual std::string toString() const{
		return current->toString();
	}

};

template <typename dataType,unsigned int dimmensions,typename floatPrecision = double> 
class KDNode: public Object{
	friend class KD_TREE;
	friend class KD_IT;
	KDNode *_parent;
	KDNode *_left;
	KDNode *_right;
	KD_TREE *_tree;
	dataType _data;
	unsigned int _dimmension;
	floatPrecision _pos[dimmensions];

	bool _goRight(floatPrecision pos[dimmensions])const;
	bool _compare(floatPrecision pos[dimmensions])const;
public:
	KDNode(KD_TREE *tree,floatPrecision pos[dimmensions],const dataType &data,KDNode *parent = 0);
	virtual ~KDNode();
	
	bool isLeaf()const;
	bool isRightLeaf()const;
	bool isLeftLeaf()const;

	unsigned long depth()const;
	unsigned long size()const;

	dataType &get();
	floatPrecision* getPosition();

	bool isOk()const;

	KDNode *findMin(unsigned int dimmensions); 
	KDNode *findMax(unsigned int dimmensions); 

	KDNode *insert(floatPrecision pos[dimmensions], const dataType &data);
	KDNode *find(floatPrecision pos[dimmensions]);

	KD_NODE* findNearest(floatPrecision pos[dimmensions],KDNode *nearest);
	void findCloseTo(floatPrecision pos[dimmensions],floatPrecision squaredDistance,std::vector<KDNode*> &nodes);

	static void swap(KD_NODE* n0,KD_NODE* n1);
	
	virtual std::string toString() const;
};



template <typename dataType,unsigned int dimmensions,typename floatPrecision = double> 
class KDTree: public Object{
public:
	typedef KDNode<KD_TYPE> Node;
private:
	Node *root;
public:
	typedef KD_IT Iterator;
	
	KDTree();
	virtual ~KDTree();

	void erase(Node *node);
	
	KD_IT begin();
	KD_IT end();

	bool isOk()const;

	unsigned long depth()const;
	unsigned long size()const;

	Node* findMin(unsigned int dimmensions);
	Node* findMax(unsigned int dimmensions);

	Node *insert(floatPrecision pos[dimmensions], const dataType &data);
	Node *find(floatPrecision pos[dimmensions]);
	
	Node *findNearest(floatPrecision pos[dimmensions]);
	std::vector<Node*> findCloseTo(floatPrecision pos[dimmensions],floatPrecision distance);

	virtual std::string toString() const;
};



#ifndef _KDTREE_HPP_
#include "KDTree.hpp"
#endif
#ifndef _KDNODE_HPP_
#include "KDNode.hpp"
#endif


#endif
