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
        MENU
    };

    Sprite* _title;
    Sprite* _mame01;
    Sprite* _mame02;
    Sprite* _mame03;
    Menu* _menu;

    MenuItemImage* _mItem1;

public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(TitleLayer);
    virtual void onEnterTransitionDidFinish();
};

#endif // __TitleLayer_H__
