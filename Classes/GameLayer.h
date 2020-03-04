#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "Config.h"

USING_NS_CC;

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
    
    //初期表示
    void initDisp();
    //ボタン押したとき
    void ClickBtnOK();

    //タッチした時に呼び出される関数
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif // __GameLayer_H__
