#pragma once
#include "models/CardModel.h"
#include <vector>

/**
 * MatchService 匹配规则服务
 * 提供无状态的牌匹配规则判断
 * 可单例或静态方法调用
 */
class MatchService
{
public:
    /**
     * 判断两张牌是否可以匹配消除
     * 规则：点数差1，无花色限制
     * @param playfieldCard 主牌区的牌
     * @param trayCard 手牌区顶部牌
     * @return 是否可以匹配
     */
    static bool canMatch(const CardModel &playfieldCard, const CardModel &trayCard);

    /**
     * 根据主牌区覆盖关系刷新翻开状态。
     * 被仍在场上的下层相邻牌盖住时不可点击。
     * @param playfieldCards 主牌区所有牌
     */
    static void refreshFaceUpStates(std::vector<CardModel> &playfieldCards);
};
