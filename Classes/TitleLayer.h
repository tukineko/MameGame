#ifndef __TitleLayer_H__
#define __TitleLayer_H__

#include "Config.h"
#include "GameLayer.h"

USING_NS_CC;

class TitleLayer : public Layer
{
protected:
    enum class mainZOderList {
        BG = 0,
        MAME,
        TITLE,
        SCORE,
        MENU
    };

    Sprite* _title;
    Sprite* _mame01;
    Sprite* _mame02;
    Sprite* _mame03;
    Menu* _menu;
    MenuItemImage* _mItem1;
    SpriteBatchNode* _scoreBatchNode;

public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(TitleLayer);
    virtual void onEnterTransitionDidFinish();

    void nextSceneCallback();
    void viewAnime();
    void viewHiScore(int score);
};

#endif // __TitleLayer_H__
