#pragma once

#include "gjk_simplex.h"

namespace cs350
{
	glm::vec3 gjk3d_supot_function(const std::vector<glm::vec3>& convex_hull_a, const std::vector<glm::vec3>& convex_hull_b, const glm::vec3& direction);

	bool gjk3d(const std::vector<glm::vec3>& convex_hull_a, const std::vector<glm::vec3>& convex_hull_b, std::vector<cs350::gjk_simplex>& simplices);
}
