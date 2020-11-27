
#ifndef BEZIER_UTILS
#define BEZIER_UTILS

#include <glm/glm.hpp>

namespace BezierUtils {
	// Evaluate a cubic bezier curve at some point t% of the way along the curve
	glm::vec3 evalBezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);
}

#endif
