#ifndef __ResultLayer_H__
#define __ResultLayer_H__

#include "Config.h"
#include "GameLayer.h"

USING_NS_CC;

class ResultLayer : public Layer
{
protected:
    enum class mainZOderList {
        BG = 0,
        KIRA,
        TEXT,
        SCORE
    };

    SpriteBatchNode* _scoreBatchNode;

    Sprite* _mame;
    Sprite* _scorebase;
    Sprite* _kekka;
    Sprite* _score_now;
    Sprite* _titleback;

    bool _recordNew = false;

public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ResultLayer);
    virtual void onEnterTransitionDidFinish();
    bool onTouchBegan(Touch* touch, Event* event);

    void doAnime01();
    void doAnime02();
    void doAnime03();

    //スコア表示
    void viewScore();
    
};

#endif // __ResultLayer_H__
