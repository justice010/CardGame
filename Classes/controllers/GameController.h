#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "models/UndoModel.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"

/**
 * GameController 游戏总控制器
 * 协调所有子控制器、管理器的初始化和交互
 * 是游戏逻辑的入口
 */
class GameController
{
public:
    GameController() = default;
    ~GameController() = default;

    /**
     * 启动游戏
     * @param parent 场景根节点
     * @param levelFile 关卡配置文件lujinf
     */
    void startGame(cocos2d::Node *parent,
                   const std::string &levelFile = "level1.json");
    /** 执行回退操作（点击回退按钮时调用） */
    void handleUndo();

private:
    void initControllers(cocos2d::Node *parent);
    void setupUndoButton(cocos2d::Node *parent);

    GameModel _gameModel;
    UndoModel _undoModel;
    UndoManager _undoManager;
    PlayFieldController _playFieldController;
    StackController _stackController;
};