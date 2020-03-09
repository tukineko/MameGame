#pragma execution_character_set("utf-8")
#include "TitleLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* TitleLayer::createScene()
{
    Scene* scene = Scene::create();
    TitleLayer* layer = TitleLayer::create();
    scene->addChild(layer);
    return scene;
}

bool TitleLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    CCLOG("----------------TitleLayer::init()----------------");

    //音データのプレロード
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se/stamp.mp3");

    //背景
    auto bg = LayerGradient::create(Color4B(50, 150, 200, 255), Color4B(109, 195, 236, 255));
    //auto bg = LayerColor::create(Color4B(109, 195, 236, 255), winSizeW, winSizeH);
    this->addChild(bg, (int)mainZOderList::BG);

    //タイトル
    _title = Sprite::create("title/title.png");
    _title->setPosition(Vec2(winSizeCenterW, winSizeH - 200));
    _title->setOpacity(0);
    this->addChild(_title, (int)mainZOderList::TITLE);
    
    //メニューボタン
    _mItem1 = MenuItemImage::create("title/btn_start.png", "title/btn_start.png", [=](Ref* sender) {
        //SE
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se/stamp.mp3");
        
        //既存のアニメーションを停止
        _mItem1->stopAllActions();
        _mItem1->runAction(
            Sequence::create(
                EaseOut::create(ScaleTo::create(0.1f, 0.4f), 2.0f),
                EaseElasticOut::create(ScaleTo::create(0.25f, 1.0f)),
                CallFunc::create([]() {
                    //ゲーム画面へ
                    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameLayer::createScene(), Color3B::WHITE));
                }),
                nullptr
            )
        );
    });
    _mItem1->setPosition(Vec2(winSizeCenterW, 130));

    //メニューを作成
    _menu = Menu::create(_mItem1, NULL);
    _menu->setPosition(Point::ZERO);
    _menu->setOpacity(0);
    this->addChild(_menu, (int)mainZOderList::MENU);

    //豆中央
    _mame01 = Sprite::create("title/mame01.png");
    _mame01->setPosition(Vec2(winSizeCenterW, winSizeH + 172));
    this->addChild(_mame01, (int)mainZOderList::MAME);

    //豆左
    _mame02 = Sprite::create("title/mame02.png");
    _mame02->setPosition(Vec2(-200, winSizeH - 410));
    this->addChild(_mame02, (int)mainZOderList::MAME);

    //豆右
    _mame03 = Sprite::create("title/mame03.png");
    _mame03->setPosition(Vec2(winSizeW + 180, 238));
    this->addChild(_mame03, (int)mainZOderList::MAME);

    return true;
}

void TitleLayer::onEnterTransitionDidFinish()
{
    _title->runAction(
        Sequence::create(
            DelayTime::create(1.0f),
            Spawn::create(
                FadeIn::create(0.5f),
                MoveBy::create(0.5f, Vec2(0, 10)),
                nullptr
            ),
            nullptr
        )
    );

    _menu->runAction(
        Sequence::create(
            DelayTime::create(1.0f),
            Spawn::create(
                FadeIn::create(0.5f),
                MoveBy::create(0.5f, Vec2(0, 10)),
                nullptr
            ),
            DelayTime::create(0.25f),
            CallFunc::create([=]() {
        //メニューボタンを動かす
        auto ac = Sequence::create(
            ScaleTo::create(0.3f, 1.1f),
            ScaleTo::create(0.3f, 1.0f),
            nullptr
        );
        _mItem1->runAction(RepeatForever::create(ac));
    }),
            nullptr
        )
    );

    _mame01->runAction(
        Sequence::create(
            EaseBackOut::create(MoveTo::create(0.5f, Vec2(winSizeCenterW, winSizeH - 238))),
            DelayTime::create(2.0f),
            CallFunc::create([=]() {
        auto ac = Sequence::create(
            MoveBy::create(0.8f, Vec2(0, 20)),
            MoveBy::create(0.8f, Vec2(0, -20)),
            nullptr
        );
        _mame01->runAction(RepeatForever::create(ac));
    }),
            nullptr
        )
    );

    _mame02->runAction(
        Sequence::create(
            DelayTime::create(0.3f),
            EaseBackOut::create(MoveTo::create(0.5f, Vec2(260, winSizeH - 410))),
            DelayTime::create(2.2f),
            CallFunc::create([=]() {
        auto ac = Sequence::create(
            MoveBy::create(0.8f, Vec2(0, 20)),
            MoveBy::create(0.8f, Vec2(0, -20)),
            nullptr
        );
        _mame02->runAction(RepeatForever::create(ac));
    }),
            nullptr
        )
    );

    _mame03->runAction(
        Sequence::create(
            DelayTime::create(0.3f),
            EaseBackOut::create(MoveTo::create(0.5f, Vec2(winSizeW - 260, 238))),
            DelayTime::create(2.4f),
            CallFunc::create([=]() {
        auto ac = Sequence::create(
            MoveBy::create(0.8f, Vec2(0, 20)),
            MoveBy::create(0.8f, Vec2(0, -20)),
            nullptr
        );
        _mame03->runAction(RepeatForever::create(ac));
    }),
            nullptr
        )
    );
}