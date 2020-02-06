#include <RDebug.h>
#include <RContext.h>
#include <rsc/RTexture.h>

using namespace Redopera;

int main()
{
    if(check(!RContext::initialization() || !RContext::setContexAsThisThread(), "Failure initialization OpenGL context!"))
        exit(EXIT_FAILURE);

    RTexture tex(RImage::redoperaIcon());
    rDebug << tex.isValid();

    return 0;
}
