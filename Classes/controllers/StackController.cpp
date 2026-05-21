#include "StackController.h"
USING_NS_CC;

void StackController::init(GameModel &gameModel, UndoManager &undoManager)
{
    _gameModel = &gameModel;
    _undoManager = &undoManager;
}

void StackController::initView(Node *parent)
{
    _trayView = TrayView::create();
    _trayView->setPosition(Vec2(400, 190)); // 手牌区在下方中央
    parent->addChild(_trayView);

    _trayView->initTray(_gameModel->trayCard);

    // 注册备用牌堆点击
    _trayView->setOnStackClickCallback([this]()
                                       { handleStackClick(); });
}

void StackController::handleStackClick()
{
    if (_gameModel->stackCards.empty())
        return;

    CardModel prevTray = _gameModel->trayCard;
    CardModel newCard = _gameModel->stackCards.back();
    int index = (int)_gameModel->stackCards.size() - 1;

    // 记录回退
    _undoManager->recordStackToTray(prevTray, newCard, index);

    // 更新model
    _gameModel->stackCards.pop_back();
    newCard.faceUp = true;
    _gameModel->trayCard = newCard;

    // 播放动画并更新视图
    Vec2 stackWorldPos = _trayView->getStackCardWorldPosition();
    _trayView->initTray(_gameModel->trayCard);
    _trayView->playCardMoveIn(stackWorldPos);
}

void StackController::updateTrayDisplay()
{
    _trayView->updateTrayCard(_gameModel->trayCard);
}

void StackController::handleUndo(const UndoRecord &undoRecord)
{
    if (undoRecord.type != UndoRecordType::STACK_TO_TRAY)
        return;

    _trayView->playCardMoveOut(_trayView->getStackCardWorldPosition(), [this]()
                               { _trayView->updateTrayCard(_gameModel->trayCard); });
}
