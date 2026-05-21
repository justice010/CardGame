#pragma once
#include "cocos2d.h"
#include "controllers/GameController.h"

/**
 * GameScene 游戏主场景
 * 负责创建场景并启动GameController
 */
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();
    bool init() override;
    CREATE_FUNC(GameScene);

private:
    GameController _gameController;
};