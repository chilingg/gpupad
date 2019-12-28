#include "WeiqiBoard.h"

WeiqiBoard::WeiqiBoard(RController *parent, const RShaderProgram &shaders):
    RController("WeiqiBoard", parent),
    board_(540, 540, "Board", RPoint(0)),
    pieces_(), //必需显示默认初始化
    pieceWhite_(24, 24, RTexture(":/image/piece-white.png", "pw"), shaders, "PieceWhite", RPoint(0)),
    pieceBlack_(24, 24, RTexture(":/image/piece-black.png", "pb"), shaders, "PieceBlack", RPoint(0)),
    currentPiece_(Black),
    anotherPiece_(White),
    downSound_(RMp3(":/music/down.mp3", "Sound-Down")),
    eatSound_(RMp3(":/music/eat.mp3", "Sound-Eat"))
{
    board_.setTexture(RImage(":/image/board.png", "BoardImg", true));
    board_.setShaderProgram(shaders, "model");

    unit_ = board_.width() / 20;
    area_ = RRect(board_.x() + unit_/2, board_.y() + unit_/2, board_.width() - unit_, board_.height() - unit_);
}

void WeiqiBoard::control()
{
    board_.render();
    for(unsigned i = 0; i < 19; ++i)
    {
        for(unsigned j = 0; j < 19; ++j)
        {
            if(pieces_[i][j] == Black)
            {
                pieceBlack_.setPosition(j*unit_+15, (18-i)*unit_+15);
                pieceBlack_.render();
            }
            else if(pieces_[i][j] == White)
            {
                pieceWhite_.setPosition(j*unit_+15, (18-i)*unit_+15);
                pieceWhite_.render();
            }
        }
    }
}

void WeiqiBoard::translation(const RController::TranslationInfo &info)
{
    board_.setSize(info.size);
    unit_ = board_.width() / 20;
    area_ = RRect(board_.x() + unit_/2, board_.y() + unit_/2, board_.width() - unit_, board_.height() - unit_);
}

void WeiqiBoard::inputEvent(RInputEvent *event)
{
    if(event->press(RInputModule::MOUSE_BUTTON_LEFT))
    {
        if(area_.contains(event->cursorPos()))
        {
            int row = ((event->cursorPos().y()+unit_/2) / unit_ - 1);
            int colume = ((event->cursorPos().x()+unit_/2) / unit_ - 1);
            if(down(row, colume))
                std::swap(currentPiece_, anotherPiece_);
        }
    }
}

bool WeiqiBoard::down(int row, int colume)
{
    if(pieces_[row][colume] == None)
    {
        //计算落子的效益
        pieces_[row][colume] = currentPiece_;
        std::unordered_map<RPoint2, std::shared_ptr<int>> record; //递归中记录处理过的棋子
        currentSound_ = &downSound_;
        int qi = compute(row, colume, std::make_shared<int>(0), currentPiece_, anotherPiece_, &record);
        if(qi == 0)
            pieces_[row][colume] = None;
        else {
            for(auto &p : record)
            {
                if(*p.second == 0)
                {
                    pieces_[p.first.x()][p.first.y()] = None;
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

int WeiqiBoard::compute(int row, int colume, std::shared_ptr<int> qi, WeiqiBoard::UnitStatu current,
                        WeiqiBoard::UnitStatu another, std::unordered_map<RPoint2, std::shared_ptr<int> > *record)
{
    static RRect area(0, 0, 18, 18);

    if(record->count(RPoint2(row, colume))) //!area.contains(row, colume) ||
        return current == currentPiece_ ? 0 : *(*record)[RPoint2(row, colume)];

    record->emplace(RPoint2(row, colume), qi);
    if(area.contains(row-1, colume))
    {
        if(pieces_[row-1][colume] == current)
            compute(row-1, colume, qi, current, another, record);
        else if(pieces_[row-1][colume] == another && another == anotherPiece_)
            //只查询当前方是否吃子
            *qi += compute(row-1, colume, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(pieces_[row-1][colume] == None)
            ++*qi;
    }
    if(area.contains(row+1, colume))
    {
        if(pieces_[row+1][colume] == current)
            compute(row+1, colume, qi, current, another, record);
        else if(pieces_[row+1][colume] == another && another == anotherPiece_)
            *qi += compute(row+1, colume, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(pieces_[row+1][colume] == None)
            ++*qi;
    }
    if(area.contains(row, colume-1))
    {
        if(pieces_[row][colume-1] == current)
            compute(row, colume-1, qi, current, another, record);
        else if(pieces_[row][colume-1] == another && another == anotherPiece_)
            *qi += compute(row, colume-1, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(pieces_[row][colume-1] == None)
            ++*qi;
    }
    if(area.contains(row, colume+1))
    {
        if(pieces_[row][colume+1] == current)
            compute(row, colume+1, qi, current, another, record);
        else if(pieces_[row][colume+1] == another && another == anotherPiece_)
            *qi += compute(row, colume+1, std::make_shared<int>(0), another, current, record) == 0 ? 1 : 0;
        else if(pieces_[row][colume+1] == None)
            ++*qi;
    }

    return *qi;
}
