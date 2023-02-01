#include "Helpers/pch.hpp"
#include "gjk3d.h"
#include "gjk_simplex.h"


glm::vec3 cs350::gjk3d_supot_function(const std::vector<glm::vec3>& convex_hull_a, const std::vector<glm::vec3>& convex_hull_b, const glm::vec3& direction)
{
	glm::vec3 nearest_point_a {};
	glm::vec3 nearest_point_b {};

	float dot_a = -std::numeric_limits<float>::max();
	float dot_b = -std::numeric_limits<float>::max();

	for (auto& it : convex_hull_a)
	{
		float actual_dot = glm::dot(it, direction);
		if (actual_dot > dot_a)
		{
			dot_a = actual_dot;
			nearest_point_a = it;
		}
	}

	glm::vec3 b_direcion = -direction;
	for (auto& it : convex_hull_b)
	{
		float actual_dot = glm::dot(it, b_direcion);
		if (actual_dot > dot_b)
		{
			dot_b = actual_dot;
			nearest_point_b = it;
		}
	}


	return nearest_point_a - nearest_point_b;
}

bool cs350::gjk3d(const std::vector<glm::vec3>& convex_hull_a, const std::vector<glm::vec3>& convex_hull_b, std::vector<cs350::gjk_simplex>& simplices)
{
	simplices.clear();
	//origin 
	glm::vec3 origin{ 0.0f, 0.0f, 0.0f };
	//arbitrary point
	//Initialize the simplex with a random point
	glm::vec3 arbitrary = convex_hull_a[0] - convex_hull_b[0];

	//create and add the point
	gjk_simplex progres;
	progres.add_point(arbitrary);
	simplices.push_back(progres);

	glm::vec3 old_point = arbitrary;

	while (true)
	{
		//find the closest point
		glm::vec3 closest_point = progres.iterate();

		//If that point is the origin stop, origin contained, objects intersect
		if (closest_point == origin)
			return true;

		//This point will be a convex combination of the points of the simplex.Discard the
		//	points that do not contribute to retrieve w
		progres.reduce();

		//get the clossest new point
		//Find the support point of the Minkowski difference in the direction towards the
		//	origin from w
		glm::vec3 new_point = gjk3d_supot_function(convex_hull_a, convex_hull_b, -closest_point);

		//If that point is not closer to the origin, origin is out of reach
		//we check if the point given is one the we already got
		const std::array<glm::vec3, 4>& points = progres.points();
		unsigned size = progres.point_count();
		for (unsigned i = 0; i < size; i++)
		{
			if (new_point == points[i])
				return false;
		}
		//we check if the point is the one of the last loop, in case we put it back and delete all time
		if (new_point == old_point)
			return false;
		else
		{
			old_point = new_point;
		}

		//Otherwise add that point to the simplexand go to step 2
		progres.add_point(new_point);
		simplices.push_back(progres);
	}




	return false;
}
