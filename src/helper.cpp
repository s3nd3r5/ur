#include "helper.hpp"
#include <iostream>

std::shared_ptr<std::vector<sf::Texture>>
loadTextures(const char* path)
{
  int sprite_width = SPRITE_SIZE, sprite_height = SPRITE_SIZE;
  sf::Image textureAtlas;
  if (!textureAtlas.loadFromFile(path)) {
    std::cerr << "Unable to load textures from file: " << path << std::endl;
    throw std::runtime_error("Unable to load spritesheet");
  }

  textureAtlas.createMaskFromColor(GLOBAL_MASK);
  auto imageSize = textureAtlas.getSize();

  auto textures = std::make_shared<std::vector<sf::Texture>>();

  for (int y = 0; y < imageSize.y; y += sprite_height) {
    for (int x = 0; x < imageSize.x; x += sprite_width) {
      sf::Texture t;
      t.loadFromImage(textureAtlas,
                      sf::IntRect(x, y, sprite_width, sprite_height));
      textures->push_back(t);
    }
  }
  return textures;
}

// increment through the textures
inline int
next(int* p, int max)
{
  int i = *p;
  (*p) = (i + 1) % max;
  return i;
}

std::shared_ptr<std::vector<struct board_t>>
createBoard(std::shared_ptr<std::vector<sf::Texture>> textures)
{
  auto board = std::make_shared<std::vector<struct board_t>>();
  sf::Texture& star_texture = (*textures)[STAR_TILE];
  int blank_idx = 0;
  int sp_idx = 0;
  /* Positions
   * [3, 2, 1, 0]  [14, 13, 12]
   * [4, 5, 6, 7, 8, 9, 10, 11]
   * [3, 2, 1, 0]  [14, 13, 12]
   * star piece is 7
   * vitory is 14
   */
  // p1 pieces
  // p1 star
  // id's don't matter
  {
    sf::Sprite s;
    s.setTexture(star_texture);
    s.setPosition(pos(3, 5));
    struct board_t bp = { P1_ID, 3, s };
    board->push_back(bp);
  }
  // p1 start
  for (int i = 0; i < 3; i++) {
    sf::Texture& t = (*textures)[P1_BOARD_TILES[next(&sp_idx, 2)]];
    sf::Sprite s;
    s.setTexture(t);
    s.setPosition(pos(4 + i, 5));
    struct board_t bp = { P1_ID, 2 - i, s };
    board->push_back(bp);
  }
  // p1 end
  {
    sf::Sprite goal;
    goal.setTexture((*textures)[P1_END]);
    goal.setPosition(pos(8, 5));
    struct board_t exit_p = { P1_ID, EXIT_SPACE, goal };
    board->push_back(exit_p);

    sf::Sprite end_star;
    end_star.setTexture(star_texture);
    end_star.setPosition(pos(9, 5));
    struct board_t end_p = { P1_ID, EXIT_SPACE - 1, end_star };
    board->push_back(end_p);

    sf::Texture& t = (*textures)[P1_BOARD_TILES[next(&sp_idx, 2)]];
    sf::Sprite s;
    s.setTexture(t);
    s.setPosition(pos(10, 5));
    struct board_t bp = { P1_ID, EXIT_SPACE - 2, s };
    board->push_back(bp);
  }
  // center pieces
  for (int i = 0; i < 8; i++) {
    sf::Sprite s;
    if (i == 3) {
      s.setTexture(star_texture);
    } else {
      sf::Texture& t = (*textures)[BLANK_TILES[next(&blank_idx, 3)]];
      s.setTexture(t);
    }
    s.setPosition(pos(3 + i, 4));
    struct board_t bp = { SHARED_ID, 4 + i, s };
    board->push_back(bp);
  }
  // p2 pieces
  // p2 star
  {
    sf::Sprite s;
    s.setTexture(star_texture);
    s.setPosition(pos(3, 3));
    struct board_t bp = { P2_ID, 3, s };
    board->push_back(bp);
  }
  // p2 start
  for (int i = 0; i < 3; i++) {
    sf::Texture& t = (*textures)[P2_BOARD_TILES[next(&sp_idx, 2)]];
    sf::Sprite s;
    s.setTexture(t);
    s.setPosition(pos(4 + i, 3));
    struct board_t bp = { P2_ID, 2 - i, s };
    board->push_back(bp);
  }
  // p2 end
  {
    sf::Sprite goal;
    goal.setTexture((*textures)[P2_END]);
    goal.setPosition(pos(8, 3));
    struct board_t goal_p = { P2_ID, EXIT_SPACE, goal };
    board->push_back(goal_p);

    sf::Sprite end_star;
    end_star.setTexture(star_texture);
    end_star.setPosition(pos(9, 3));
    struct board_t end_p = { P2_ID, EXIT_SPACE - 1, end_star };
    board->push_back(end_p);

    sf::Texture& t = (*textures)[P2_BOARD_TILES[next(&sp_idx, 2)]];
    sf::Sprite end_tile;
    end_tile.setTexture(t);
    end_tile.setPosition(pos(10, 3));
    struct board_t bp = { P2_ID, EXIT_SPACE - 2, end_tile };
    board->push_back(bp);
  }

  return board;
}

sf::Font
loadFont()
{
  sf::Font font;
  if (!font.loadFromFile("./res/DejaVuSansMono.ttf")) {
    std::cerr << "Unable to load font" << std::endl;
    throw std::runtime_error("Unable to load font");
  }
  return font;
}

std::shared_ptr<struct piece_t>
createPiece(int id, sf::Texture& texture)
{

  sf::Sprite s(texture);
  auto p = std::make_shared<struct piece_t>();
  p->id = id;
  p->position = -1;
  p->sprite = s;

  return p;
}

std::shared_ptr<struct player_t>
createPlayer(const int pid, sf::Texture& texture)
{
  std::shared_ptr<struct player_t> player = std::make_shared<struct player_t>();
  player->pid = pid;
  player->score = 0;
  player->pieces = std::make_shared<std::vector<struct piece_t>>();
  for (int i = 0; i < NUM_PIECES; i++) {
    player->pieces->push_back(*createPiece(i + 1, texture));
  }

  return player;
}

std::shared_ptr<std::vector<struct dice_t>>
createAllDice(sf::Texture& die0Texture, sf::Texture& die1Texture)
{
  auto dice = std::make_shared<std::vector<struct dice_t>>();

  // create dice, even 0 odds 1
  // there are 8 dice results int total
  // 4 potential 0s
  // 4 potential 1s
  // The dice will be rendered in pairs
  // [0, 1] will be the left most die
  // [2, 3] will be the second
  // ... and so on
  // Since a die can only have 2 results (0 or 1)
  // To simplify how we will be placing them, the results of the roll
  // will dictate which value each die gets
  for (int i = 0; i < 8; i++) {
    if (i % 2 == 0) {
      sf::Sprite s;
      s.setTexture(die0Texture);
      struct dice_t die = { 0, true, s };
      dice->push_back(die);
    } else {
      sf::Sprite s;
      s.setTexture(die1Texture);
      struct dice_t die = { 1, false, s };
      dice->push_back(die);
    }
  }

  return dice;
}

std::shared_ptr<std::vector<sf::Sprite>>
createRollSprites(sf::Texture& t1, sf::Texture& t2)
{
  auto sprites = std::make_shared<std::vector<sf::Sprite>>();

  sprites->push_back(sf::Sprite(t1));
  sprites->push_back(sf::Sprite(t2));

  return sprites;
}

std::shared_ptr<std::vector<sf::Sprite>>
createPassSprites(std::shared_ptr<std::vector<sf::Texture>> textures)
{
  auto sprites = std::make_shared<std::vector<sf::Sprite>>();

  for (int i = 0; i < 3; i++) {
    sprites->push_back(sf::Sprite((*textures)[PASS_TILES[i]]));
  }
  return sprites;
}

std::shared_ptr<std::vector<sf::Sprite>>
createStartSprites(std::shared_ptr<std::vector<sf::Texture>> textures)
{
  auto sprites = std::make_shared<std::vector<sf::Sprite>>();

  for (int i = 0; i < 3; i++) {
    sprites->push_back(sf::Sprite((*textures)[START_TILES[i]]));
  }
  return sprites;
}

void
makeNum(sf::Sprite* sprite_ptr,
        int num,
        std::shared_ptr<std::vector<sf::Texture>> textures)
{
  sf::Texture& t = (*textures)[NUMS_TILES[num]];
  sprite_ptr->setTexture(t);
};

bool
clickedPiece(sf::Vector2i mousePosition, struct piece_t* piece)
{
  return piece->sprite.getGlobalBounds().contains(mousePosition.x,
                                                  mousePosition.y);
}

bool
canMovePiece(struct piece_t* piece,
             int roll,
             std::shared_ptr<std::vector<struct piece_t>> myPieces,
             std::shared_ptr<std::vector<struct piece_t>> enemyPieces)
{
  int next = piece->position + roll;

  // rolled passed the exit
  if (next > EXIT_SPACE) {
    return false;
  }

  // colliding with another piece
  for (struct piece_t& p : (*myPieces)) {
    // cannot move onto your own piece
    if (p.id != piece->id && p.position == next) {
      return false;
    }
  }

  // can't attack in safe square
  for (struct piece_t& p : (*enemyPieces)) {
    // cannot move onto a protected enemy piece
    if (next == SAFE_SPACE && p.position == SAFE_SPACE) {
      return false;
    }
  }

  return true;
}

// This function takes in an row and col we want to put the sprite in
// and translate it to a real position in the view
// This is because we've ZOOMed in an that adjusts the entire view.
sf::Vector2f
pos(float c, float r)
{
  return { c * SPRITE_SIZE, r * SPRITE_SIZE };
}

bool
canPlace(struct piece_t* piece,
         int turn_pid,
         struct board_t board_tile,
         std::shared_ptr<std::vector<struct piece_t>> myPieces,
         std::shared_ptr<std::vector<struct piece_t>> opponentPieces,
         int& takenPieceId)
{
  if (board_tile.pid != turn_pid && board_tile.pid != SHARED_ID)
    return false;
  int position = board_tile.position;
  if (position == EXIT_SPACE)
    return true; // can always place in the exit

  // can never collide with your own pieces
  for (auto& p : (*myPieces)) {
    if ((p.id != piece->id) // not the same
        && (p.position == position)) {
      return false; // collides with own pieces
    }
  }
  // can collide with an enemy
  if (position >= 4 && position <= 11) {
    for (auto& p : (*opponentPieces)) {
      // collided
      if (p.position == position) {
        // can not capture on safe space
        if (position == SAFE_SPACE)
          return false;
        takenPieceId = p.id;
        return true;
      }
    }
  }

  return true;
}
