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
static const float ZOOM = 0.5f;
static const float SPRITE_ROWS = 9.f;
static const float SPRITE_COLS = 14.f;
static const float SCR_W = SPRITE_SIZE / ZOOM * SPRITE_COLS / ZOOM;
static const float SCR_H = SPRITE_SIZE / ZOOM * SPRITE_ROWS / ZOOM;
static const int P1_PIECE = 6; 
static const int P2_PIECE = 5;
static const int P1_BOARD_TILES[2] = { 0, 1 };
static const int P2_BOARD_TILES[2] = { 2, 3 };
static const int STAR_TILE = 4;
static const int BLANK_TILE = 9;
static const int DIE_0 = 8;
static const int DIE_1 = 7;
static const int NUMS_TILES[8] = { 10, 11, 12, 13, 14, 15, 16, 17 };
static const int ROLL_TILES[2] = { 18, 19 };

static const char* TITLE = "Royal Game of Ur";
static const sf::Color GLOBAL_MASK(255, 0, 255, 255);

enum GameState
{
  WAITING,
  ROLLING,
  PLACING,
  GAME_OVER
};

struct piece_t
{
  int id;
  int position;
  sf::Sprite sprite;
};


struct player_t
{
  int score;
  std::shared_ptr<std::vector<struct piece_t>> pieces;
};

struct dice_t
{
  int value;
  bool show;
  sf::Sprite sprite;
};


std::shared_ptr<std::vector<sf::Texture>>
loadTextures(const char* path);

std::shared_ptr<std::vector<sf::Sprite>>
createBoard(std::shared_ptr<std::vector<sf::Texture>> textures);

sf::Font loadFont();

std::shared_ptr<struct player_t>
createPlayer(sf::Texture& pieceTexture);

std::shared_ptr<struct piece_t>
createPiece(int id, sf::Texture& texture);

std::shared_ptr<std::vector<struct dice_t>>
createAllDice(sf::Texture& die0Texture, sf::Texture& die1Texture);

std::shared_ptr<std::vector<sf::Sprite>>
createRollSprites(sf::Texture& t1, sf::Texture& t2);

void makeNum(
    sf::Sprite* sprite_ptr,
    int num,
    std::shared_ptr<std::vector<sf::Texture>> textures);

bool
clickedPiece(sf::Vector2i mousePosition, std::shared_ptr<struct piece_t> piece);

bool
canMovePiece(
    std::shared_ptr<struct piece_t> piece,
    int roll,
    std::shared_ptr<std::vector<struct piece_t>> myPieces,
    std::shared_ptr<std::vector<struct piece_t>> enemyPieces);

std::vector<int> 
getLegalMoves(
    std::shared_ptr<struct player_t> activePlayer,
    std::shared_ptr<struct player_t> opponent);

sf::Vector2f
pos(float c, float r);
#endif

