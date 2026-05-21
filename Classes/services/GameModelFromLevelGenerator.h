#pragma once
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

/**
 * GameModelFromLevelGenerator 游戏模型生成服务
 * 将静态关卡配置（LevelConfig）转换为运行时数据（GameModel）
 * 无状态服务，使用静态方法，不持有任何数据
 */
class GameModelFromLevelGenerator
{
public:
    /**
     *根据关卡配置生成游戏运行时数据模型
     * @param config 关卡静态配置
     * @return 初始化完成的GameModel
     */
    static GameModel generateGameModel(const LevelConfig &config);
};
