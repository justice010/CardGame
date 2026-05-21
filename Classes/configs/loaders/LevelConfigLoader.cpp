#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;

LevelConfig LevelConfigLoader::loadFromFile(const std::string &filePath)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(fullPath);

    // 临时：如果读取失败用硬编码
    if (jsonStr.empty())
    {
        jsonStr = R"({
            "Playfield": [
                {"CardFace": 12, "CardSuit": 0, "Position": {"x": 250, "y": 1000}},
                {"CardFace": 2, "CardSuit": 0, "Position": {"x": 300, "y": 800}},
                {"CardFace": 2, "CardSuit": 1, "Position": {"x": 350, "y": 600}},
                {"CardFace": 2, "CardSuit": 0, "Position": {"x": 850, "y": 1000}},
                {"CardFace": 2, "CardSuit": 0, "Position": {"x": 800, "y": 800}},
                {"CardFace": 1, "CardSuit": 3, "Position": {"x": 750, "y": 600}}
            ],
            "Stack": [
                {"CardFace": 2, "CardSuit": 0, "Position": {"x": 0, "y": 0}},
                {"CardFace": 0, "CardSuit": 2, "Position": {"x": 0, "y": 0}},
                {"CardFace": 3, "CardSuit": 0, "Position": {"x": 0, "y": 0}}
            ]
        })";
    }

    return loadFromJson(jsonStr);
}

LevelConfig LevelConfigLoader::loadFromJson(const std::string &jsonStr)
{
    LevelConfig config;
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError())
        return config;

    // 解析主牌区
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        for (auto &item : doc["Playfield"].GetArray())
        {
            CardConfig card;
            card.cardFace = item["CardFace"].GetInt();
            card.cardSuit = item["CardSuit"].GetInt();
            if (item.HasMember("Position") && item["Position"].IsObject())
            {
                card.x = item["Position"]["x"].GetFloat();
                card.y = item["Position"]["y"].GetFloat();
            }
            config.playfield.push_back(card);
        }
    }

    // 解析备用牌堆
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        for (auto &item : doc["Stack"].GetArray())
        {
            CardConfig card;
            card.cardFace = item["CardFace"].GetInt();
            card.cardSuit = item["CardSuit"].GetInt();
            if (item.HasMember("Position") && item["Position"].IsObject())
            {
                card.x = item["Position"]["x"].GetFloat();
                card.y = item["Position"]["y"].GetFloat();
            }
            config.stack.push_back(card);
        }
    }
    return config;
}
