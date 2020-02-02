#include <RDebug.h>
#include <resource/RFont.h>

using namespace Redopera;

int main()
{
    RFont font;
    font.setSize(22);

    rDebug << "========字体图形加载测试========\n"
              "默认字体:SourceCodePro 字体大小:" << font.size();

    char c;
    do {
        std::cout << "\n输入英文字符 ('q'退出): ";
        std::cin >> c;
        RFont::Glyph *glyph = font.getFontGlyph(c);
        rDebug << "\n字符:" << c << " w:" << glyph->width << " h:" << glyph->height
               << " xoff:" << glyph->xoff << " yoff" << glyph->yoff << '\n';

        for (int j = 0; j < glyph->height; ++j)
        {
           for (int i = 0; i < glyph->width; ++i)
               std::cout << " ':0IM%@"[glyph->data[j*glyph->width+i] >> 5] << ' ';
           std::cout << '\n';
        }
        std::cout  << std::endl;
    } while(c != L'q');

    return 0;
}
