#include "UndoManager.h"

void UndoManager::init(UndoModel &undoModel)
{
    _undoModel = &undoModel;
}

void UndoManager::recordPlayfieldToTray(const CardModel &prevTray, const CardModel &movedCard, int cardIndex)
{
    if (!_undoModel)
        return;
    UndoRecord record;
    record.type = UndoRecordType::PLAYFIELD_TO_TRAY;
    record.prevTrayCard = prevTray;
    record.movedCard = movedCard;
    record.movedCardIndex = cardIndex;
    _undoModel->records.push_back(record);
}

void UndoManager::recordStackToTray(const CardModel &prevTray, const CardModel &movedCard, int cardIndex)
{
    if (!_undoModel)
        return;
    UndoRecord record;
    record.type = UndoRecordType::STACK_TO_TRAY;
    record.prevTrayCard = prevTray;
    record.movedCard = movedCard;
    record.movedCardIndex = cardIndex;
    _undoModel->records.push_back(record);
}

bool UndoManager::undo(GameModel &gameModel)
{
    if (!_undoModel || !_undoModel->canUndo())
        return false;
    UndoRecord record = _undoModel->records.back();
    _undoModel->records.pop_back();

    // 还原手牌区顶部牌
    gameModel.trayCard = record.prevTrayCard;

    if (record.type == UndoRecordType::PLAYFIELD_TO_TRAY)
    {
        // 边界检查
        int index = record.movedCardIndex;
        if (index >= 0 && index < (int)gameModel.playfieldCards.size())
        {
            gameModel.playfieldCards[index] = record.movedCard;
        }
    }
    else if (record.type == UndoRecordType::STACK_TO_TRAY)
    {
        // 直接把牌加回备用堆末尾，不用管原来的索引
        gameModel.stackCards.push_back(record.movedCard);
    }

    if (onUndoSuccess)
    {
        onUndoSuccess(record);
        return true;
    }
    return true;
}

bool UndoManager::canUndo() const
{
    return _undoModel && _undoModel->canUndo();
}