#pragma execution_character_set("utf-8")
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

Scene* GameLayer::createScene()
{
    Scene* scene = Scene::create();
    GameLayer* layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

bool GameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}
