#pragma once
#include "CardModel.h"
#include <vector>

/**
 * GameModel 游戏运行时数据模型
 * 持有所有牌的状态，是游戏数据的唯一来源
 * 支持序列化以便存档恢复
 */
struct GameModel
{
    std::vector<CardModel> playfieldCards; // 主牌区所有牌
    std::vector<CardModel> stackCards;     // 备用牌堆（从后往前抽）
    CardModel trayCard;                    // 手牌区当前牌
    int score = 0;                         // 当前分数
};
