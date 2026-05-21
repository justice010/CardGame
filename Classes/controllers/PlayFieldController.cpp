#include "PlayFieldController.h"
USING_NS_CC;

void PlayFieldController::init(GameModel &gameModel,
                               UndoManager &undoManager)
{
    _gameModel = &gameModel;
    _undoManager = &undoManager;
}

void PlayFieldController::initView(Node *parent)
{
    _playFieldView = PlayFieldView::create();
    _playFieldView->setPosition(Vec2(0, 0));
    parent->addChild(_playFieldView);

    _playFieldView->initCards(_gameModel->playfieldCards);

    // 注册点击回调
    _playFieldView->setOnCardClickCallback([this](int cardId)
                                           { handleCardClick(cardId); });
}

bool PlayFieldController::handleCardClick(int cardId)
{
    int index = findCardIndex(cardId);
    if (index < 0)
        return false;

    const CardModel &card = _gameModel->playfieldCards[index];
    if (!card.faceUp || card.matched)
        return false;

    // 检查是否可以和手牌区顶部牌匹配
    if (!MatchService::canMatch(card, _gameModel->trayCard))
        return false;

    replaceTrayWithPlayFieldCard(index);
    return true;
}

void PlayFieldController::replaceTrayWithPlayFieldCard(int cardIndex)
{
    CardModel &card = _gameModel->playfieldCards[cardIndex];
    CardModel prevTray = _gameModel->trayCard;

    // 记录回退
    _undoManager->recordPlayfieldToTray(prevTray, card, cardIndex);

    // 更新model
    card.matched = true;
    _gameModel->trayCard = card;
    _gameModel->trayCard.x = card.x;
    _gameModel->trayCard.y = card.y;
    MatchService::refreshFaceUpStates(_gameModel->playfieldCards);

    Vec2 trayWorldPos = getTrayCardWorldPosition ? getTrayCardWorldPosition() : Vec2(650, 190);

    // 播放动画
    _playFieldView->playCardMoveToTray(card.id,
                                       _playFieldView->convertToNodeSpace(trayWorldPos),
                                       [this, cardIndex]()
                                       {
                                           _playFieldView->setCardVisible(
                                               _gameModel->playfieldCards[cardIndex].id, false);
                                           refreshPlayfieldCards();
                                           if (onMatchSuccess)
                                               onMatchSuccess(_gameModel->trayCard);
                                       });
}

void PlayFieldController::handleUndo(const UndoRecord &record)
{
    if (record.type != UndoRecordType::PLAYFIELD_TO_TRAY)
        return;

    int index = record.movedCardIndex;
    _gameModel->playfieldCards[index] = record.movedCard;
    MatchService::refreshFaceUpStates(_gameModel->playfieldCards);
    refreshPlayfieldCards();

    // 显示牌并播放回退动画
    _playFieldView->setCardVisible(record.movedCard.id, true);
    Vec2 bacPos = Vec2(record.movedCard.x, record.movedCard.y);
    _playFieldView->playCardMoveBack(record.movedCard.id, bacPos);
}

void PlayFieldController::refreshPlayfieldCards()
{
    for (const auto &card : _gameModel->playfieldCards)
    {
        _playFieldView->updateCardDisplay(card);
    }
}

int PlayFieldController::findCardIndex(int cardId) const
{
    for (int i = 0; i < (int)_gameModel->playfieldCards.size(); i++)
    {
        if (_gameModel->playfieldCards[i].id == cardId)
            return i;
    }
    return -1;
}
