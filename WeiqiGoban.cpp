#include "WeiqiGoban.h"
#include <RPackFile.h>
#include <RResource/RLuaScript.h>

WeiqiGoban::WeiqiGoban(RController *parent, const RShaderProgram &shaders):
    RController("WeiqiBoard", parent),
    goban_(540, 540, "Board", RPoint(0)),
    stones_(), //必需显示默认初始化
    stoneWhite_(24, 24, RTexture(":/image/stone-white.png", "pw"), shaders, "stoneWhite", RPoint(0)),
    stoneBlack_(24, 24, RTexture(":/image/stone-black.png", "pb"), shaders, "stoneBlack", RPoint(0)),
    currentstone_(Black),
    anotherstone_(White),
    downSound_(RMp3(":/music/down.mp3", "Sound-Down")),
    eatSound_(RMp3(":/music/eat.mp3", "Sound-Eat")),
    currentLabel_(54, 10, "WhiteLabel", RPoint(540-54)),
    count_(),
    menuBackground_(540, 540, "MenuBackground", RPoint(0)),
    finishInfo_(),
    finishBack_(540, 40, "FinishBack", RPoint(0, 262)),
    startOption_(L"开局", 108, 40, "StartOption", RPoint(216, 95)),
    startBack_(108, 40, "StartBack", RPoint(216, 92))
{
    RPackFile pack("sources");
#ifdef R_DEBUG
    pack.packing("/home/carper/Code/Redopera/Redopera/Resource/image/board.png");
    pack.packing("../Test.lua");
    if(!pack.fileCheck("sources"))
        pack.save();
#endif

    RImage img;
    auto info = pack.getFileInfo("/home/carper/Code/Redopera/Redopera/Resource/image/board.png");
    img.load(info->data.get(), info->size, true);
    goban_.setTexture(img);
    goban_.setShaderProgram(shaders, "model");

    unit_ = goban_.width() / 20;
    area_ = RRect(goban_.x() + unit_/2, goban_.y() + unit_/2, goban_.width() - unit_, goban_.height() - unit_);

    currentLabel_.addFrame(RTexture(":/image/w-label.png", "wl"));
    currentLabel_.addFrame(RTexture(":/image/b-label.png", "wl"));
    currentLabel_.setShaderProgram(shaders, "model");
    currentLabel_.flipV();

    count_.emplace(Black, 0);
    count_.emplace(White, 0);

    menuBackground_.setColorTexture(150, 150, 130, 60);
    finishBack_.setColorTexture(120, 68, 33);
    startOption_.setBackgroundColor(160, 90, 44);
    startBack_.setColorTexture(120, 68, 33);

    finishInfo_.setSize(finishBack_.size());
    finishInfo_.setPosition(finishBack_.x(), finishBack_.y()+3);
    finishInfo_.rename("FinishInfo");

    RFont font(":/fonts/SourceHanSerifSC_EL-M/SourceHanSerifSC-Medium.otf", "SourceSerif");
    finishInfo_.setFont(font);
    finishInfo_.setAlignment(RTextPlane::Align_Mind, RTextPlane::Align_Mind);
    finishInfo_.setWordSpacing(1.2f);
    startOption_.setFont(font);
    startOption_.setAlignment(RTextPlane::Align_Mind, RTextPlane::Align_Mind);
    startOption_.setWordSpacing(1.2f);

    menuBackground_.setShaderProgram(shaders, "model");
    finishInfo_.setShaderProgram(shaders, "model");
    finishBack_.setShaderProgram(shaders, "model");
    startOption_.setShaderProgram(shaders, "model");
    startBack_.setShaderProgram(shaders, "model");

    info = pack.getFileInfo("../Test.lua");
    RLuaScript tScript(info->data.get(), info->size, "TestScript");
    RDebug() << info->size;
    if(tScript.isValid())
    {
        RDebug() << "Call Test Function is "
                 << (tScript.call("testFunc", {20, 2, 3}, {"Stack Size"}, 2) ? "success. Result is " + std::to_string(tScript.getInteger(-2))
                                                         : "failure");
        RDebug() << tScript.getString() << tScript.stackSize();
        tScript.setStackSize(0);
    }
}

void WeiqiGoban::control()
{
    goban_.render();
    for(unsigned i = 0; i < 19; ++i)
    {
        for(unsigned j = 0; j < 19; ++j)
        {
            if(stones_[i][j] == Black)
            {
                stoneBlack_.setPosition(goban_.x()+j*unit_+15, goban_.y()+(18-i)*unit_+15);
                stoneBlack_.render();
            }
            else if(stones_[i][j] == White)
            {
                stoneWhite_.setPosition(goban_.x()+j*unit_+15, goban_.y()+(18-i)*unit_+15);
                stoneWhite_.render();
            }
        }
    }
    currentLabel_.render();

    if(status_ == Mune)
    {
        menuBackground_.render();
        startBack_.render();
        startOption_.render();
    }
    if(status_ == Finished)
    {
        menuBackground_.render();
        finishBack_.render();
        finishInfo_.render();
    }
}

void WeiqiGoban::inputEvent(RInputEvent *event)
{
    if(status_ == Started)
    {
        if(event->press(RInputModule::MOUSE_BUTTON_LEFT))
        {
            if(area_.contains(event->cursorPos()))
            {
                int row = ((event->cursorPos().y()+unit_/2) / unit_ - 1);
                int colume = ((event->cursorPos().x()+unit_/2) / unit_ - 1);
                if(down(row, colume))
                {
                    ++count_[currentstone_];
                    std::swap(currentstone_, anotherstone_);
                    currentLabel_.setFrame(currentstone_ == Black ? 1 : 0);
                }

                if(count_[Black] > BLACK_WIN)
                {
                    status_ = Finished;
                    finishInfo_.setBackgroundColor(51, 51, 51);
                    finishInfo_.setFontColor(242, 242, 242);
                    finishInfo_.setTexts(L"黑" + toCountWstring(count_[Black]) + L"目 --- 胜 --- 白"
                                         + toCountWstring(count_[White]) + L"目");
                }
                else if(count_[White] > WHITE_WIN)
                {
                    status_ = Finished;
                    finishInfo_.setBackgroundColor(242, 242, 242);
                    finishInfo_.setFontColor(51, 51, 51);
                    finishInfo_.setTexts(L"白" + toCountWstring(count_[White]) + L"目 --- 胜 --- 黑"
                                         + toCountWstring(count_[Black]) + L"目");
                }
            }
        }

        if(event->press(RInputModule::MOUSE_BUTTON_RIGHT))
        {
            status_ = Mune;
        }
        if(event->press(RInputModule::MOUSE_BUTTON_MIDDLE))
        {
            if(count_[Black] - 8 > count_[White])
            {
                status_ = Finished;
                finishInfo_.setBackgroundColor(51, 51, 51);
                finishInfo_.setFontColor(242, 242, 242);
                finishInfo_.setTexts(L"黑" + toCountWstring(count_[Black]) + L"目 --- 胜 --- 白"
                                     + toCountWstring(count_[White]) + L"目");
            }
            else
            {
                status_ = Finished;
                finishInfo_.setBackgroundColor(242, 242, 242);
                finishInfo_.setFontColor(51, 51, 51);
                finishInfo_.setTexts(L"白" + toCountWstring(count_[White]) + L"目 --- 胜 --- 黑"
                                     + toCountWstring(count_[Black]) + L"目");
            }
        }

        return;
    }
    else if(status_ == Mune)
    {
        if(event->press(RInputModule::MOUSE_BUTTON_LEFT))
        {
            if(startOption_.getPlaneRect().contains(event->cursorPos().MirrorV(270)))
                start();
            status_ = Started;
        }
    }
    else if(status_ == Finished)
    {
        if(event->press(RInputModule::MOUSE_BUTTON_LEFT))
        {
            status_ = Started;
        }
    }
}

void WeiqiGoban::start()
{
    stones_ = std::array<std::array<UnitStatus, 19>, 19>();
    currentstone_ = Black;
    anotherstone_ = White;
    count_[Black] = 0;
    count_[White] = 0;
    status_ = Started;
}

bool WeiqiGoban::down(int row, int colume)
{
    if(count_[Black] > BLACK_WIN || count_[White] > WHITE_WIN)
        return false;

    if(stones_[row][colume] == None)
    {
        //计算落子的效益
        stones_[row][colume] = currentstone_;
        std::unordered_map<RPoint2, std::shared_ptr<int>> record; //递归中记录处理过的棋子
        currentSound_ = &downSound_;
        int qi = compute(row, colume, std::make_shared<int>(0), currentstone_, anotherstone_, &record);
        if(qi == 0)
            stones_[row][colume] = None;
        else {
            for(auto &p : record)
            {
                if(*p.second == 0)
                {
                    --count_[anotherstone_];
                    stones_[p.first.x()][p.first.y()] = None;
                    currentSound_ = &eatSound_;
                }
            }

            if(!currentSound_->isRunning())
                currentSound_->startStream();
            return true;
        }
    }
    return false;
}

int WeiqiGoban::compute(int row, int colume, std::shared_ptr<int> qi, WeiqiGoban::UnitStatus current,
                        WeiqiGoban::UnitStatus another, std::unordered_map<RPoint2, std::shared_ptr<int> > *record)
{
    static RRect area(0, 0, 18, 18);

    if(record->count(RPoint2(row, colume))) //!area.contains(row, colume) ||
        return current == currentstone_ ? 0 : *(*record)[RPoint2(row, colume)];

    record->emplace(RPoint2(row, colume), qi);
    if(area.contains(row-1, colume))
    {
        if(stones_[row-1][colume] == current)
            compute(row-1, colume, qi, current, another, record);
        else if(stones_[row-1][colume] == another && another == anotherstone_)
            //只查询当前方是否吃子
            *qi += compute(row-1, colume, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(stones_[row-1][colume] == None)
            ++*qi;
    }
    if(area.contains(row+1, colume))
    {
        if(stones_[row+1][colume] == current)
            compute(row+1, colume, qi, current, another, record);
        else if(stones_[row+1][colume] == another && another == anotherstone_)
            *qi += compute(row+1, colume, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(stones_[row+1][colume] == None)
            ++*qi;
    }
    if(area.contains(row, colume-1))
    {
        if(stones_[row][colume-1] == current)
            compute(row, colume-1, qi, current, another, record);
        else if(stones_[row][colume-1] == another && another == anotherstone_)
            *qi += compute(row, colume-1, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(stones_[row][colume-1] == None)
            ++*qi;
    }
    if(area.contains(row, colume+1))
    {
        if(stones_[row][colume+1] == current)
            compute(row, colume+1, qi, current, another, record);
        else if(stones_[row][colume+1] == another && another == anotherstone_)
            *qi += compute(row, colume+1, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(stones_[row][colume+1] == None)
            ++*qi;
    }

    return *qi;
}

std::wstring WeiqiGoban::toCountWstring(int count)
{
    std::wstring numbur;
    int n = count / 100;
    if(n)
    {
        numbur += L"一百";
    }

    n = count / 10 % 10;
    switch(n)
    {
    case 0:
        if((n % 10) & (count > 99)) numbur += L"零";
        break;
    case 1:
        numbur += L"一十";
        break;
    case 2:
        numbur += L"二十";
        break;
    case 3:
        numbur += L"三十";
        break;
    case 4:
        numbur += L"四十";
        break;
    case 5:
        numbur += L"五十";
        break;
    case 6:
        numbur += L"六十";
        break;
    case 7:
        numbur += L"七十";
        break;
    case 8:
        numbur += L"八十";
        break;
    case 9:
        numbur += L"九十";
        break;
    }

    n = count % 10;
    switch(n)
    {
    case 0:
        if(count < 10) numbur = L"零";
        break;
    case 1:
        numbur += L"一";
        break;
    case 2:
        numbur += L"二";
        break;
    case 3:
        numbur += L"三";
        break;
    case 4:
        numbur += L"四";
        break;
    case 5:
        numbur += L"五";
        break;
    case 6:
        numbur += L"六";
        break;
    case 7:
        numbur += L"七";
        break;
    case 8:
        numbur += L"八";
        break;
    case 9:
        numbur += L"九";
        break;
    }

    return numbur;
}
