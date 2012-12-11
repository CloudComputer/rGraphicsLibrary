#include "../FluidSimulator/Mesh.h"


TEST (Triangle, distances){
	Triangle t;
	t.v0 = new Vertex();
	t.v1 = new Vertex();
	t.v2 = new Vertex();

	t.v0->position = glm::vec3(0,0,0);
	t.v1->position = glm::vec3(1,0,0);
	t.v2->position = glm::vec3(0,1,0);

	EXPECT_EQ(0,t.distance(glm::vec3(0,0,0))); //region 0
	EXPECT_EQ(0,t.distance(glm::vec3(1,0,0))); //region 0
	EXPECT_EQ(0,t.distance(glm::vec3(0,1,0))); //region 0
	EXPECT_EQ(0.5,t.distance(glm::vec3(0.3,0.3,0.5))); //region 0

	EXPECT_NEAR(sqrt(0.5),t.distance(glm::vec3(1,1,0)),0.01) << "Region 1"; //region 1

	EXPECT_NEAR(sqrt(5.0),t.distance(glm::vec3(-1,3,0)),0.01) << "Region 2"; //region 2

	EXPECT_EQ(0.5,t.distance(glm::vec3(-0.5,0.5,0))) << "Region 3"; //region 3

	EXPECT_NEAR(sqrt(0.5),t.distance(glm::vec3(-0.5,-0.5,0)),0.01) << "Region 4"; //region 4

	EXPECT_EQ(0.5,t.distance(glm::vec3(0.5,-0.5,0))) << "Region 5"; //region 5

	EXPECT_NEAR(sqrt(1.01),t.distance(glm::vec3(2,-0.1,0)),0.01) << "Region 6"; //region 6

}


TEST (Triangle, signedDistances){
	Triangle t;
	t.v0 = new Vertex();
	t.v1 = new Vertex();
	t.v2 = new Vertex();

	t.v0->position = glm::vec3(0,0,0);
	t.v1->position = glm::vec3(1,0,0);
	t.v2->position = glm::vec3(0,1,0);

	EXPECT_EQ(0.5,t.distance(glm::vec3(0.3,0.3,-0.5))); //region 0
	EXPECT_EQ(0.5,t.distance(glm::vec3(0.3,0.3,0.5))); //region 0

	
}


TEST (Triangle,calculateNormals){
	Triangle t(new Vertex(),new Vertex(),new Vertex());

	t.v0->position = glm::vec3(0,0,0);
	t.v1->position = glm::vec3(1,0,0);
	t.v2->position = glm::vec3(0,0,-1);
	t.calculateNormal();
	

	EXPECT_EQ(t.normal,glm::vec3(0,1,0));

}