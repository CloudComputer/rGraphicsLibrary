#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <Base\Object.h>

#include <vector>

#include <boost\iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>

#define KD_TYPE dataType,dimmensions,floatPrecision
#define KD_TEMPLATE template <typename dataType,unsigned int dimmensions,typename floatPrecision>
#define KD_TREE		KDTree<KD_TYPE>
#define KD_NODE		KDNode<KD_TYPE>
#define KD_NODE_IT	KDNodeIterator<KD_TYPE>
#define KD_DATA_IT	KDDataIterator<KD_TYPE>
#define KD_POS_IT	KDPositionIterator<KD_TYPE>

KD_TEMPLATE class KDTree;
KD_TEMPLATE class KDNode;


#include "KDIterator.hpp"


template <typename dataType,unsigned int dimmensions,typename floatPrecision = double> 
class KDNode: public Object{
	friend class KD_TREE;
	friend class KD_NODE_IT;
	friend class KD_DATA_IT;
	friend class KD_POS_IT;
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
	KDTree(const KDTree &tree);
	KDTree& operator=( const KDTree& rhs );
public:
	typedef KD_NODE_IT NodeIterator;
	typedef KD_DATA_IT DataIterator;
	typedef KD_POS_IT PositionIterator;
	
	KDTree();
	virtual ~KDTree();

	void erase(Node *node);
	
	KD_NODE_IT begin();
	KD_NODE_IT end();
	KD_DATA_IT begin_data();
	KD_DATA_IT end_data();
	KD_POS_IT begin_position();
	KD_POS_IT end_position();

	bool isOk()const;
	bool empty()const{return root==0;}

	unsigned long depth()const;
	unsigned long size()const;
	
	std::vector<dataType> getAsVector();

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
