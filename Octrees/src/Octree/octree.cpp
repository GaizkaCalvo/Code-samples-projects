#include "Helpers/pch.hpp"
#include "octree.hpp"

namespace cs350 {
	/**
	* @brief: we compute the locational code with the given bounding volume, root size and root level
	* @param  bv: bounding volume
	* @param  root_size: size of the root
	* @param  levels: levels of the root
	* @return: we return the locational code that we create
	*/
	uint32_t compute_locational_code(aabb const& bv, uint32_t root_size, uint32_t levels)
	{
		glm::vec3 min = glm::floor(bv.mMin);
		glm::vec3 max = glm::ceil (bv.mMax);

		uint32_t lc1 = compute_locational_code({ min.x, min.y, min.z }, root_size, levels);
		uint32_t lc2 = compute_locational_code({ max.x, max.y, max.z }, root_size, levels);

		return common_locational_code(lc1, lc2);
	}
	/**
	* @brief: we compute a bounding volume giving the locational code and the size of the root
	* @param  locational_code: locational code were the node is
	* @param  root_size: size of the root
	* @return: we return the bv that we created
	*/
	aabb compute_bv(uint32_t locational_code, uint32_t root_size)
	{
		//initialize the value
		int size = root_size / 2;
		aabb result({ -size, -size ,-size }, { size ,size ,size });

		uint32_t depth = glm::log2(locational_code);

		if (depth == 0)
			return result;

		int it = 1;
		for (int i = depth -1; i >= 0; i--)
		{
			if (((locational_code >> i) & 1) == 1)//has a 1 in it
			{
				//we touch min
				result.mMin[i % 3] += root_size / (uint32_t)glm::pow(2, it);
			}
			else//has a in the position
			{
				//we touch max
				result.mMax[i % 3] -= root_size / (uint32_t)glm::pow(2, it);
			}
			if(i%3 == 0)
				it++;
		}


		return result;
	}
	/**
	* @brief: we compute the depth of the given lc
	* @param  lc: locational code were the node
	* @param  root_size: size of the root
	* @return: we return the depth
	*/
	uint32_t locational_code_depth(uint32_t lc)
	{
		return (glm::log2(lc) / 3);
	}
	/**
	* @brief: we compute the common locational code
	* @param  lc1: locational code were the node
	* @param  lc1: locational code were the node
	* @return: we return the common locational code
	*/
	uint32_t common_locational_code(uint32_t lc1, uint32_t lc2)
	{
		if (lc1 == 0 || lc2 == 0)
		{
			std::cout << "Fatal error: one of the location code is 0 \n";
			return 0;
		}

		if (lc1 == 1 || lc2 == 1)
			return 1;

		int lc1_first_positive = 32;

		int i = glm::log2(lc1);
		int j = glm::log2(lc2);

		if (i <= 1 || j <= 1)
			return 1;

		uint32_t result = 1;
		uint32_t it = 0;

		while (true)
		{
			//update the checks
			i -= 3; j -= 3;
			//see if its the same locational code
			it = (lc1 >> i) & (lc2 >> j);
			if (it != (lc1 >> i) || it != (lc2 >> j))
				return result;
			else
				result = it;
			if(i <= 0 || j <= 0)
				return result;
		}
		//return the result when finished
		return result;
	}
}