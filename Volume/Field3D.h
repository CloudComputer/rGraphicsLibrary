#ifndef __FIELD3D_H__
#define __FIELD3D_H__

#define FOR(dim) for(int z = 0;z<dim.z;z++)for(int y = 0;y<dim.y;y++)for(int x = 0;x<dim.x;x++)
#define INDEX(vf,x,y,z) x+vf->getDimensions()[0]*(y+vf->getDimensions()[1]*z)

#include <glm/glm.hpp>

#include <Geometry/BoundingGeometry/BoundingVolume.h>

template <typename T>
class Field3D
{
	friend class UltrasoundVariationalClassification;
public:
	Field3D(glm::ivec3 dimensions = glm::ivec3(64,64,64),BoundingAABB boundingAABB = BoundingAABB(glm::vec3(0,0,0),glm::vec3(1,1,1)));
	virtual ~Field3D(void);

	void foreach(T (*funcPtr)(T,glm::vec3));
	void foreach(void (*funcPtr)(T&,glm::vec3));

	void set(const glm::ivec3 &pos,const T &value);
	T get(const glm::ivec3 &pos)const;
	T getFromWorldPos(const glm::vec3 &worldPos)const;
	glm::ivec3 getDimensions()const;
	BoundingAABB getBoundingAABB()const;

	void setZero();

	T getMin()const;
	T getMax()const;
	void normalize();

	T* getData()const;
	unsigned int getMemSize()const;
	
	
	static Field3D *Add(const Field3D *f0,const Field3D *f1);
	static Field3D *Sub(const Field3D *f0,const Field3D *f1);
	static Field3D *Mult(const Field3D *f0,const Field3D *f1);
	static Field3D *Div(const Field3D *f0,const Field3D *f1);

	static Field3D *Max(const Field3D *f0,const Field3D *f1);
	static Field3D *Min(const Field3D *f0,const Field3D *f1);

	static Field3D *Add(const Field3D *f0,const float &f1);
	static Field3D *Sub(const Field3D *f0,const float &f1);
	static Field3D *Mult(const Field3D *f0,const float &f1);
	static Field3D *Div(const Field3D *f0,const float &f1);

protected:
	unsigned int _index(glm::ivec3 pos,bool clamp = true)const;
	glm::vec3 _getWorldPos(const glm::ivec3 &pos)const;

	glm::vec3 _delta;
	glm::ivec3 _dimensions;
	BoundingAABB _boundingAABB;
	T *_data;
	
	
private:
	void _allocate();
	void _deallocate();
};


#ifndef __FIELD3D_HPP__
#include "Field3D.hpp"
#endif

#endif