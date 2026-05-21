#include "CardView.h"
USING_NS_CC;

namespace
{
const float kCardResourceScale = 1.0f;
}

CardView *CardView::create(const CardModel &model)
{
    auto view = new CardView();
    if (view && view->init(model))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(const CardModel &model)
{
    if (!Node::init())
        return false;
    _cardId = model.id;

    // 第一层：牌的底框
    auto bgSprite = Sprite::create("res/card_general.png");
    if (bgSprite)
    {
        bgSprite->setScale(kCardResourceScale);
        addChild(bgSprite, 0);
    }

    // 第二层：大数字图叠加在上面
    std::string imageName = getCardImageName(model);
    _cardSprite = Sprite::create(imageName);
    if (_cardSprite)
    {
        _cardSprite->setScale(kCardResourceScale);
        addChild(_cardSprite, 1);
    }

    // 第二层副：新增小数字图（利用字符串替换直接生成路径）
    std::string smallImageName = imageName;
    size_t pos = smallImageName.find("big_");
    if (pos != std::string::npos)
    {
        smallImageName.replace(pos, 4, "small_");
    }

    auto smallCardSprite = Sprite::create(smallImageName);
    if (smallCardSprite)
    {
        smallCardSprite->setScale(kCardResourceScale);
        smallCardSprite->setPosition(Vec2(-60, 105));
        addChild(smallCardSprite, 1, 101); // Tag 设为 101
    }

    // 第三层：花色图标
    const std::string suitNames[] = {
        "club", "diamond", "heart", "spade"};
    if (model.suit >= 0 && model.suit < CST_NUM_CARD_SUIT_TYPES)
    {
        std::string suitFile = "res/suits/" + suitNames[(int)model.suit] + ".png";

        auto suitTL = Sprite::create(suitFile);
        if (suitTL)
        {
            suitTL->setScale(kCardResourceScale);
            suitTL->setPosition(Vec2(55, 105));
            addChild(suitTL, 2, 102);
        }
    }

    updateDisplay(model);
    setupTouchListener();
    return true;
}

std::string CardView::getCardImageName(const CardModel &model) const
{
    bool isRed = (model.suit == CST_HEARTS || model.suit == CST_DIAMONDS);
    std::string color = isRed ? "red" : "black";

    const std::string faceNames[] = {
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    if (model.face < 0 || model.face >= CFT_NUM_CARD_FACE_TYPES)
    {
        return "res/card_general.png";
    }

    return "res/number/big_" + color + "_" + faceNames[(int)model.face] + ".png";
}

void CardView::setupTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch *touch, Event *) -> bool
    {
        Vec2 localPos = convertToNodeSpace(touch->getLocation());
        if (_cardSprite)
        {
            return _cardSprite->getBoundingBox().containsPoint(localPos);
        }
        return getBoundingBox().containsPoint(localPos);
    };

    listener->onTouchEnded = [this](Touch *touch, Event *)
    {
        if (_onClickCallback)
        {
            _onClickCallback(_cardId);
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::setOnClickCallback(std::function<void(int)> callback)
{
    _onClickCallback = callback;
}

void CardView::playMoveTo(cocos2d::Vec2 targetPos,
                          float duration,
                          std::function<void()> onComplete)
{
    auto move = MoveTo::create(duration, targetPos);
    if (onComplete)
    {
        auto call = CallFunc::create(onComplete);
        runAction(Sequence::create(move, call, nullptr));
    }
    else
    {
        runAction(move);
    }
}

void CardView::playMoveBack(cocos2d::Vec2 targetPos,
                            std::function<void()> onComplete)
{
    playMoveTo(targetPos, 0.2f, onComplete);
}

void CardView::updateDisplay(const CardModel &model)
{
    if (!_cardSprite)
        return;

    auto smallNum = (Sprite *)getChildByTag(101);
    auto suit = getChildByTag(102);

    if (model.faceUp)
    {
        // 1. 恢复大数字
        _cardSprite->setTexture(getCardImageName(model));

        // 2. 恢复并更新小数字
        if (smallNum)
        {
            std::string smallName = getCardImageName(model);
            size_t pos = smallName.find("big_");
            if (pos != std::string::npos)
                smallName.replace(pos, 4, "small_");

            smallNum->setTexture(smallName);
            smallNum->setVisible(true);
        }

        // 3. 恢复花色
        if (suit)
            suit->setVisible(true);
    }
    else
    {
        // 牌面朝下：变成卡背，并隐藏小数字和花色
        _cardSprite->setTexture("res/card_general.png");

        if (smallNum)
            smallNum->setVisible(false);
        if (suit)
            suit->setVisible(false);
    }
}
