#include <RDebug.h>
#include <RContext.h>
#include <rsc/RShaderProgram.h>

using namespace Redopera;

// 使用RWindow的场合，初始化与结束由RWindow实例负责

const char *code =
        "#version 430 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec2 aTexCoor;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "out vec2 TexCoor;\n"
        "void main(void)\n"
        "{\n"
        "gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "TexCoor = aTexCoor;\n"
        "}\n";

int main()
{
    RContext::ContexFormat format;
    format.debug = true;
    if(check(!RContext::initialization() || !RContext::setContexAsThisThread(format), "Failure initialization OpenGL context!"))
        exit(EXIT_FAILURE);

    std::cout << "OpenGl Version:" << GLVersion.major << '.' << GLVersion.minor << std::endl;

    RShader shader(code, ShaderType::VertexShader);
    check(!shader.isValid(), "Failure load shader!");

    RShaderProgram program({ shader });
    check(!program.isValid(), "Failure link shader program!");

    rDebug << "end";

    //RContext::terminate();
    return 0;
}
