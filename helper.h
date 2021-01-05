#ifndef UR_HELPER_H
#define UR_HELPER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// BOARD LAYOUT [0, 1, 2, 3](start) [4,5,6,7,8,9,10,11](middle), [12,13](end)

static const unsigned int SPRITE_SIZE = 16;
static const unsigned int NUM_PIECES = 7;
static const unsigned int SAFE_SPACE = 7; // 0-indexed
static const unsigned int EXIT_SPACE = 14; // final space + 1

static const sf::Color GLOBAL_MASK(255, 0, 255, 255);

struct piece_t
{
  int id;
  int position;
  sf::Sprite sprite;
};


struct player_t
{
  int score;
  std::shared_ptr<std::vector<std::shared_ptr<struct piece_t>>> pieces;
};


std::shared_ptr<std::vector<sf::Texture>>
loadTextures(const char* path);

sf::Font loadFont();

std::shared_ptr<struct player_t>
createPlayer(sf::Texture& pieceTexture);

std::shared_ptr<struct piece_t>
createPiece(int id, sf::Texture& texture);

bool
clickedPiece(sf::Vector2i mousePosition, std::shared_ptr<struct piece_t> piece);

bool
canMovePiece(
    std::shared_ptr<struct piece_t> piece,
    int roll,
    std::shared_ptr<std::vector<std::shared_ptr<struct piece_t>>> myPieces,
    std::shared_ptr<std::vector<std::shared_ptr<struct piece_t>>> enemyPieces);

std::vector<int> getLegalMoves(std::shared_ptr<struct player_t> activePlayer, std::shared_ptr<struct player_t> opponent);
#endif

