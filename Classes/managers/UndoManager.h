#pragma once
#include "models/UndoModel.h"
#include "models/GameModel.h"
#include <functional>

/**
 * UndoManager 回退管理器
 * 作为GameController的成员变量，管理回退记录的增删
 * 禁止实现为单例；通过调用与外部通信
 */
class UndoManager
{
public:
    UndoManager() = default;

    /**
     * 初始化，绑定UndoModel
     * @param undoModel 外部传入的回退数据模型引用
     */
    void init(UndoModel &undoModel);

    /**
     * 记录一次桌面牌->手牌区的操作，便于回退
     * @param prevTray 操作前手牌区顶部牌
     * @param movedCard 被移动的桌面牌
     * @param cardIndex 被移动牌在playfieldCards中的索引
     */
    void recordPlayfieldToTray(const CardModel &prevTray, const CardModel &movedCard, int cardIndex);

    /**
     * 记录一次备用牌->手牌区的操作，便于回退
     * @param prevTray 操作前手牌区顶部牌
     * @param movedCard 被移动的备用牌
     * @param cardIndex 被移动牌在stackCards中的原索引
     */
    void recordStackToTray(const CardModel &prevTray, const CardModel &movedCard, int cardIndex);

    /**
     * 执行一次回退，还原GameModel到上一次状态
     * @param gameModel 需要被还原的游戏数据
     * @return 是否成功回退（有记录返回true）
     */
    bool undo(GameModel &gameModel);

    /** 是否有可回退的记录 */
    bool canUndo() const;

    /** 回退成功后的回调，外部（Controller）注册用于刷新View */
    std::function<void(const UndoRecord &)> onUndoSuccess;

private:
    UndoModel *_undoModel = nullptr;
};