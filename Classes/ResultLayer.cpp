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

    //音データのプレロード
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se/result.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se/chuumokunokekkaha.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se/score.mp3");

    //スコア画像はBatchNodeで処理
    _scoreBatchNode = SpriteBatchNode::create("game/number.png");
    this->addChild(_scoreBatchNode);

    //背景
    auto bg = LayerColor::create(Color4B::WHITE, winSizeW, winSizeH);
    this->addChild(bg, (int)mainZOderList::BG);

    //結果発表
    _kekka = Sprite::create("result/kekka.png");
    _kekka->setPosition(Vec2(winSizeW + 500, winSizeCenterH));
    this->addChild(_kekka, (int)mainZOderList::TEXT);

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

    //今回の得点は
    _score_now = Sprite::create("result/score_now.png");
    _score_now->setPosition(Vec2(winSizeCenterW, winSizeH - 100));
    _score_now->setOpacity(0);
    this->addChild(_score_now, (int)mainZOderList::TEXT);

    //タイトルの戻る
    _titleback = Sprite::create("result/titleback.png");
    _titleback->setPosition(Vec2(winSizeCenterW + 300, 80));
    _titleback->setVisible(false);
    this->addChild(_titleback, (int)mainZOderList::TEXT);

    return true;
}

void ResultLayer::onEnterTransitionDidFinish()
{
    this->doAnime01();
}

void ResultLayer::doAnime01() {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se/chuumokunokekkaha.mp3");

    _kekka->runAction(
        Sequence::create(
            EaseOut::create(MoveTo::create(0.5f, Vec2(winSizeCenterW, winSizeCenterH)), 3.0f),
            DelayTime::create(1.0f),
            EaseIn::create(MoveTo::create(0.5f, Vec2(0 - 500, winSizeCenterH)), 3.0f),
            CallFunc::create([&]() {
               this->doAnime02();
            }),
            RemoveSelf::create(),
        nullptr
        )
    );
}

void ResultLayer::doAnime02() {
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
            Spawn::create(
                FadeIn::create(0.5f),
                MoveBy::create(0.5f, Vec2(0, 10)),
                nullptr
            ),
            nullptr
        )
    );

    _score_now->runAction(
        Sequence::create(
            DelayTime::create(0.5f),
            Spawn::create(
                FadeIn::create(0.5f),
                MoveBy::create(0.5f, Vec2(0, 10)),
                nullptr
            ),
            DelayTime::create(1.0f),
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
    
    auto hi_score = _userDef->getIntegerForKey("hi_score");
    if (hi_score < _score) {
        _recordNew = true;
        _userDef->setIntegerForKey("hi_score", _score);
    }

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
                CallFunc::create([&]() {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se/score.mp3");
                }),
                Spawn::create(
                    FadeIn::create(0.2f),
                    MoveBy::create(0.2f, Vec2(0, -10)),
                    nullptr
                ),
                DelayTime::create(0.5f),
                CallFunc::create([&]() {
                    this->doAnime03();
                }),
                nullptr
            );
        }
        else {
            ac = Sequence::create(
                DelayTime::create(i * 0.5),
                CallFunc::create([&]() {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se/score.mp3");
                }),
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

void ResultLayer::doAnime03() {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se/result.mp3");

    if (_recordNew) {
        auto base = Sprite::create("result/base.png");
        base->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
        this->addChild(base, (int)mainZOderList::KIRA);

        auto bg = Sprite::create("result/kira2.png");
        bg->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
        this->addChild(bg, (int)mainZOderList::KIRA);
        bg->runAction(
            RepeatForever::create(RotateBy::create(10.0f, 360.0f))
        );

        auto newrecord = Sprite::create("result/newrecord.png");
        newrecord->setPosition(Vec2(winSizeCenterW, winSizeCenterH + 50));
        this->addChild(newrecord, (int)mainZOderList::TEXT);
    }
    else {
        auto bg = Sprite::create("result/kira.png");
        bg->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
        this->addChild(bg, (int)mainZOderList::KIRA);
        bg->runAction(
            RepeatForever::create(RotateBy::create(10.0f, 360.0f))
        );
    }

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