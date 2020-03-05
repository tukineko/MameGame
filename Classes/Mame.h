#ifndef __Mame_H__
#define __Mame_H__

#include "Config.h"

USING_NS_CC;

const int MAME_SIZE = 100;

class Mame : public Sprite {
protected:
public:
    enum class mameType {
        OK1 = 0,
        OK2,
        OK3,
        NG,
        COUNT
    };

    enum class State {
        /// 停止中
        STATIC = 0,
        /// 落下中
        FALLING,
        /// 消去中
        DISAPEARING
    };

    Mame();
    ~Mame();
    virtual bool init();
    CREATE_FUNC(Mame);

    static int getSize() {
        return MAME_SIZE;
    }

    /** ステージ上の座標をグリッド上の位置に変換します
     *  @param stagePosition ステージ上の座標
     *  @return グリッド上の位置
     */
    static Vec2 convertToGridSpace(const Vec2& stagePosition);

    /** グリッド上の位置をステージ上の座標に変換します
     *  @param gridPosition グリッド上の位置
     *  @return ステージ上の座標
     */
    static Vec2 convertToStageSpace(const Vec2& gridPosition);

    /** 豆のグリッド上の位置を指定します
    * @param position x, y位置を含んだ二次元ベクトル
    */
    void setMamePosition(const Vec2& position);
    
    /** 豆の位置を、現在のグリッド上の位置に合わせて調整します
     */
    void adjustPosition();

    /** 豆が停止状態かどうかを返します
     *  @return 停止状態かどうか
     */
    bool isStatic() {
        return _state == State::STATIC;
    }

    //スコア点数を返します
    int getMameScore();

    CC_SYNTHESIZE(State, _state, State);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(Vec2, _mamePosition, MamePosition);
    CC_SYNTHESIZE_READONLY(mameType, _mameType, MameType);
};

#endif // __Mame_H__
