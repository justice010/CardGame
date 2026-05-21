#include "PlayFieldView.h"
USING_NS_CC;

PlayFieldView *PlayFieldView::create()
{
    auto view = new PlayFieldView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool PlayFieldView::init()
{
    if (!Node::init())
        return false;
    return true;
}

void PlayFieldView::initCards(const std::vector<CardModel> &cards)
{
    for (const auto &cardModel : cards)
    {
        auto cardView = CardView::create(cardModel);
        cardView->setPosition(Vec2(cardModel.x, cardModel.y));

        // 注册点击回调，转发给Controller
        cardView->setOnClickCallback([this](int cardId)
                                     {
        if (_onCardClickCallback) {
          _onCardClickCallback(cardId);
        } });
        addChild(cardView);
        _cardViews[cardModel.id] = cardView;
    }
}

void PlayFieldView::playCardMoveToTray(int cardId,
                                       Vec2 targetPos,
                                       std::function<void()> onComplete)
{
    auto it = _cardViews.find(cardId);
    if (it == _cardViews.end())
        return;
    it->second->playMoveTo(targetPos, 0.2f, onComplete);
}

void PlayFieldView::playCardMoveBack(int cardId,
                                     Vec2 targetPos,
                                     std::function<void()> onComplete)
{
    auto it = _cardViews.find(cardId);
    if (it == _cardViews.end())
        return;
    it->second->playMoveBack(targetPos, onComplete);
}

void PlayFieldView::setCardVisible(int cardId, bool visible)
{
    auto it = _cardViews.find(cardId);
    if (it == _cardViews.end())
        return;
    it->second->setVisible(visible);
}

void PlayFieldView::updateCardDisplay(const CardModel &cardModel)
{
    auto it = _cardViews.find(cardModel.id);
    if (it == _cardViews.end())
        return;
    it->second->updateDisplay(cardModel);
}

void PlayFieldView::setOnCardClickCallback(std::function<void(int)> callback)
{
    _onCardClickCallback = callback;
}
