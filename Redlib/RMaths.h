#ifndef RMATHS_H
#define RMATHS_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RMath = glm;

/* GLM算法和功能
 * min(genType x, genType y)   //返回最小值
 * max(genType x, genType y)   //返回最大值
 * abs(int x)                  //返回绝对值
 * round(genType x)            //四舍六入五取偶
 * trunc(genType x)            //返回截断整数部分
 */

using RMatrix4 = RMath::mat4;
using RVector4 = RMath::vec4;
using RIVector4 = RMath::ivec4;
using RVector3 = RMath::vec3;

#endif // RMATHS_H
