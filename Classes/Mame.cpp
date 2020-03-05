#pragma execution_character_set("utf-8")
#include "Mame.h"
#include "SimpleAudioEngine.h"

//コンストラクター
//作成されるときに起こる処理
//:以降で数値の初期化をすることが出来る
Mame::Mame() :_state(State::STATIC)
{
}

//デストラクター
//このクラスが終わるときに起こる処理
Mame::~Mame()
{
}

bool Mame::init()
{
    // 乱数発生器の初期化
    std::random_device rdev;
    auto engine = std::mt19937(rdev());
    auto dist = std::uniform_int_distribution<>(0, (int)(Mame::mameType::COUNT) - 1);

    // 豆をランダムに1つ選ぶ
    auto mame = dist(engine);
    _mameType = static_cast<Mame::mameType>(mame);

    if (!Sprite::initWithFile("game/mame.png", Rect(Mame::getSize() * mame, 0, Mame::getSize(), Mame::getSize()))) return false;

    return true;
}

Vec2 Mame::convertToStageSpace(const Vec2& gridPosition)
{
    return std::move((gridPosition + Vec2::ONE * 0.5) * -Mame::getSize());
}

Vec2 Mame::convertToGridSpace(const Vec2& stagePosition)
{
    auto x = floor(stagePosition.x / Mame::getSize());
    auto y = floor(stagePosition.y / Mame::getSize());
    return std::move(Vec2(x, y));
}

void Mame::setMamePosition(const Vec2& position)
{
    _mamePosition = position;
}

void Mame::adjustPosition()
{
    auto position = _mamePosition;
    // _mamePositionを元にpositionを設定する
    this->setPosition(Mame::convertToStageSpace(position));
}

int Mame::getMameScore() {
    switch (_mameType)
    {
    case Mame::mameType::OK1:
        return 100;
        break;
    case Mame::mameType::OK2:
        return 100;
        break;
    case Mame::mameType::OK3:
        return 100;
        break;
    case Mame::mameType::NG:
        return 100;
        break;
    default:
        return 0;
        break;
    }


}