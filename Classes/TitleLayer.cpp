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

    //背景
    auto bg = LayerGradient::create(Color4B(50, 150, 200, 255), Color4B(109, 195, 236, 255));
    //auto bg = LayerColor::create(Color4B(109, 195, 236, 255), winSizeW, winSizeH);
    this->addChild(bg, (int)mainZOderList::BG);

    //タイトル
    auto title = Sprite::create("title/title.png");
    title->setPosition(Vec2(winSizeCenterW, winSizeH - 200));
    title->setOpacity(0);
    this->addChild(title, (int)mainZOderList::TITLE);
    title->runAction(
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
    
    //メニューボタン
    auto mItem1 = MenuItemImage::create("title/btn_start.png", "title/btn_start.png", [](Ref* sender) {
        //ゲーム画面へ
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameLayer::createScene(), Color3B::WHITE));
    });
    mItem1->setPosition(Vec2(winSizeCenterW, 130));

    //メニューを作成
    auto menu = Menu::create(mItem1, NULL);
    menu->setPosition(Point::ZERO);
    menu->setOpacity(0);
    this->addChild(menu, (int)mainZOderList::MENU);
    menu->runAction(
        Sequence::create(
            DelayTime::create(1.0f),
            Spawn::create(
                FadeIn::create(0.5f),
                MoveBy::create(0.5f, Vec2(0, 10)),
                nullptr
            ),
            DelayTime::create(0.3f),
            CallFunc::create([=]() {
                //メニューボタンを動かす
                auto ac = Sequence::create(
                    ScaleTo::create(0.4f, 1.1f),
                    ScaleTo::create(0.4f, 1.0f),
                    nullptr
                    );
                mItem1->runAction(RepeatForever::create(ac));
            }),
            nullptr
        )
    );

    //豆中央
    auto mame01 = Sprite::create("title/mame01.png");
    mame01->setPosition(Vec2(winSizeCenterW, winSizeH + 172));
    this->addChild(mame01, (int)mainZOderList::MAME);
    mame01->runAction(
        Sequence::create(
            EaseBackOut::create(MoveTo::create(0.5f, Vec2(winSizeCenterW, winSizeH - 238))),
            DelayTime::create(2.0f),
            CallFunc::create([=]() {
                auto ac = Sequence::create(
                    MoveBy::create(0.8f, Vec2(0, 20)),
                    MoveBy::create(0.8f, Vec2(0, -20)),
                    nullptr
                );
                mame01->runAction(RepeatForever::create(ac));
            }),
            nullptr
        )
    );

    //豆左
    auto mame02 = Sprite::create("title/mame02.png");
    mame02->setPosition(Vec2(-200, winSizeH - 410));
    this->addChild(mame02, (int)mainZOderList::MAME);
    mame02->runAction(
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
                mame02->runAction(RepeatForever::create(ac));
            }),
            nullptr
        )
    );

    //豆右
    auto mame03 = Sprite::create("title/mame03.png");
    mame03->setPosition(Vec2(winSizeW + 180, 238));
    this->addChild(mame03, (int)mainZOderList::MAME);
    mame03->runAction(
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
                mame03->runAction(RepeatForever::create(ac));
            }),
            nullptr
        )
    );

    return true;
}
