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
        COMBO,
        BTN,
        SCORE,
        TIMEUP,
        RESULT,
        START,
        MENU
    };

    //ボタンの種類
    enum class BtnType {
        OK1 = 0,
        OK2,
        OK3,
        NG
    };

    //ゲームの状態
    enum class GameState {
        DEFAULT = 0,
        COUNTDOWN,
        GAME,
        TIMEUP,
        RESULT,
    };

    //豆の総数
    const int MAME_COUNT = 8;

    Sprite* _btnOK1;
    Sprite* _btnOK2;
    Sprite* _btnOK3;
    Sprite* _btnNG;

    SpriteBatchNode* _scoreBatchNode;
    SpriteBatchNode* _comboBatchNode;

    int _score;
    int _combo;
    float _timer;

    //ゲーム状態を管理
    GameState _state;

public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameLayer);
    void update(float dt);
    //タッチした時に呼び出される関数
    bool onTouchBegan(Touch* touch, Event* event);
    virtual void onEnterTransitionDidFinish();
    
    //初期表示
    void initDisp();
    //制限時間表示
    void viewTimer();
    //スコア表示
    void viewScore();
    //コンボ表示
    void viewCombo();
    //ゲーム開始表示
    void viewGameStart();
    //タイムアップ表示
    void viewTimeUp();
    //ゲームリザルト表示
    void viewResult();
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

    //コンボ数によってポイントの変動して値を返します
    int calcScore(int point);

};

#endif // __GameLayer_H__
