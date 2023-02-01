/**
 * @file math.hpp
 * @author Eder Beldad (eder.b1@digipen.edu)
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 * @brief Forward declarations, so it they don't need to happen anywhere else
 */
#pragma once

#define GLM_FORCE_INLINE
#define GLM_FORCE_NO_CTOR_INIT
#define GLM_FORCE_EXPLICIT_CTOR
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/integer.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtc/random.hpp>

constexpr float cEpsilon = 1e-6f;
