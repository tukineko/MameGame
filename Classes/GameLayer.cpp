#pragma execution_character_set("utf-8")
#include "GameLayer.h"
#include "ResultLayer.h"
#include "SimpleAudioEngine.h"
#include <iomanip>

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
    
    //スコア画像はBatchNodeで処理
    _scoreBatchNode = SpriteBatchNode::create("game/number.png");
    this->addChild(_scoreBatchNode);

    //コンボ画像はBatchNodeで処理
    _comboBatchNode = SpriteBatchNode::create("game/number_combo.png");
    this->addChild(_comboBatchNode);

    //初期化
    _score = 0;
    _combo = 0;
    _timer = 30.0f;
    _state = GameState::DEFAULT;

    //初期表示
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

    //ゲーム開始土台
    auto bg_start = LayerColor::create(Color4B(0, 0, 0, 128), winSizeW, winSizeH);
    this->addChild(bg_start, (int)mainZOderList::START, "base_start");

    //コンボ土台を表示
    auto base_combo = Sprite::create("game/base_combo.png");
    base_combo->setPosition(Vec2(winSizeCenterW - 150, winSizeCenterH + 230));
    this->addChild(base_combo, (int)mainZOderList::COMBO);

    //制限時間を表示
    this->viewTimer();

    //スコア表示
    this->viewScore();

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

void GameLayer::onEnterTransitionDidFinish()
{
    this->viewGameStart();
}

void GameLayer::viewGameStart() {

    auto count3 = Sprite::create("game/3.png");
    count3->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
    count3->setOpacity(0);
    this->addChild(count3, (int)mainZOderList::START);

    auto count2 = Sprite::create("game/2.png");
    count2->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
    count2->setOpacity(0);
    this->addChild(count2, (int)mainZOderList::START);

    auto count1 = Sprite::create("game/1.png");
    count1->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
    count1->setOpacity(0);
    this->addChild(count1, (int)mainZOderList::START);

    auto start = Sprite::create("game/start.png");
    start->setPosition(Vec2(winSizeCenterW, winSizeCenterH));
    start->setOpacity(0);
    this->addChild(start, (int)mainZOderList::START);

    auto ac = Sequence::create(
        Spawn::create(
            EaseOut::create(MoveBy::create(0.3f, Vec2(0, 10)), 3),
            EaseOut::create(FadeIn::create(0.5f), 3),
            nullptr
        ),
        EaseOut::create(FadeOut::create(0.1f), 3),
        nullptr);

    auto ac2 = Sequence::create(
        FadeIn::create(0.0f),
        Spawn::create(
            EaseIn::create(ScaleTo::create(0.3f, 2.0f), 3),
            EaseIn::create(FadeOut::create(0.3f), 3),
            nullptr
        ),
        EaseOut::create(FadeOut::create(0.1f), 3),
        CallFunc::create([this]() {
            this->removeChildByName("base_start");
            _state = GameState::GAME;
        }),
        nullptr);

    count3->runAction(
        Sequence::create(
            DelayTime::create(0.5f),
            ac,
            TargetedAction::create(count2, Sequence::create(ac, RemoveSelf::create(), nullptr)),
            TargetedAction::create(count1, Sequence::create(ac, RemoveSelf::create(), nullptr)),
            TargetedAction::create(start, Sequence::create(ac2, RemoveSelf::create(), nullptr)),
            RemoveSelf::create(),
            nullptr)
    );
}

void GameLayer::viewTimer() {
    //timerの名前がついているノードをすべて削除
    this->enumerateChildren("timer", [](Node* node) -> bool {
        auto action = RemoveSelf::create();
        node->runAction(action);
        return false;
    });

    std::string timer = StringUtils::toString((int)_timer);
    int lang = timer.length();
    int numberRect = 64;

    for (int i = 0; i < lang; i++) {
        auto number = Sprite::createWithTexture(_scoreBatchNode->getTexture(), Rect(0, 0, numberRect, numberRect));
        number->setPosition(Vec2(100 + numberRect * i, winSizeH - 50));
        char c = timer[i];
        int num = c - '0';
        number->setTextureRect(Rect(numberRect * num, 0, numberRect, numberRect));
        this->addChild(number, (int)mainZOderList::SCORE, "timer");
    }
}

void GameLayer::viewScore() {
    //scoreの名前がついているノードをすべて削除
    this->enumerateChildren("score", [](Node* node) -> bool {
        auto action = RemoveSelf::create();
        node->runAction(action);
        return false;
    });

    //文字列に変換、10桁0で埋める
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(10) << _score;
    std::string score = oss.str().c_str();
    int lang = score.length();
    int numberRect = 64;

    for (int i = 0; i < lang; i++) {
        auto number = Sprite::createWithTexture(_scoreBatchNode->getTexture(), Rect(0, 0, numberRect, numberRect));
        number->setPosition(Vec2((winSizeCenterW + 50) + numberRect * i, winSizeH - 50));
        char c = score[i];
        int num = c - '0';
        number->setTextureRect(Rect(numberRect * num, 0, numberRect, numberRect));
        this->addChild(number, (int)mainZOderList::SCORE, "score");
    }
}

void GameLayer::viewCombo() {
    //comboの名前がついているノードをすべて削除
    this->enumerateChildren("combo", [](Node* node) -> bool {
        auto action = RemoveSelf::create();
        node->runAction(action);
        return false;
    });
    //ラベルも削除
    this->removeChildByName("combo_label");

    //10コンボ以下なら
    if (_combo < 10) return;

    auto combo_label = Sprite::create("game/combo.png");
    combo_label->setPosition(Vec2(winSizeCenterW - 155, winSizeCenterH + 205));
    this->addChild(combo_label, (int)mainZOderList::COMBO, "combo_label");

    std::string combo = std::to_string(_combo);
    int lang = combo.length();
    int numberRect = 64;

    for (int i = 0; i < lang; i++) {
        auto number = Sprite::createWithTexture(_comboBatchNode->getTexture(), Rect(0, 0, numberRect, numberRect));
        number->setPosition(Vec2((winSizeCenterW - 180) + numberRect * i, winSizeCenterH + 260));
        char c = combo[i];
        int num = c - '0';
        number->setTextureRect(Rect(numberRect * num, 0, numberRect, numberRect));
        this->addChild(number, (int)mainZOderList::COMBO, "combo");
        number->runAction(
            Sequence::create(
                EaseOut::create(ScaleTo::create(0.1f, 1.0f, 1.2f), 2.0f),
                EaseElasticOut::create(ScaleTo::create(0.25f, 1.0f)),
                nullptr
            )
        );
    }
}

void GameLayer::update(float dt) {
    if (_state == GameState::GAME) {
        //秒数を減らす
        _timer -= dt;
        this->viewTimer();
        if ((int)_timer <= 0) {
            this->viewTimeUp();
        }

        // 全豆に対して落下を判定する
        for (auto mame : _mames) {
            this->fallMame(mame);
        }

        //豆のチェックと出現
        this->checkSpawn();
    }
}

bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (_state != GameState::GAME) {
        return false;
    }
    
    Vec2 location = touch->getLocation();

    if (_btnOK1->getBoundingBox().containsPoint(location)) {
        this->ClickBtn(BtnType::OK1);
    }

    if (_btnOK2->getBoundingBox().containsPoint(location)) {
        this->ClickBtn(BtnType::OK2);
    }

    if (_btnOK3->getBoundingBox().containsPoint(location)) {
        this->ClickBtn(BtnType::OK3);
    }

    if (_btnNG->getBoundingBox().containsPoint(location)) {
        this->ClickBtn(BtnType::NG);
    }

    return false;
}

void GameLayer::ClickBtn(BtnType btn_type) {

    Sprite* btn;
    switch (btn_type)
    {
    case GameLayer::BtnType::OK1:
        //豆の削除
        this->deleteMame(Mame::mameType::OK1);
        btn = _btnOK1;
        break;
    case GameLayer::BtnType::OK2:
        this->deleteMame(Mame::mameType::OK2);
        btn = _btnOK2;
        break;
    case GameLayer::BtnType::OK3:
        this->deleteMame(Mame::mameType::OK3);
        btn = _btnOK3;
        break;
    case GameLayer::BtnType::NG:
        this->deleteMame(Mame::mameType::NG);
        btn = _btnNG;
        break;
    default:
        return;
        break;
    }

    auto ac = Sequence::create(
        EaseOut::create(ScaleTo::create(0.1f, 0.4f), 2.0f),
        EaseElasticOut::create(ScaleTo::create(0.25f, 1.0f)),
        nullptr
        );
    btn->runAction(ac);
}

void GameLayer::viewTimeUp() {
    //状態をタイムアップに
    _state = GameState::TIMEUP;

    auto bg = LayerColor::create(Color4B(0, 0, 0, 128), winSizeW, 240);
    bg->setPosition(Vec2(0, winSizeCenterH - 120));
    this->addChild(bg, (int)mainZOderList::TIMEUP, "base_timeup");

    auto game_end = Sprite::create("game/timeup.png");
    game_end->setPosition(Vec2(0, winSizeCenterH));
    game_end->setOpacity(0);
    this->addChild(game_end, (int)mainZOderList::TIMEUP);
    
    game_end->runAction(
        Sequence::create(
            Spawn::create(
                EaseOut::create(MoveTo::create(0.5f, Vec2(winSizeCenterW, winSizeCenterH)), 3.0f),
                FadeIn::create(0.5f),
                nullptr
            ),
            DelayTime::create(2.0f),
            Spawn::create(
                EaseIn::create(MoveTo::create(0.5f, Vec2(winSizeW, winSizeCenterH)), 3.0f),
                FadeOut::create(0.4f),
                nullptr
            ),
            CallFunc::create([&]() {
                this->removeChildByName("base_timeup");
                this->viewResult();
            }),
            RemoveSelf::create(),
            nullptr
        )
    );
}

void GameLayer::viewResult() {
    //状態をタイムアップに
    _state = GameState::RESULT;

    //スコアを保存
    UserDefault* _userDef = UserDefault::getInstance();
    _userDef->setIntegerForKey("score", _score);

    //リザルト画面へ
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, ResultLayer::createScene(), Color3B::WHITE));

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
    if (mame->getMameType() != mame_type) {
        //コンボをリセット
        _combo = 0;
        //コンボ表示を更新
        this->viewCombo();
        return;
    };

    //コンボ表示を更新
    _combo++;
    this->viewCombo();

    //スコア表示を更新
    auto point = this->calcScore(mame->getMameScore());
    _score += point;
    this->viewScore();

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

int GameLayer::calcScore(int point) {
    if (100 <= _combo) {
        return point * 8;
    }
    else if (60 <= _combo && _combo < 100) {
        return point * 6;
    }
    else if (30 <= _combo && _combo < 60) {
        return point * 2;
    }
    else if (10 <= _combo && _combo < 30) {
        return point * 1.5;
    }
    else {
        return point;
    }
}