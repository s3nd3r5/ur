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

sf::Font loadFont()
{
  sf::Font font;
  if (!font.loadFromFile("./res/DejaVuSansMono.ttf"))
  {
    std::cerr << "Unable to load font" << std::endl;
    throw std::runtime_error("Unable to load font");
  }
  return font;
}

std::shared_ptr<struct piece_t>
createPiece(int id, sf::Texture texture)
{
  std::shared_ptr<sf::Sprite> s = std::make_shared<sf::Sprite>();
  s->setTexture(texture);

  auto p = std::make_shared<struct piece_t>();
  p->id = id;
  p->sprite = s;

  return p;
}

std::shared_ptr<struct player_t>
createPlayer(sf::Texture texture)
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

bool
clickedPiece(sf::Vector2i mousePosition, struct piece_t* piece)
{
  return piece->sprite->getGlobalBounds().contains(mousePosition.x,
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
