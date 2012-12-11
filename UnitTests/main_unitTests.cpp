#include "gtest/gtest.h"

#include "BoundingAABB.tests.h"
#include "BoundingSphere.tests.h"
#include "LinearInterploator.tests.h"
#include "BiLinearInterploator.tests.h"
#include "TriLinearInterploator.tests.h"
#include "Field3D.tests.h"
#include "Triangle.tests.h"
#include "mesh.tests.h"


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int res = RUN_ALL_TESTS();
	std::cin.get();
	return res;
}

