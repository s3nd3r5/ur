#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "helper.h"

const float SCR_W = 800.f;
const float SCR_H = 600.f;
const char* TITLE = "Royal Game of Ur";
const char* TEXTURE_PATH = "./res/ur.png";
const int P1_PIECE = 6; 
const int P2_PIECE = 5;
const int P1_BOARD_TILES[2] = { 0, 1 };
const int P2_BOARD_TILES[2] = { 2, 3 };
const int STAR_TILE = 4;
const int BLANK_TILE = 9;
const int DIE_0 = 8;
const int DIE_1 = 7;
const float ZOOM = 0.5f;
const float PAD = 32.f;
const float PIECE_PAD = 8.f;
const float TEXT_OFFSET = 8.f;
const sf::Color BG_COLOR = sf::Color(66, 47, 81, 255);

int
main()
{
  const std::shared_ptr<std::vector<sf::Texture>> textures =
    loadTextures(TEXTURE_PATH);
  
  const std::shared_ptr<struct player_t> p1 =
    createPlayer((*textures)[P1_PIECE]);

  const std::shared_ptr<struct player_t> p2 =
    createPlayer((*textures)[P2_PIECE]);

  sf::Font font = loadFont();
  sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), TITLE);

  sf::Text p1ScoreMsg(std::to_string(p1->score), font, 36);
  p1ScoreMsg.setPosition(TEXT_OFFSET, SCR_H - p1ScoreMsg.getGlobalBounds().height - (TEXT_OFFSET * 2));
  auto p1HudY = SCR_H - p1ScoreMsg.getGlobalBounds().height - TEXT_OFFSET - SPRITE_SIZE;
  auto p1PieceOffset =  p1ScoreMsg.getGlobalBounds().width + PAD;

  sf::Text p2ScoreMsg(std::to_string(p2->score), font, 36);
  p2ScoreMsg.setPosition(TEXT_OFFSET, (SPRITE_SIZE / ZOOM / 2.f) - (p2ScoreMsg.getGlobalBounds().height / 2.f));
  auto p2PieceOffset = p2ScoreMsg.getGlobalBounds().width + PAD;
  auto p2HudY = TEXT_OFFSET / 2.f + SPRITE_SIZE;

  sf::View view(window.getDefaultView());
  view.zoom(ZOOM);
  view.setSize(view.getSize() * ZOOM);
  view.setCenter(view.getSize() / 2.f);

  sf::Sprite s((*textures)[0]);
  s.setPosition(0, 0);
  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        window.close();
      }
    }

    window.clear(BG_COLOR);
    window.setView(view);
    int p_num = 0;
    for (auto p : *(p1->pieces))
    {
      p->sprite.setPosition(SPRITE_SIZE + PIECE_PAD + (p_num++ * SPRITE_SIZE), view.getSize().y - SPRITE_SIZE);
      window.draw(p->sprite);
    }
    for (int i = 0; i < 7; i++)
    {
      sf::Sprite s((*textures)[P2_PIECE]);
      s.setPosition(SPRITE_SIZE + PIECE_PAD + (i * SPRITE_SIZE), 0.f);
      window.draw(s);
    }
    window.setView(window.getDefaultView());
    window.draw(p1ScoreMsg);
    window.draw(p2ScoreMsg);
    window.display();
  }

  return EXIT_SUCCESS;
}
