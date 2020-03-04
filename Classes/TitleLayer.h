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
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(TitleLayer);

};

#endif // __TitleLayer_H__
