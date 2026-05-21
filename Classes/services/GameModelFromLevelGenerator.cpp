#include "GameModelFromLevelGenerator.h"
#include "MatchService.h"

namespace
{
const float kPlayfieldYOffset = 250.0f;
}

GameModel GameModelFromLevelGenerator::generateGameModel(const LevelConfig &config)
{
    GameModel gameModel;
    int idCounter = 0;

    for (size_t i = 0; i < config.playfield.size(); ++i)
    {
        const CardConfig &cardConfig = config.playfield[i];
        CardModel card;
        card.id = idCounter++;
        card.face = static_cast<CardFaceType>(cardConfig.cardFace);
        card.suit = static_cast<CardSuitType>(cardConfig.cardSuit);
        card.x = cardConfig.x;
        card.y = cardConfig.y + kPlayfieldYOffset;
        card.faceUp = true;
        gameModel.playfieldCards.push_back(card);
    }
    MatchService::refreshFaceUpStates(gameModel.playfieldCards);

    for (size_t i = 0; i < config.stack.size(); ++i)
    {
        const CardConfig &cardConfig = config.stack[i];
        CardModel card;
        card.id = idCounter++;
        card.face = static_cast<CardFaceType>(cardConfig.cardFace);
        card.suit = static_cast<CardSuitType>(cardConfig.cardSuit);
        card.x = cardConfig.x;
        card.y = cardConfig.y;
        card.faceUp = false;
        gameModel.stackCards.push_back(card);
    }

    if (!gameModel.stackCards.empty())
    {
        gameModel.trayCard = gameModel.stackCards.back();
        gameModel.trayCard.faceUp = true;
        gameModel.stackCards.pop_back();
    }

    return gameModel;
}
