#pragma once
#include "CardModel.h"
#include <vector>

/**
 * UndoRecordType 回退操作类型枚举
 * 便于未来扩展新的回退类型
 */
enum class UndoRecordType
{
    PLAYFIELD_TO_TRAY, // 从主牌区移到手牌区
    STACK_TO_TRAY,     // 从备用牌堆移到手牌区
};

/**
 * UndoRecord 单条回退记录
 * 存储一次操作前的完整状态快照，用于撤销
 */
struct UndoRecord
{
    UndoRecordType type;

    CardModel prevTrayCard;  // 操作前手牌区顶部牌
    CardModel movedCard;     // 被移动的牌
    int movedCardIndex = -1; // 被移动的牌在原数组中的位置
};

/**
 * UndoModel 回退数据模型
 * 存储所有可回退的操作记录栈
 */
struct UndoModel
{
    std::vector<UndoRecord> records; // 回退记录栈，back()是最新的操作记录

    bool canUndo() const { return !records.empty(); }
};
