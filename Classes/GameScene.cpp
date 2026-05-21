#include "GameScene.h"
USING_NS_CC;

Scene *GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    auto visSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 背景
    auto bgMain = LayerColor::create(Color4B(179, 128, 68, 255));
    this->addChild(bgMain, -2);

    auto bottomBar = LayerColor::create(Color4B(143, 34, 141, 255), 1080, 580);
    bottomBar->setPosition(Vec2(0, 0));
    this->addChild(bottomBar, -1);

    _gameController.startGame(this, "level1.json");
    return true;
}
