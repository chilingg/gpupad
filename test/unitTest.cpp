#include <RDebug.h>
#include <RContext.h>
#include <rsc/RShaderProgram.h>

using namespace Redopera;

// 使用RWindow的场合，初始化与结束由RWindow实例负责

int main()
{
    glm::vec4 vec1 { 25.0f, 50.0f, -5.0f, 1.0f };
    glm::vec4 vec2 { 50.0f, 50.0f, -5.0f, 1.0f };
    glm::vec4 vec3 {  0.0f,  0.0f, -5.0f, 1.0f };
    glm::vec4 vec4 { 25.0f, 25.0f, -5.0f, 1.0f };
    glm::mat4 mat = perspective(0.0f, 50.0f, 0.0f, 50.0f, -5.0f, 50.0f);
    vec1 = mat * vec1;
    vec2 = mat * vec2;
    vec3 = mat * vec3;
    vec4 = mat * vec4;
    rDebug << mat;
    rDebug << vec1.x/vec1.w << vec1.y/vec1.w << vec1.z/vec1.w;
    rDebug << vec2.x/vec2.w << vec2.y/vec2.w << vec2.z/vec2.w;
    rDebug << vec3.x/vec3.w << vec3.y/vec3.w << vec3.z/vec3.w;
    rDebug << vec4.x/vec4.w << vec4.y/vec4.w << vec4.z/vec4.w;

    rDebug << "end";

    //RContext::terminate();
    return 0;
}
