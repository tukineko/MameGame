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
    
    this->initDisp();

    
    //タッチイベントの設定
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GameLayer::initDisp() {
    //背景
    auto bg = LayerColor::create(Color4B::WHITE, winSizeW, winSizeH);
    this->addChild(bg, (int)mainZOderList::BG);

    //床
    auto floor = Sprite::create("game/floor.png");
    floor->setPosition(Vec2(winSizeCenterW, winSizeCenterH + 50));
    this->addChild(floor, (int)mainZOderList::FLOOR);

    //目印
    auto hitarea = Sprite::create("game/hit_area.png");
    hitarea->setPosition(Vec2(winSizeCenterW, winSizeCenterH + 50));
    this->addChild(hitarea, (int)mainZOderList::HITEREA);

    //OKボタン1
    _btnOK1 = Sprite::create("game/btnOK1.png");
    _btnOK1->setPosition(Vec2(winSizeCenterW - 400, 130));
    this->addChild(_btnOK1, (int)mainZOderList::BTN);

    //OKボタン2
    _btnOK2 = Sprite::create("game/btnOK2.png");
    _btnOK2->setPosition(Vec2(winSizeCenterW - 130, 130));
    this->addChild(_btnOK2, (int)mainZOderList::BTN);

    //OKボタン3
    _btnOK3 = Sprite::create("game/btnOK3.png");
    _btnOK3->setPosition(Vec2(winSizeCenterW + 130, 130));
    this->addChild(_btnOK3, (int)mainZOderList::BTN);

    //NGボタン
    _btnNG = Sprite::create("game/btnNG.png");
    _btnNG->setPosition(Vec2(winSizeCenterW + 400, 130));
    this->addChild(_btnNG, (int)mainZOderList::BTN);

}

bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    Vec2 location = touch->getLocation();

    if (_btnOK1->getBoundingBox().containsPoint(location) && !_btnOK1_state) {
        this->ClickBtnOK();
    }

    return false;
}

void GameLayer::ClickBtnOK() {
    _btnOK1_state = true;
    auto ac = Sequence::create(
        EaseElasticOut::create(ScaleTo::create(0.3f, 1.1f)),
        ScaleTo::create(0, 1.0f),
        CallFunc::create([&]() {
        _btnOK1_state = false;
    }),
        nullptr
        );
    _btnOK1->runAction(ac);
}