#include "TrayView.h"
USING_NS_CC;

TrayView *TrayView::create()
{
    auto view = new TrayView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool TrayView::init()
{
    if (!Node::init())
        return false;

    // 显示叠牌效果（3张错开叠放）
    for (int i = 2; i >= 0; i--)
    {
        auto stackCard = Sprite::create("res/card_general.png");
        if (stackCard)
        {
            stackCard->setScale(1.0f);
            // 每张牌稍微错开，制造叠牌感
            stackCard->setPosition(getStackCardLocalPosition() + Vec2(i * 5, i * 5));
            addChild(stackCard, i);
        }
    }

    // MenuItem 备用牌堆点击
    auto normalSprite = Sprite::create("res/card_general.png");
    auto selectedSprite = Sprite::create("res/card_general.png");
    if (normalSprite && selectedSprite)
    {
        selectedSprite->setColor(cocos2d::Color3B(150, 150, 150));
        auto btn = cocos2d::MenuItemSprite::create(
            normalSprite, selectedSprite,
            [this](Ref *)
            {
                if (_onStackClickCallback)
                    _onStackClickCallback();
            });
        btn->setScale(1.0f);
        btn->setPosition(getStackCardLocalPosition());
        auto menu = cocos2d::Menu::create(btn, nullptr);
        menu->setPosition(Vec2::ZERO);
        addChild(menu, 10);
    }

    return true;
}

void TrayView::initTray(const CardModel &trayCard)
{
    if (_trayCardView)
    {
        _trayCardView->removeFromParent();
    }
    _trayCardView = CardView::create(trayCard);
    _trayCardView->setPosition(getTrayCardLocalPosition()); // 顶部牌在右边
    addChild(_trayCardView, 5);
}

void TrayView::updateTrayCard(const CardModel &trayCard)
{
    if (_trayCardView)
        _trayCardView->updateDisplay(trayCard);
}

void TrayView::playCardMoveIn(Vec2 fromPos,
                              std::function<void()> onComplete)
{
    if (!_trayCardView)
        return;
    _trayCardView->setPosition(convertToNodeSpace(fromPos));
    _trayCardView->playMoveTo(getTrayCardLocalPosition(), 0.2f, onComplete);
}

void TrayView::playCardMoveOut(Vec2 toWorldPos,
                               std::function<void()> onComplete)
{
    if (!_trayCardView)
        return;
    _trayCardView->playMoveTo(convertToNodeSpace(toWorldPos), 0.2f, [this, onComplete]()
                              {
                                  _trayCardView->setPosition(getTrayCardLocalPosition());
                                  if (onComplete)
                                      onComplete();
                              });
}

void TrayView::setOnStackClickCallback(std::function<void()> callback)
{
    _onStackClickCallback = callback;
}

Vec2 TrayView::getTrayCardWorldPosition() const
{
    return convertToWorldSpace(getTrayCardLocalPosition());
}

Vec2 TrayView::getStackCardWorldPosition() const
{
    return convertToWorldSpace(getStackCardLocalPosition());
}

Vec2 TrayView::getTrayCardLocalPosition() const
{
    return Vec2(250, 0);
}

Vec2 TrayView::getStackCardLocalPosition() const
{
    return Vec2(-50, 0);
}
