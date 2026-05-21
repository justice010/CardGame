#pragma once
#include "cocos2d.h"
#include "models/CardModel.h"
#include "views/CardView.h"
#include <functional>

/**
 * TrayView 手牌区视图
 * 显示当前手牌区顶部牌和备用牌堆
 * 点击备用牌堆触发抽牌，通知Controller
 */
class TrayView : public cocos2d::Node
{
public:
    static TrayView *create();
    bool init() override;

    /** 初始化手牌区显示 */
    void initTray(const CardModel &trayCard);

    /** 更新手牌区顶部展示 */
    void updateTrayCard(const CardModel &trayCard);

    /**
     * 播放移入手牌区动画
     * @param fromPos 动画起始位置
     * @param onComplete 完成回调
     */
    void playCardMoveIn(cocos2d::Vec2 fromPos,
                        std::function<void()> onComplete = nullptr);

    /**
     * 播放当前手牌区顶部牌移回备用牌堆动画
     * @param toWorldPos 动画目标世界坐标
     * @param onComplete 完成回调
     */
    void playCardMoveOut(cocos2d::Vec2 toWorldPos,
                         std::function<void()> onComplete = nullptr);

    /** 注册点击备用牌堆的回调 */
    void setOnStackClickCallback(std::function<void()> callback);

    /** 获取手牌区顶部牌的世界坐标位置 */
    cocos2d::Vec2 getTrayCardWorldPosition() const;

    /** 获取备用牌堆的世界坐标位置 */
    cocos2d::Vec2 getStackCardWorldPosition() const;

private:
    cocos2d::Vec2 getTrayCardLocalPosition() const;
    cocos2d::Vec2 getStackCardLocalPosition() const;

    CardView *_trayCardView = nullptr;
    cocos2d::Sprite *_stackSprite = nullptr;
    std::function<void()> _onStackClickCallback;
};
