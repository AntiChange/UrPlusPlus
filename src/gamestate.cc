#include <memory>
#include <random>
#include <vector>
#include <algorithm>
#include "gamestate.h"
#include "board.h"

char* InvalidGameStateOperation::what() {
    return "Illegal Operation";
}

void GameState::rollDice() {
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::binomial_distribution<> d(4, 0.5);
    // flexdiceroll = static_cast<size_t>(d(gen));
    // diceroll = static_cast<size_t>(d(gen));

    diceroll = 2;
    flexdiceroll = 2;

    // Check if a valid move exists
    for (size_t i = 0; i < board->playersTokens.at(playerTurn).size(); ++i) {
        if (moveValid(i, diceroll) || moveValid(i, diceroll + 1)
            || moveValid(i, flexdiceroll)) {
            diceAreFresh = true;
            noValidMoves = false;
            return;
        }
    }
    // if there's no valid moves
    noValidMoves = true;
}

void GameState::moveToNextPlayerTurn() {
    diceAreFresh = false;
    playerTurn = (playerTurn + 1) % Board::playerCount;
}

bool GameState::gameIsOver() const {
    // TODO IMPLEMENT ME
    return false;
}

size_t GameState::getWinner() const {
    // maybe throw if not gameIsOver ?
    return winnerPlayer;
}

size_t GameState::getPlayerTurn() const {
    return playerTurn;
}

size_t GameState::getDiceRoll() const {
    return diceroll;
}

size_t GameState::getFlexDiceRoll() const {
    return flexdiceroll;
}

bool GameState::requiresDiceRoll() const {
    return !diceAreFresh;
}

bool GameState::hasNoValidMoves() const {
    return noValidMoves;
}

const std::vector<std::vector<Tile*>>& GameState::getBoard() const {
    return board->getGameMap();
}

const std::vector<std::vector<Token*>>& GameState::getPlayersTokens() const {
    return board->getPlayersTokens();
}

const std::vector<std::vector<Tile*>>& GameState::getPlayersPaths() const {
    return board->getPlayersPaths();
}

std::istream& operator>>(std::istream& in, GameState& g) {
    in >> *(g.board) >> g.playerTurn;

    if (g.playerTurn < 0 || g.playerTurn >= Board::playerCount) {
        // maybe a different exception
        throw BoardParseException{};
    }

    return in;
}

GameState::GameState() : board{std::make_unique<Board>()} {}

void GameState::skipTurn() {
    if (!hasNoValidMoves()) {
        throw InvalidGameStateOperation{};
    }
    moveToNextPlayerTurn();
}

bool GameState::moveValid(size_t tokenId, size_t distance) {
    Token* movingToken = board->playersTokens.at(playerTurn).at(tokenId).get();
    if (!movingToken->isValidMove(diceroll, flexdiceroll, distance)) {
        return false;
    }
    size_t newIndex = movingToken->getPathProgress() + distance - 1;
    if (board->paths.at(playerTurn).size() < newIndex) {
        // Out of bounds move
        return false;
    }
    Tile* newTile = board->paths.at(playerTurn).at(newIndex);
    if (!newTile->tileAvailable(movingToken)) {
        return false;
    }
    return true;
}

bool GameState::movePiece(size_t tokenId, size_t distance) {
    auto &playerTokens = board->playersTokens.at(playerTurn);

    // before any checks, make sure the tokenId is a valid token
    if (std::find_if(playerTokens.begin(), playerTokens.end(), [tokenId](std::unique_ptr<Token> &t) {
        return t->getTokenId() == tokenId;
    }) == playerTokens.end()) {
        return false;
    }

    if (!moveValid(tokenId, distance)) {
        return false;
    }
    Token* movingToken = playerTokens.at(tokenId).get();
    size_t oldIndex = movingToken->getPathProgress() - 1;
    Tile* oldTile = board->paths.at(playerTurn).at(oldIndex);
    if ((oldIndex + distance) == (board->paths.at(playerTurn).size())) {
        // We are moving into the end
        oldTile->setOccupant(nullptr);
        movingToken->updatePosition(std::make_pair(0, 0), movingToken->getPathProgress() + distance);
    } else {
        Tile* newTile = board->paths.at(playerTurn).at(oldIndex + distance);
        if (newTile->getOccupant() != nullptr) {
            // At this point, we are guaranteed this is an enemy token
            // that is capturable.
            // We need to send the enemy token to the start.
            Token* killedToken = newTile->getOccupant();
            killedToken->updatePosition(std::make_pair(0, 0), 0);
        }

        // update tile occupants
        oldTile->setOccupant(nullptr);
        newTile->setOccupant(movingToken);

        // update position stored in token
        movingToken->updatePosition(newTile->getPosition(), movingToken->getPathProgress() + distance);
    }

    // TODO integrate with tile/token abilities that make it so you stay on this players turn
    moveToNextPlayerTurn();
    return true;
}
