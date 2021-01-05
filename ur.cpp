#include <SFML/Graphics.hpp>
#include <iostream>
#include "helper.h"

const float SCR_W = 800.f;
const float SCR_H = 600.f;
const char* TITLE = "Royal Game of Ur";
const char* TEXTURE_PATH = "./res/ur.png";
const int P1_PIECE = 5;
const int P2_PIECE = 6;
const int P1_BOARD_TILES[2] = { 0, 1 };
const int P2_BOARD_TILES[2] = { 2, 3 };
const int STAR_TILE = 4;
const int BLANK_TILE = 9;
const int DIE_0 = 8;
const int DIE_1 = 7;
const float ZOOM = .5f;
const float PAD = 32.f;
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
  sf::Text p1Text("Player one text here!", font, 24);
  p1Text.setPosition(0.f, 0.f);
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
    window.draw(s);
    window.setView(window.getDefaultView());
    window.draw(p1Text);
    window.display();
  }

  return EXIT_SUCCESS;
}
