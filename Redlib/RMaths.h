#ifndef RMATHS_H
#define RMATHS_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RMath {
using namespace::glm;

}

using RMatrix4 = RMath::mat4;
using RVector4 = RMath::vec4;
using RIVector4 = RMath::ivec4;
using RVector3 = RMath::vec3;

#endif // RMATHS_H
