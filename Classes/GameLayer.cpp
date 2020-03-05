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


    this->scheduleUpdate();
    
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

    //ステージノードを作成
    this->setStage(Node::create());
    _stage->setPosition(Vec2(winSizeCenterW + 50, winSizeCenterH + 100));
    this->addChild(_stage, (int)mainZOderList::MAME);

    //豆を表示
    for (int x = 0; x < MAME_COUNT; x++) {
        auto mame = Mame::create();
        mame->setMamePosition(Vec2(x, 0));
        //豆リストに豆を追加
        _mames.pushBack(mame);
        //_stageノードに豆を追加
        _stage->addChild(mame);
        //位置を調整する
        mame->adjustPosition();
    }

}

void GameLayer::update(float dt) {
    // 全豆に対して落下を判定する
    for (auto mame : _mames) {
        this->fallMame(mame);
    }

    //豆のチェックと出現
    this->checkSpawn();
}

bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    Vec2 location = touch->getLocation();

    if (_btnOK1->getBoundingBox().containsPoint(location) && !_btnOK1_state) {
        this->ClickBtn(BtnType::OK1);
    }

    if (_btnOK2->getBoundingBox().containsPoint(location) && !_btnOK2_state) {
        this->ClickBtn(BtnType::OK2);
    }

    if (_btnOK3->getBoundingBox().containsPoint(location) && !_btnOK3_state) {
        this->ClickBtn(BtnType::OK3);
    }

    if (_btnNG->getBoundingBox().containsPoint(location) && !_btnNG_state) {
        this->ClickBtn(BtnType::NG);
    }

    return false;
}

void GameLayer::ClickBtn(BtnType btn_type) {

    CallFunc* func;
    Sprite* btn;
    switch (btn_type)
    {
    case GameLayer::BtnType::OK1:
        //豆の削除
        this->deleteMame(Mame::mameType::OK1);

        _btnOK1_state = true;
        btn = _btnOK1;
        func = CallFunc::create([&]() {
            _btnOK1_state = false;
        });
        break;
    case GameLayer::BtnType::OK2:
        //豆の削除
        this->deleteMame(Mame::mameType::OK2);

        _btnOK2_state = true;
        btn = _btnOK2;
        func = CallFunc::create([&]() {
            _btnOK2_state = false;
        });
        break;
    case GameLayer::BtnType::OK3:
        //豆の削除
        this->deleteMame(Mame::mameType::OK3);

        _btnOK3_state = true;
        btn = _btnOK3;
        func = CallFunc::create([&]() {
            _btnOK3_state = false;
        });
        break;
    case GameLayer::BtnType::NG:
        //豆の削除
        this->deleteMame(Mame::mameType::NG);

        _btnNG_state = true;
        btn = _btnNG;
        func = CallFunc::create([&]() {
            _btnNG_state = false;
        });
        break;
    default:
        return;
        break;
    }

    auto ac = Sequence::create(
        EaseOut::create(ScaleTo::create(0.1f, 0.4f), 2.0f),
        EaseElasticOut::create(ScaleTo::create(0.25f, 1.0f)),
        func,
        nullptr
        );
    btn->runAction(ac);
}

Mame* GameLayer::getMameAt(const Vec2& position) {
    for (auto& mame : _mames) {
        if (position.equals(mame->getMamePosition())) {
            return mame;
        }
    }
    return nullptr;
}

void GameLayer::deleteMame(Mame::mameType mame_type) {
    auto mame = this->getMameAt(Vec2(0, 0));
    if (!mame) return;

    //豆タイプと一致していなかたら何もしない
    if (mame->getMameType() != mame_type) return;

    // 状態を消去中にする
    mame->setState(Mame::State::DISAPEARING);

    if (mame_type == Mame::mameType::OK1 || mame_type == Mame::mameType::OK2 || mame_type == Mame::mameType::OK3) {
        //削除アニメーション
        mame->runAction(
            Sequence::create(
                CallFuncN::create([this](Node* node) {
                    // クッキー一覧から削除する
                    auto mame = dynamic_cast<Mame*>(node);
                    _mames.eraseObject(mame);
                }),
                MoveTo::create(1.0f, Vec2(winSizeW, 0)),
                RemoveSelf::create(),
            NULL));
    }
    else {
        mame->runAction(
            Sequence::create(
                CallFuncN::create([this](Node* node) {
                    auto mame = dynamic_cast<Mame*>(node);
                    _mames.eraseObject(mame);
                }),
                Spawn::create(
                    MoveBy::create(0.3f, Vec2(0, 200)),
                    FadeOut::create(0.3f),
                    nullptr
                ),
                RemoveSelf::create(),
            NULL));
    }

}

bool GameLayer::fallMame(Mame* mame)
{
    auto position = mame->getMamePosition();
    // すでに一番右にあったときや、停止中じゃないとき、落ちない
    if (position.x == 0 || !mame->isStatic()) {
        return false;
    }

    // 1つ右のグリッド座標を取り出す
    auto downPosition = Vec2(position.x - 1, position.y);
    // 1つ右を取り出す
    auto down = this->getMameAt(Vec2(position.x - 1, position.y));
    // 1つ右がなかったとき、落ちる
    if (down == nullptr) {
        // 落下アニメーション時間
        const auto duration = 0.05;
        // 落下距離
        auto distance = Mame::getSize();
        // 状態を落下中にする
        mame->setState(Mame::State::FALLING);
        // 落下アニメーションの実行
        mame->runAction(Sequence::create(MoveBy::create(duration, Vec2(distance, 0)),
            CallFuncN::create([this, downPosition](Node* node) {
            // 落下アニメーション終了後
            auto mame = dynamic_cast<Mame*>(node);
            // 豆を動かす
            this->moveMame(mame, downPosition);
            mame->setState(Mame::State::STATIC);
            // さらに落ちないか再度落下判定を行う
            this->fallMame(mame);
        }),
            NULL));
        return true;
    }
    return false;
}

void GameLayer::moveMame(Mame* mame, const Vec2& mamePosition)
{
    mame->setMamePosition(mamePosition);
    mame->adjustPosition();
}

Vector<Mame*> GameLayer::checkSpawn()
{
    // 出現したクッキーの一覧
    Vector<Mame*> mames;
    // 一番上をチェック
    auto mame = this->getMameAt(Vec2(MAME_COUNT - 1, 0));
    if (!mame) { // もし豆がなければ
        // 豆を追加する
        auto mame = Mame::create();
        mame->setMamePosition(Vec2(MAME_COUNT - 1, 0));
        //豆リストに豆を追加
        _mames.pushBack(mame);
        //_stageノードに豆を追加
        _stage->addChild(mame);
        //位置を調整する
        mame->adjustPosition();
    }
    return std::move(mames);
}