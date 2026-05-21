#pragma once
#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

/**
 * CardView 单张牌的识图组件
 * 只负责显示和接收点击事件，不包含业务逻辑
 * 通过回调通知Controller处理点击
 */
class CardView : public cocos2d::Node
{
public:
    /**
     * 创建一张牌的视图
     * @param model 牌的数据模型
     */
    static CardView *create(const CardModel &model);
    bool init(const CardModel &model);

    /**
     * 播放移动画面（平移到目标位置）
     * @param targetPos 目标位置
     * @param duration 动画时长
     * @param onComplete 动画完成回调
     */
    void playMoveTo(cocos2d::Vec2 targetPos,
                    float duration = 0.2f,
                    std::function<void()> onComplete = nullptr);

    /**
     * 播放回退动画（平移到原始位置）
     * @param targetPos 回退位置目标
     * @param onComplete 动画完成回调
     */
    void playMoveBack(cocos2d::Vec2 targetPos,
                      std::function<void()> onComplete = nullptr);

    /** 更新显示（翻开/覆盖状态切换） */
    void updateDisplay(const CardModel &model);

    /** 设置点击回调，Controller注册用 */
    void setOnClickCallback(std::function<void(int cardId)> callback);

    int getCardId() const { return _cardId; }

private:
    void setupTouchListener();
    std::string getCardImageName(const CardModel &model) const;

    int _cardId = -1;
    cocos2d::Sprite *_cardSprite = nullptr;
    std::function<void(int)> _onClickCallback;
};
