#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "Config.h"

USING_NS_CC;

class GameLayer : public Layer
{
protected:
    

public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
};

#endif // __GameLayer_H__
