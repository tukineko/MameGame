#pragma execution_character_set("utf-8")
#include "ResultLayer.h"
#include "TitleLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* ResultLayer::createScene()
{
    Scene* scene = Scene::create();
    ResultLayer* layer = ResultLayer::create();
    scene->addChild(layer);
    return scene;
}

bool ResultLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    CCLOG("----------------ResultLayer::init()----------------");

    //スコア画像はBatchNodeで処理
    _scoreBatchNode = SpriteBatchNode::create("game/number.png");
    this->addChild(_scoreBatchNode);

    //背景
    auto bg = LayerColor::create(Color4B::WHITE, winSizeW, winSizeH);
    this->addChild(bg, (int)mainZOderList::BG);

    //豆
    _mame = Sprite::create("result/mame.png");
    _mame->setPosition(Vec2(320, 200));
    _mame->setOpacity(0);
    this->addChild(_mame, (int)mainZOderList::TEXT);

    //スコア土台
    _scorebase = Sprite::create("result/hukidasi.png");
    _scorebase->setPosition(Vec2(winSizeCenterW, winSizeCenterH + 100));
    _scorebase->setOpacity(0);
    this->addChild(_scorebase, (int)mainZOderList::TEXT);

    //あたなの得点は
    _text01 = Sprite::create("result/anatano.png");
    _text01->setPosition(Vec2(winSizeCenterW, winSizeH + 54));
    this->addChild(_text01, (int)mainZOderList::TEXT);

    //タイトルの戻る
    _titleback = Sprite::create("result/titleback.png");
    _titleback->setPosition(Vec2(winSizeCenterW + 300, 80));
    _titleback->setVisible(false);
    this->addChild(_titleback, (int)mainZOderList::TEXT);

    return true;
}

void ResultLayer::onEnterTransitionDidFinish()
{
    _mame->runAction(
        Sequence::create(
            Spawn::create(
                FadeIn::create(0.5f),
                MoveBy::create(0.5f, Vec2(0, 10)),
                nullptr
            ),
            nullptr
        )
    );

    _scorebase->runAction(
        Sequence::create(
            DelayTime::create(0.8f),
            Spawn::create(
                FadeIn::create(0.5f),
                MoveBy::create(0.5f, Vec2(0, 10)),
                nullptr
            ),
            nullptr
        )
    );

    _text01->runAction(
        Sequence::create(
            DelayTime::create(1.3f),
            EaseOut::create(MoveTo::create(0.5f, Vec2(winSizeCenterW, winSizeH - 100)), 3.0f),
            CallFunc::create([&]() {
                this->viewScore();
            }),
            nullptr
        )
    );
}

void ResultLayer::viewScore() {
    UserDefault* _userDef = UserDefault::getInstance();
    auto _score = _userDef->getIntegerForKey("score");
    
    std::string score = std::to_string(_score);
    std::reverse(score.begin(), score.end());
    int lang = score.length();
    int numberRect = 64;

    for (int i = 0; i < lang; i++) {
        auto number = Sprite::createWithTexture(_scoreBatchNode->getTexture(), Rect(0, 0, numberRect, numberRect));
        number->setPosition(Vec2((winSizeCenterW + 200) - numberRect * i, winSizeCenterH + 120));
        char c = score[i];
        int num = c - '0';
        number->setTextureRect(Rect(numberRect * num, 0, numberRect, numberRect));
        number->setOpacity(0);

        Sequence* ac;
        if (i == lang - 1) {
            ac = Sequence::create(
                DelayTime::create(i * 0.5),
                Spawn::create(
                    FadeIn::create(0.2f),
                    MoveBy::create(0.2f, Vec2(0, -10)),
                    nullptr
                ),
                CallFunc::create([&]() {
                    this->viewScore2();
                }),
                nullptr
            );
        }
        else {
            ac = Sequence::create(
                DelayTime::create(i * 0.5),
                Spawn::create(
                    FadeIn::create(0.2f),
                    MoveBy::create(0.2f, Vec2(0, -10)),
                    nullptr
                ),
                nullptr
            );
        }

        number->runAction(ac);
        this->addChild(number, (int)mainZOderList::SCORE);
    }
}

void ResultLayer::viewScore2() {
    auto bg = Sprite::create("result/kira.png");
    bg->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
    this->addChild(bg, (int)mainZOderList::KIRA);
    bg->runAction(
        RepeatForever::create(RotateBy::create(10.0f, 360.0f))
    );
    
    _titleback->setVisible(true);
    _titleback->runAction(
        RepeatForever::create(
            Sequence::create(
                DelayTime::create(0.4f),
                FadeOut::create(0.2f),
                FadeIn::create(0.2f),
                nullptr
            )
        )
    );

    //タッチイベントの設定
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ResultLayer::onTouchBegan, this);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool ResultLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    
    //タイトル画面へ
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, TitleLayer::createScene(), Color3B::WHITE));

    return false;
}