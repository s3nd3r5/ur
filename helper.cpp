#include "helper.h"
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
next(int* p)
{
  int i = *p;
  (*p) = (i + 1) % 2;
  return i;
}

std::shared_ptr<std::vector<sf::Sprite>>
createBoard(std::shared_ptr<std::vector<sf::Texture>> textures)
{
  auto sprites = std::make_shared<std::vector<sf::Sprite>>();
  sf::Texture& star_texture = (*textures)[STAR_TILE];
  sf::Texture& blank_texture = (*textures)[BLANK_TILE];
  int sp_idx = 0;
  // p1 pieces
  // p1 star
  {
    sf::Sprite s;
    s.setTexture(star_texture);
    s.setPosition(pos(3, 5));
    sprites->push_back(s);
  }
  // p1 start
  for (int i = 0; i < 3; i++) {
    sf::Texture& t = (*textures)[P1_BOARD_TILES[next(&sp_idx)]];
    sf::Sprite s;
    s.setTexture(t);
    s.setPosition(pos(4 + i, 5));
    sprites->push_back(s);
  }
  // p1 end
  {
    sf::Sprite end_star;
    end_star.setTexture(star_texture);
    end_star.setPosition(pos(9, 5));
    sprites->push_back(end_star);

    sf::Texture& t = (*textures)[P1_BOARD_TILES[next(&sp_idx)]];
    sf::Sprite s;
    s.setTexture(t);
    s.setPosition(pos(10, 5));
    sprites->push_back(s);
  }
  // center pieces
  for (int i = 0; i < 8; i++) {
    sf::Sprite s;
    if (i == 3) {
      s.setTexture(star_texture);
    } else {
      s.setTexture(blank_texture);
    }
    s.setPosition(pos(3 + i, 4));
    sprites->push_back(s);
  }
  // p2 pieces
  // p2 star
  {
    sf::Sprite s;
    s.setTexture(star_texture);
    s.setPosition(pos(3, 3));
    sprites->push_back(s);
  }
  // p2 start
  for (int i = 0; i < 3; i++) {
    sf::Texture& t = (*textures)[P2_BOARD_TILES[next(&sp_idx)]];
    sf::Sprite s;
    s.setTexture(t);
    s.setPosition(pos(4 + i, 3));
    sprites->push_back(s);
  }
  // p2 end
  {
    sf::Sprite end_star;
    end_star.setTexture(star_texture);
    end_star.setPosition(pos(9, 3));
    sprites->push_back(end_star);

    sf::Texture& t = (*textures)[P2_BOARD_TILES[next(&sp_idx)]];
    sf::Sprite s;
    s.setTexture(t);
    s.setPosition(pos(10, 3));
    sprites->push_back(s);
  }

  return sprites;
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
  p->sprite = s;

  return p;
}

std::shared_ptr<struct player_t>
createPlayer(sf::Texture& texture)
{
  std::shared_ptr<struct player_t> player = std::make_shared<struct player_t>();
  player->score = 0;
  player->pieces =
    std::make_shared<std::vector<std::shared_ptr<struct piece_t>>>();
  for (int i = 0; i < NUM_PIECES; i++) {
    player->pieces->push_back(createPiece(i + 1, texture));
  }

  return player;
}

std::shared_ptr<std::vector<std::shared_ptr<struct dice_t>>>
createAllDice(sf::Texture& die0Texture, sf::Texture& die1Texture)
{
  auto dice = std::make_shared<std::vector<std::shared_ptr<struct dice_t>>>();

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
      auto die = std::make_shared<struct dice_t>();
      die->value = 0;
      die->show = true;
      die->sprite = s;
      dice->push_back(die);
    } else {
      sf::Sprite s;
      s.setTexture(die1Texture);
      auto die = std::make_shared<struct dice_t>();
      die->value = 1;
      die->show = false;
      die->sprite = s;
      dice->push_back(die);
    }
  }

  return dice;
}

std::shared_ptr<std::vector<std::shared_ptr<sf::Sprite>>>
createRollSprites(sf::Texture& t1, sf::Texture& t2)
{
  auto sprites = std::make_shared<std::vector<std::shared_ptr<sf::Sprite>>>();

  auto s1 = std::make_shared<sf::Sprite>();
  s1->setTexture(t1);

  auto s2 = std::make_shared<sf::Sprite>();
  s2->setTexture(t2);

  sprites->push_back(s1);
  sprites->push_back(s2);

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
canMovePiece(
  struct piece_t* piece,
  int roll,
  std::shared_ptr<std::vector<std::shared_ptr<struct piece_t>>> myPieces,
  std::shared_ptr<std::vector<std::shared_ptr<struct piece_t>>> enemyPieces)
{
  int next = piece->position + roll;

  // rolled passed the exit
  if (next > EXIT_SPACE) {
    return false;
  }

  // colliding with another piece
  for (std::shared_ptr<struct piece_t> p : (*myPieces)) {
    // cannot move onto your own piece
    if (p->id != piece->id && p->position == next) {
      return false;
    }
  }

  // can't attack in safe square
  for (std::shared_ptr<struct piece_t> p : (*enemyPieces)) {
    // cannot move onto a protected enemy piece
    if (next == SAFE_SPACE && p->position == SAFE_SPACE) {
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
