
#include "BezierUtils.h"

glm::vec3 BezierUtils::evalBezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t) {
    glm::vec3 curvePoint(0.0f);
    float p0c = (1 - t) * (1 - t) * (1 - t);
    float p1c = 3 * (1 - t) * (1 - t) * t;
    float p2c = 3 * (1 - t) * t * t;
    float p3c = t * t * t;
    curvePoint = (p0c * p0) + (p1c * p1) + (p2c * p2) + (p3c * p3);

    return curvePoint;
}
