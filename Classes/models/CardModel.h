#pragma once

/**
 * CardSuitType 花色枚举
 */
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS = 0,    // 梅花
    CST_DIAMONDS = 1, // 方块
    CST_HEARTS = 2,   // 红桃
    CST_SPADES = 3,   // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * CardFaceType 点数枚举
 */
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE = 0,
    CFT_TWO = 1,
    CFT_THREE = 2,
    CFT_FOUR = 3,
    CFT_FIVE = 4,
    CFT_SIX = 5,
    CFT_SEVEN = 6,
    CFT_EIGHT = 7,
    CFT_NINE = 8,
    CFT_TEN = 9,
    CFT_JACK = 10,
    CFT_QUEEN = 11,
    CFT_KING = 12,
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * CardModel 单张牌运行时数据模型
 * 存储牌的状态，支持序列化以便存档恢复
 */
struct CardModel
{
    int id = -1;                  // 唯一ID
    CardFaceType face = CFT_NONE; // 点数
    CardSuitType suit = CST_NONE; // 花色
    float x = 0.0f;               // 当前X坐标
    float y = 0.0f;               // 当前Y坐标
    bool faceUp = false;          // 是否翻开
    bool matched = false;         // 是否已消除

    /**
     * 判断是否可以和另一张牌匹配消除
     * 规则：点数相差为1，无花色限制
     */
    bool canMatchWith(const CardModel &other) const
    {
        int diff = face - other.face;
        return diff == 1 || diff == -1;
    }
};