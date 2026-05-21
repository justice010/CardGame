#include "MatchService.h"
#include <cmath>

namespace
{
const float kCoverXThreshold = 180.0f;
const float kCoverYThreshold = 260.0f;
}

bool MatchService::canMatch(const CardModel &playfieldCard, const CardModel &trayCard)
{
    if (!playfieldCard.faceUp)
        return false;
    if (playfieldCard.matched)
        return false;

    int diff = std::abs(int(playfieldCard.face) - int(trayCard.face));
    return diff == 1;
}

void MatchService::refreshFaceUpStates(std::vector<CardModel> &playfieldCards)
{
    for (auto &card : playfieldCards)
    {
        if (card.matched)
        {
            card.faceUp = false;
            continue;
        }

        bool covered = false;
        for (const auto &other : playfieldCards)
        {
            if (other.id == card.id || other.matched)
                continue;

            bool isLowerLayer = other.y < card.y;
            bool closeInX = std::abs(other.x - card.x) <= kCoverXThreshold;
            bool closeInY = std::abs(other.y - card.y) <= kCoverYThreshold;
            if (isLowerLayer && closeInX && closeInY)
            {
                covered = true;
                break;
            }
        }

        card.faceUp = !covered;
    }
}
