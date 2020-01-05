#ifndef WEIQIGOBAN_H
#define WEIQIGOBAN_H

#include <RController.h>
#include <RPlane.h>
#include <RAudioStream.h>
#include <RSprite.h>
#include <RTextPlane.h>

#include <array>
#include <unordered_map>

class WeiqiGoban : public RController
{
    enum UnitStatus
    {
        Black = -1,
        None,
        White = 1
    };

    enum GameStatus
    {
        Mune,
        Started,
        Finished
    };

public:
    WeiqiGoban(RController *parent, const RShaderProgram &shaders);

    void control() override;

protected:
    void inputEvent(RInputEvent *event) override;

private:
    static constexpr int BLACK_WIN = 184;
    static constexpr int WHITE_WIN = 176;

    void start();
    bool down(int row, int colume); //处理输入
    int compute(int row, int colume, std::shared_ptr<int> qi, UnitStatus current,
                UnitStatus another, std::unordered_map<RPoint2, std::shared_ptr<int>> *record); //计算落子
    std::wstring toCountWstring(int count);

    RPlane goban_;
    std::array<std::array<UnitStatus, 19>, 19> stones_;
    RPlane stoneWhite_;
    RPlane stoneBlack_;
    UnitStatus currentstone_;
    UnitStatus anotherstone_;
    int unit_;
    RRect area_;

    RAudioStream downSound_;
    RAudioStream eatSound_;
    RAudioStream *currentSound_;

    RSprite currentLabel_;
    std::map<UnitStatus, int> count_;

    RPlane menuBackground_;
    RTextPlane finishInfo_;
    RPlane finishBack_;
    RTextPlane startOption_;
    RPlane startBack_;

    GameStatus status_ = Mune;

    _RSLOT_TAIL_
};

#endif // WEIQIGOBAN_H
