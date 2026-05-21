#pragma once
#include <vector>
#include <string>

/**
 * CardConfig 单张牌的静态配置数据
 * 存储关卡配置文件中每张牌的花色、点数和初始位置
 */
struct CardConfig
{
    int cardFace; // 点数 对应CardFaceType
    int cardSuit; // 花色 对应CardSuitType
    float x = 0.0f;
    float y = 0.0f;
};

/**
 * LevelConfig 关卡静态配置
 * 存储从JSON文件解析出的关卡数据，只读，不包含运行时状态
 */
struct LevelConfig
{
    std::vector<CardConfig> playfield; // 主牌区牌配置
    std::vector<CardConfig> stack;     // 备用牌堆配置
};
