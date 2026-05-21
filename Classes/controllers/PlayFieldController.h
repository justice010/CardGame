#pragma once
#include "models/GameModel.h"
#include "models/UndoModel.h"
#include "views/PlayFieldView.h"
#include "managers/UndoManager.h"
#include "services/MatchService.h"
#include <functional>

/**
 * PlayFieldController 主牌区控制器
 * 处理桌面牌的点击逻辑，协调PlayFieldView和GameModel
 * 作为GameController的成员
 */
class PlayFieldController
{
public:
    /**
     * 初始化控制器
     * @param gameModel 游戏数据模型引用
     * @param undoManager 回退管理器引用
     */
    void init(GameModel &gameModel, UndoManager &undoManager);

    /**
     * 初始化视图
     * @param parent 父节点
     */
    void initView(cocos2d::Node *parent);

    /**
     * 处理桌面牌点击事件
     * @param cardId 被点击牌的id
     * @return 是否匹配成功
     */
    bool handleCardClick(int cardId);

    /**
     * 执行回退：将手牌区移回桌面
     * @param record 回退记录
     */
    void handleUndo(const UndoRecord &record);

    /** 获取主牌区视图，（供GameController拿到位置信息） */
    PlayFieldView *getView() const { return _playFieldView; };

    /** 匹配成功回调，通知GameController更新手牌区 */
    std::function<void(const CardModel &movedCard)> onMatchSuccess;

    /** 提供手牌区顶部牌世界坐标，用于桌面牌移动动画 */
    std::function<cocos2d::Vec2()> getTrayCardWorldPosition;

private:
    /**
     * 执行桌面牌替换手牌区顶部牌
     * @param cardIndex 牌在playfieldCards中的索引
     */
    void replaceTrayWithPlayFieldCard(int cardIndex);

    void refreshPlayfieldCards();

    int findCardIndex(int cardId) const;

    GameModel *_gameModel = nullptr;
    UndoManager *_undoManager = nullptr;
    PlayFieldView *_playFieldView = nullptr;
};
