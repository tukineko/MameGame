#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "Config.h"
#include "Mame.h"

USING_NS_CC;

typedef cocos2d::Vector<Mame*> MameVector;

class GameLayer : public Layer
{
protected:
    enum class mainZOderList {
        BG = 0,
        FLOOR,
        MAME,
        HITEREA,
        BTN,
        MENU
    };

    enum class BtnType {
        OK1 = 0,
        OK2,
        OK3,
        NG
    };

    const int MAME_COUNT = 8;

    Sprite* _btnOK1;
    bool _btnOK1_state = false;
    Sprite* _btnOK2;
    bool _btnOK2_state = false;
    Sprite* _btnOK3;
    bool _btnOK3_state = false;
    Sprite* _btnNG;
    bool _btnNG_state = false;

public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameLayer);
    void update(float dt);
    //タッチした時に呼び出される関数
    bool onTouchBegan(Touch* touch, Event* event);
    
    //初期表示
    void initDisp();
    //ボタン押したとき
    void ClickBtn(BtnType btn_type);
    //豆を設置するステージ
    CC_SYNTHESIZE_RETAIN(Node*, _stage, Stage);
    //豆配列を準備
    CC_SYNTHESIZE_PASS_BY_REF(MameVector, _mames, Mames);

    /** グリッド上の特定位置にある豆を取り出します
    *   何もなかった場合はnullptrを返します
     *  @param position グリッド上の豆位置
     *  @return その位置にあるMame、またはnullptr
    */
    Mame* getMameAt(const Vec2& position);

    //豆の削除
    void deleteMame(Mame::mameType mame_type);

    /** 渡された豆が落ちるかどうかを判定し、落ちる場合は落下させます
     *  @param mame チェックする豆
     *  @return 落ちたかどうか
     */
    bool fallMame(Mame* mame);

    /** 豆をグリッド上の指定した位置に動かします
     *  @param mame 動かすブロック
     *  @param mamePosition 動かすグリッド上の座標
     */
    void moveMame(Mame* mame, const Vec2& mamePosition);

    /** ステージをチェックして出現できる場所に豆を出現させ、出現した豆の一覧を返します
     *  出現しなかった場合、空のベクターを返します
     *  @return 出現した豆の一覧
     */
    Vector<Mame*> checkSpawn();

};

#endif // __GameLayer_H__
