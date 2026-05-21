# CardGame - 记忆翻牌游戏

基于 Cocos2d-x + C++ 开发的休闲消除类游戏。

## 游戏玩法

从手牌区选择卡牌与主牌区进行匹配消除，点数相差1即可消除，支持撤销操作。

## 项目架构

采用 MVC 分层设计：

- **Model**：CardModel（单张卡牌数据）、GameModel（游戏状态）
- **View**：CardView（卡牌展示）、TrayView（手牌区）、PlayFieldView（主牌区）
- **Controller**：GameController（游戏逻辑）、PlayFieldController（消除逻辑）
- **Manager**：UndoManager（撤销管理）
- **Service**：MatchService（匹配判断）、GameModelFromLevelGenerator（关卡生成）

## 开发环境

- Cocos2d-x 4.x
- C++17
- CMake + Ninja
