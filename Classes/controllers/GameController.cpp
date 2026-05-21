#include "GameController.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

void GameController::startGame(Node *parent,
                               const std::string &levelFile)
{
    // 1. 加载关卡配置
    LevelConfig config = LevelConfigLoader::loadFromFile(levelFile);

    // 2. 生成运行时数据
    _gameModel = GameModelFromLevelGenerator::generateGameModel(config);

    // 3. 初始化管理器
    _undoManager.init(_undoModel);

    // 4. 初始化子控制器
    initControllers(parent);

    // 5. 添加回退按钮
    setupUndoButton(parent);
}

void GameController::initControllers(Node *parent)
{
    // 初始化主牌区控制器
    _playFieldController.init(_gameModel, _undoManager);
    _playFieldController.initView(parent);

    // 匹配成功后更新手牌区显示
    _playFieldController.onMatchSuccess = [this](const CardModel &movedCard)
    {
        _gameModel.trayCard = movedCard;
        _stackController.updateTrayDisplay();
    };

    _stackController.init(_gameModel, _undoManager);
    _stackController.initView(parent);

    _playFieldController.getTrayCardWorldPosition = [this]()
    {
        return _stackController.getView()->getTrayCardWorldPosition();
    };

    _undoManager.onUndoSuccess = [this](const UndoRecord &record)
    {
        if (record.type == UndoRecordType::PLAYFIELD_TO_TRAY)
        {
            _playFieldController.handleUndo(record);
            _stackController.updateTrayDisplay();
        }
        else if (record.type == UndoRecordType::STACK_TO_TRAY)
        {
            _stackController.handleUndo(record);
        }
    };
}

void GameController::setupUndoButton(Node *parent)
{
    auto undoBtn = cocos2d::ui::Button::create();
    undoBtn->setTitleText("回退");
    undoBtn->setTitleFontSize(50);
    undoBtn->setPosition(Vec2(900, 150));
    undoBtn->addClickEventListener([this](Ref *)
                                   { handleUndo(); });
    parent->addChild(undoBtn);
}

void GameController::handleUndo()
{
    _undoManager.undo(_gameModel);
}
