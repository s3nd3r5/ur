#ifndef UR_HELPER_H
#define UR_HELPER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// BOARD LAYOUT [0, 1, 2, 3](start) [4,5,6,7,8,9,10,11](middle), [12,13](end)

static const unsigned int SPRITE_SIZE = 16;
static const unsigned int NUM_PIECES = 7;
static const unsigned int SAFE_SPACE = 7;  // 0-indexed
static const unsigned int EXIT_SPACE = 14; // final space + 1
static const float ZOOM = 0.5f;
static const float SPRITE_ROWS = 9.f;
static const float SPRITE_COLS = 14.f;
static const float SCR_W = SPRITE_SIZE / ZOOM * SPRITE_COLS / ZOOM;
static const float SCR_H = SPRITE_SIZE / ZOOM * SPRITE_ROWS / ZOOM;
static const int SHARED_ID = -1;
static const int P1_ID = 0;
static const int P2_ID = 1;
static const int P1_PIECE = 19;
static const int P2_PIECE = 18;
static const int P1_BOARD_TILES[2] = { 0, 1 };
static const int P2_BOARD_TILES[2] = { 2, 3 };
static const int STAR_TILE = 4;
static const int BLANK_TILES[3] = { 5, 6, 7 };
static const int P1_END = 22;
static const int P2_END = 23;
static const int DIE_0 = 17;
static const int DIE_1 = 16;
static const int NUMS_TILES[8] = { 8, 9, 10, 11, 12, 13, 14, 15 };
static const int ROLL_TILES[2] = { 20, 21 };
static const int PASS_TILES[3] = { 24, 25, 26 };
static const int START_TILES[3] = { 27, 28, 29 };
static const int REROLL_POS[3] = { 3, 7, 13 };
static const int P1_WIN_TILES[5] = { 30, 31, 32, 33, 34 };
static const int P2_WIN_TILES[5] = { 35, 36, 37, 38, 39 };

static const char* TITLE = "Royal Game of Ur";
static const sf::Color GLOBAL_MASK(255, 0, 255, 255);

enum GameState
{
  WAITING,
  ROLLING,
  PASSING,
  PLACING,
  GAME_OVER
};

struct piece_t
{
  int id;
  int position;
  sf::Sprite sprite;
  sf::Vector2f origin;
};

struct board_t
{
  int pid; // owning players space -1 for shared
  int position;
  sf::Sprite sprite;
};

struct player_t
{
  int pid;
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

std::shared_ptr<std::vector<struct board_t>>
createBoard(std::shared_ptr<std::vector<sf::Texture>> textures);

sf::Font
loadFont();

std::shared_ptr<struct player_t>
createPlayer(const int pid, sf::Texture& pieceTexture);

std::shared_ptr<struct piece_t>
createPiece(int id, sf::Texture& texture);

std::shared_ptr<std::vector<struct dice_t>>
createAllDice(sf::Texture& die0Texture, sf::Texture& die1Texture);

std::shared_ptr<std::vector<sf::Sprite>>
createRollSprites(sf::Texture& t1, sf::Texture& t2);

std::shared_ptr<std::vector<sf::Sprite>>
createPassSprites(std::shared_ptr<std::vector<sf::Texture>> textures);

std::shared_ptr<std::vector<sf::Sprite>>
createWinSprites(int player_id,
                 std::shared_ptr<std::vector<sf::Texture>> textures);

std::shared_ptr<std::vector<sf::Sprite>>
createStartSprites(std::shared_ptr<std::vector<sf::Texture>> textures);

void
makeNum(sf::Sprite* sprite_ptr,
        int num,
        std::shared_ptr<std::vector<sf::Texture>> textures);

bool
clickedPiece(sf::Vector2i mousePosition, std::shared_ptr<struct piece_t> piece);

bool
canMovePiece(std::shared_ptr<struct piece_t> piece,
             int roll,
             std::shared_ptr<std::vector<struct piece_t>> myPieces,
             std::shared_ptr<std::vector<struct piece_t>> enemyPieces);

bool
hasMoves(std::shared_ptr<struct player_t> activePlayer,
         std::shared_ptr<struct player_t> opponent,
         int roll);

sf::Vector2f
pos(float c, float r);

bool
canPlace(struct piece_t* piece,
         int turn_pid,
         struct board_t board_tile,
         std::shared_ptr<std::vector<struct piece_t>> myPieces,
         std::shared_ptr<std::vector<struct piece_t>> opponentPieces,
         int& takenPieceId);

void
clearPiece(std::shared_ptr<std::vector<struct piece_t>> pieces,
           struct piece_t* piece);

bool
hasWon(std::shared_ptr<struct player_t> player);

#endif
