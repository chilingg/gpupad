//#define MINIMP3_ONLY_SIMD
//#define MINIMP3_NO_SIMD
//#define MINIMP3_NONSTANDARD_BUT_LOGICAL
//#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_ONLY_MP3
#define MINIMP3_IMPLEMENTATION
#include "dependents/minimp3.h"
#include "dependents/minimp3_ex.h"

#define STB_IMAGE_IMPLEMENTATION
#include "dependents/stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "dependents/stb_truetype.h"

#include "RMath.h"

void* Redopera::bufOff(size_t off) { return reinterpret_cast<void*>(off); }

glm::mat4 Redopera::perspective(float left, float right, float buttom, float top, float near, float far)
{
    static float accuracy = 0.95; // 投影之后far平面拥有的最大z轴标量

    glm::mat4 mat(1);
    float len = far - near;

    // 缩放
    mat[0][0] = 1 / (right - left) * 2;
    mat[1][1] = 1 / (top - buttom) * 2;
    // z轴翻转与投影
    mat[2][3] = -1 / ((2*accuracy*len - 2*len) / (1+accuracy));
    mat[2][2] = mat[2][3];
    // 左乘以平移矩阵
    mat[3][0] = mat[0][0] * -(left + (right - left) / 2);
    mat[3][1] = mat[1][1] * -(buttom + (top - buttom) / 2);
    mat[3][2] = mat[2][2] * -near;
    mat[3][3] = mat[3][2] + 1;

    return mat;
}
