#ifndef WEIQIBOARD_H
#define WEIQIBOARD_H

#include <RController.h>
#include <RPlane.h>
#include <RAudioStream.h>
#include <array>
#include <unordered_map>

class WeiqiBoard : public RController
{
    enum UnitStatu
    {
        Black = -1,
        None,
        White = 1
    };
public:
    WeiqiBoard(RController *parent, const RShaderProgram &shaders);

    void control() override;
    void translation(const TranslationInfo &info) override;

protected:
    void inputEvent(RInputEvent *event) override;

private:
    bool down(int row, int colume); //处理输入
    int compute(int row, int colume, std::shared_ptr<int> qi, UnitStatu current,
                UnitStatu another, std::unordered_map<RPoint2, std::shared_ptr<int>> *record); //计算落子

    RPlane board_;
    std::array<std::array<UnitStatu, 19>, 19> pieces_;
    RPlane pieceWhite_;
    RPlane pieceBlack_;
    UnitStatu currentPiece_;
    UnitStatu anotherPiece_;
    int unit_;
    RRect area_;

    RAudioStream downSound_;
    RAudioStream eatSound_;
    RAudioStream *currentSound_;
};

#endif // WEIQIBOARD_H
