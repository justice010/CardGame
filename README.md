# 纸牌游戏程序设计说明文档

## 一、项目概述

本项目基于 **Cocos2d-x 3.17** 实现，按题目要求完成了以下功能：

1. 手牌区翻牌替换
2. 桌面牌与手牌区顶部牌匹配
3. 连续回退功能

项目整体采用 **MVC 架构**，将视图、逻辑和数据分层组织，便于维护和扩展。

## 二、开发环境

- 引擎版本：`Cocos2d-x 3.17`
- 平台：`Win32`
- 构建工具：`CMake`
- 设计分辨率：`1080 x 2080`

## 三、运行与构建说明

### 1. 直接运行

可直接运行以下目录中的完整运行包：

`build_release/bin/TemplateCpp/Release`

注意：不能只单独复制 `TemplateCpp.exe`，需要连同同目录下的 `dll`、`res`、`fonts`、`level1.json` 等资源一起使用。

### 2. 本地构建

在项目根目录执行：

```bash
cmake --fresh -S . -B build_release -G "Visual Studio 17 2022" -A Win32 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES=Release
cmake --build build_release --config Release
```

构建完成后，可执行文件位于：

`build_release/bin/TemplateCpp/Release/TemplateCpp.exe`

## 四、目录结构

```text
Classes/
├── configs/              # 静态配置相关类
│   ├── models/
│   │   └── LevelConfig.h
│   └── loaders/
│       ├── LevelConfigLoader.h
│       └── LevelConfigLoader.cpp
│
├── models/               # 运行时动态数据模型
│   ├── CardModel.h
│   ├── GameModel.h
│   └── UndoModel.h
│
├── views/                # 视图层，只负责显示与输入
│   ├── CardView.h/cpp
│   ├── PlayFieldView.h/cpp
│   └── TrayView.h/cpp
│
├── controllers/          # 控制器层，协调 Model 与 View
│   ├── GameController.h/cpp
│   ├── PlayFieldController.h/cpp
│   └── StackController.h/cpp
│
├── managers/             # 管理器层
│   ├── UndoManager.h
│   └── UndoManager.cpp
│
├── services/             # 无状态服务层
│   ├── GameModelFromLevelGenerator.h/cpp
│   └── MatchService.h/cpp
│
└── utils/                # 通用工具类
```

## 五、架构说明

### 1. Model

负责维护游戏运行时状态：

- `CardModel`：单张牌数据，包含点数、花色、位置、是否翻开、是否已消除等
- `GameModel`：全局游戏状态，包括主牌区、备用牌堆、手牌区顶部牌
- `UndoModel`：回退记录栈

### 2. View

负责界面展示与输入响应：

- `CardView`：单张牌显示与移动动画
- `PlayFieldView`：桌面主牌区显示
- `TrayView`：手牌区顶部牌与备用牌堆显示

视图层通过回调把点击事件通知给控制器，不直接处理业务逻辑。

### 3. Controller

负责业务逻辑调度：

- `GameController`：初始化游戏流程并协调子控制器
- `PlayFieldController`：处理主牌区点击、匹配和回退
- `StackController`：处理备用牌抽牌和相关回退

### 4. Manager

- `UndoManager`：维护回退记录，并在执行撤销后通知控制器刷新视图

### 5. Service

- `GameModelFromLevelGenerator`：将关卡配置转换为运行时模型
- `MatchService`：处理匹配规则与主牌区翻开状态刷新

## 六、功能实现说明

### 1. 需求1：手牌区翻牌替换

点击备用牌堆后：

- 记录一次 `STACK_TO_TRAY` 回退信息
- 从备用牌堆取出顶部牌
- 新牌以简单 `MoveTo` 动画移动到手牌区顶部牌位置
- 替换当前手牌区顶部牌

### 2. 需求2：桌面牌与手牌区顶部牌匹配

点击主牌区可操作的翻开牌后：

- 使用 `MatchService::canMatch()` 判断是否满足点数差 1
- 匹配成功后记录一次 `PLAYFIELD_TO_TRAY` 回退信息
- 点击牌以简单 `MoveTo` 动画移动到手牌区顶部牌位置
- 替换当前手牌区顶部牌

当前实现中，主牌区支持题目要求的覆盖逻辑：

- 被下层相邻牌覆盖的牌不可点击
- 消除遮挡牌后，会自动刷新并翻开新的可操作牌

### 3. 需求3：回退功能

点击回退按钮后：

- 从 `UndoModel` 栈顶取出最近一次操作记录
- 还原 `trayCard`
- 若为主牌区移动，则将牌还原回原位置并播放反向 `MoveTo`
- 若为备用牌堆翻牌，则将当前手牌区牌回退到备用牌堆，并恢复原顶部牌
- 支持连续多次回退，直到没有回退记录

## 七、组件通信流程

### 1. 点击主牌区卡牌

```text
用户点击卡牌
    ↓
PlayFieldView 捕获点击
    ↓
PlayFieldController::handleCardClick(cardId)
    ↓
MatchService 判断是否匹配
    ↓
UndoManager 记录回退
    ↓
更新 GameModel
    ↓
PlayFieldView 播放移动动画
    ↓
StackController 更新手牌区显示
```

### 2. 点击备用牌堆

```text
用户点击备用牌堆
    ↓
TrayView 回调给 StackController
    ↓
UndoManager 记录回退
    ↓
更新 GameModel
    ↓
TrayView 播放移动动画
```

### 3. 点击回退按钮

```text
用户点击回退
    ↓
GameController::handleUndo()
    ↓
UndoManager::undo(gameModel)
    ↓
按回退类型分发给 PlayFieldController / StackController
    ↓
刷新视图并播放反向动画
```

## 八、扩展说明

### 1. 如何新增一种卡牌

以新增 Joker 为例：

1. 在 `models/CardModel.h` 中扩展点数枚举
2. 在 `Resources/res/number/` 中补充资源图片
3. 在 `views/CardView.cpp` 中补充点数到资源名的映射
4. 在 `services/MatchService.cpp` 中补充 Joker 的特殊匹配规则
5. 在关卡配置 `level1.json` 中直接加入新卡牌配置

这样可以在不修改主要控制流程的情况下完成扩展。

### 2. 如何新增一种回退类型

以新增“合并牌堆”回退为例：

1. 在 `models/UndoModel.h` 中新增 `UndoRecordType`
2. 在 `UndoRecord` 中补充该操作所需的快照字段
3. 在 `UndoManager` 中新增记录方法
4. 在 `UndoManager::undo()` 中增加还原逻辑
5. 在 `GameController` 中补充回退后的分发处理

这样扩展点集中在回退模型、管理器和总控制器中，便于维护。

## 九、说明

本项目重点放在：

- 按题目要求完成核心玩法和回退流程
- 使用清晰的分层结构组织代码
- 为后续扩展卡牌种类和操作类型保留清晰入口
