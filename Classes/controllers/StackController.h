#pragma once
#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "views/TrayView.h"
#include <functional>

/**
 * StackController 备用牌堆和手牌区控制器
 * 处理备用牌抽取逻辑，管理TrayView
 */
class StackController
{
public:
    void init(GameModel &gameModel, UndoManager &undoManager);
    void initView(cocos2d::Node *parent);

    /** 处理点击备用牌堆，抽一张新牌到手牌区 */
    void handleStackClick();

    /** 处理回退：将手牌区牌还原到备用堆 */
    void handleUndo(const UndoRecord &record);

    /** 更新手牌区显示（匹配成功后由GameController调用）*/
    void updateTrayDisplay();

    TrayView *getView() const { return _trayView; }

    /** 手牌区位置变化回调 */
    std::function<void(cocos2d::Vec2)> onTrayPositionChanged;

private:
    GameModel *_gameModel = nullptr;
    UndoManager *_undoManager = nullptr;
    TrayView *_trayView = nullptr;
};