#include <RDebug.h>
#include <resource/RFont.h>

using namespace Redopera;

int main()
{
    RFont font("/usr/share/fonts/SourceHanSansSC/SourceHanSansSC-Bold.otf");

    rDebug << font.isValid() << font.size();

    return 0;
}
