#include "../FluidSimulator/Interpolator.h"

TEST (BiLinearInterpolatorTests, vec3){
	BiLinearInterpolator<glm::vec2> l(glm::vec2(0,0),glm::vec2(1,0),glm::vec2(0,1),glm::vec2(1,1));

	EXPECT_EQ(glm::vec2(0.5,0.5),l.interpolate(glm::vec2(.5,.5)));
}
