#pragma once
#include "configs/models/LevelConfig.h"
#include <string>

/**
 * LevelConfigLoader 关卡配置加载器
 * 负责从JSON文件读取并解析关卡配置，返回LevelConfig
 * 无状态，使用静态方法
 */
class LevelConfigLoader
{
public:
    /**
     * 从JSON字符串加载关卡配置
     * @param jsonStr JSON格式的关卡配置字符串
     * @return 解析完成的LevelConfig
     */
    static LevelConfig loadFromJson(const std::string &jsonStr);

    /**
     * 从文件路径加载关卡配置
     * @param filePath 配置文件路径（相对Resources目录）
     * @return 解析完成的LevelConfig
     */
    static LevelConfig loadFromFile(const std::string &filepath);
};