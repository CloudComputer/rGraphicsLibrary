#include "../FluidSimulator/Field3D.h"


float TestFuncForeachNonPointers(float input,glm::vec3 worldPos){
	return input * 2;
}

void TestFuncForeachPointers(float &input,glm::vec3 worldPos){
	input *= 2;
}

TEST (Field3D, foreachNonPointers){
	Field3D<float> field(glm::ivec3(2,2,2));
	field.set(glm::ivec3(0,0,0),0);
	field.set(glm::ivec3(0,0,1),1);
	field.set(glm::ivec3(0,1,0),2);
	field.set(glm::ivec3(0,1,1),3);
	field.set(glm::ivec3(1,0,0),4);
	field.set(glm::ivec3(1,0,1),5);
	field.set(glm::ivec3(1,1,0),6);
	field.set(glm::ivec3(1,1,1),7);
	field.foreach(TestFuncForeachNonPointers);
	EXPECT_EQ(0,field.get(glm::ivec3(0,0,0)));
	EXPECT_EQ(2,field.get(glm::ivec3(0,0,1)));
	EXPECT_EQ(4,field.get(glm::ivec3(0,1,0)));
	EXPECT_EQ(6,field.get(glm::ivec3(0,1,1)));
	EXPECT_EQ(8,field.get(glm::ivec3(1,0,0)));
	EXPECT_EQ(10,field.get(glm::ivec3(1,0,1)));
	EXPECT_EQ(12,field.get(glm::ivec3(1,1,0)));
	EXPECT_EQ(14,field.get(glm::ivec3(1,1,1)));
}


TEST (Field3D, foreachPointers){
	Field3D<float> field(glm::ivec3(2,2,2));
	field.set(glm::ivec3(0,0,0),0);
	field.set(glm::ivec3(0,0,1),1);
	field.set(glm::ivec3(0,1,0),2);
	field.set(glm::ivec3(0,1,1),3);
	field.set(glm::ivec3(1,0,0),4);
	field.set(glm::ivec3(1,0,1),5);
	field.set(glm::ivec3(1,1,0),6);
	field.set(glm::ivec3(1,1,1),7);
	field.foreach(TestFuncForeachPointers);
	EXPECT_EQ(0,field.get(glm::ivec3(0,0,0)));
	EXPECT_EQ(2,field.get(glm::ivec3(0,0,1)));
	EXPECT_EQ(4,field.get(glm::ivec3(0,1,0)));
	EXPECT_EQ(6,field.get(glm::ivec3(0,1,1)));
	EXPECT_EQ(8,field.get(glm::ivec3(1,0,0)));
	EXPECT_EQ(10,field.get(glm::ivec3(1,0,1)));
	EXPECT_EQ(12,field.get(glm::ivec3(1,1,0)));
	EXPECT_EQ(14,field.get(glm::ivec3(1,1,1)));
}

TEST(Field3D,getFromWorldPos){
	Field3D<float> field(glm::ivec3(2,2,2));
	field.set(glm::ivec3(0,0,0),0);
	field.set(glm::ivec3(0,0,1),1);
	field.set(glm::ivec3(0,1,0),2);
	field.set(glm::ivec3(0,1,1),3);
	field.set(glm::ivec3(1,0,0),4);
	field.set(glm::ivec3(1,0,1),5);
	field.set(glm::ivec3(1,1,0),6);
	field.set(glm::ivec3(1,1,1),7);



	EXPECT_EQ(field.getFromWorldPos(glm::vec3(.5,.5,.5)),3.5);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(0,0,0)),0);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(0,0,1)),1);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(0,1,0)),2);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(0,1,1)),3);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(1,0,0)),4);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(1,0,1)),5);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(1,1,0)),6);
	EXPECT_EQ(field.getFromWorldPos(glm::vec3(1,1,1)),7);


	for(float a = 0;a<=1;a+=0.1f){
		EXPECT_NEAR(field.getFromWorldPos(glm::vec3(a,a,a)),a*7,0.001);
	}




	Field3D<float> field2(glm::ivec3(2,2,2),BoundingAABB(glm::vec3(-1,-1,-1),glm::vec3(1,1,1)));
	field2.set(glm::ivec3(-1,-1,-1),0);
	field2.set(glm::ivec3(-1,-1,1),1);
	field2.set(glm::ivec3(-1,1,-1),2);
	field2.set(glm::ivec3(-1,1,1),3);
	field2.set(glm::ivec3(1,-1,-1),4);
	field2.set(glm::ivec3(1,-1,1),5);
	field2.set(glm::ivec3(1,1,-1),6);
	field2.set(glm::ivec3(1,1,1),7);

	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(0,0,0)),3.5);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(-1,-1,-1)),0);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(-1,-1,1)),1);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(-1,1,-1)),2);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(-1,1,1)),3);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(1,-1,-1)),4);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(1,-1,1)),5);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(1,1,-1)),6);
	EXPECT_EQ(field2.getFromWorldPos(glm::vec3(1,1,1)),7);
}