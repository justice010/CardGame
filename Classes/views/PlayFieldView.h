#pragma once
#include "cocos2d.h"
#include "models/CardModel.h"
#include "views/CardView.h"
#include <vector>
#include <functional>
#include <unordered_map>

/**
 * PlayFieldView 主牌区视图
 * 负责显示所有桌面牌，捕获点击事件并通知Controller
 * 不包含任何业务逻辑
 */
class PlayFieldView : public cocos2d::Node
{
public:
    static PlayFieldView *create();
    bool init() override;

    /**
     * 根据GameModel初始化所有牌的视图
     * @param cards 主牌区所有牌的数据
     */
    void initCards(const std::vector<CardModel> &cards);

    /**
     * 播放某张牌移动到手牌区的动画
     * @param cardId    被移动牌的id
     * @param targetPos 目标位置（手牌区顶部牌位置）
     * @param onComplete 动画完成回调
     */
    void playCardMoveToTray(int cardId,
                            cocos2d::Vec2 targetPos,
                            std::function<void()> onComplete = nullptr);

    /**
     * 播放回退动画：牌从手牌区移回桌面
     * @param cardId    要移回的牌id
     * @param targetPos 回退目标位置
     * @param onComplete 动画完成回调
     */
    void playCardMoveBack(int cardId,
                          cocos2d::Vec2 targetPos,
                          std::function<void()> onComplete = nullptr);

    /** 隐藏/显示某张牌（消除后隐藏） */
    void setCardVisible(int cardId, bool visible);

    /** 按最新Model刷新某张牌的牌面/背面显示 */
    void updateCardDisplay(const CardModel &cardModel);

    /** 注册点击回调，Controller调用 */
    void setOnCardClickCallback(std::function<void(int cardId)> callback);

private:
    std::unordered_map<int, CardView *> _cardViews;
    std::function<void(int)> _onCardClickCallback;
};
