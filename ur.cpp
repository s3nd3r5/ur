#include "helper.hpp"
#include "random.hpp"
#include "timedLatch.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>

const char* TEXTURE_PATH = "./res/ur.png";
const float PAD = 32.f;
const float PIECE_PAD = 8.f;
const float TEXT_OFFSET = 8.f;
const sf::Color BG_COLOR = sf::Color(66, 47, 81, 255);
const sf::Color SEMI_TRANSPARENT = sf::Color(255, 255, 255, 128);

ur::Random dice_rand(0, 1); // 50/50 random
GameState state = GameState::WAITING;
GameState prev_state = GameState::WAITING;

bool mouse_left_locked = false;

inline void
change_state(GameState next)
{
  std::cout << "GameState == " << next << std::endl;
  prev_state = state;
  state = next;
}

// p1 = false, p2 = true
bool turn_tracker = true;
int rolling_frame = 0;

inline void
next(int* i, int max)
{
  (*i) = ((*i) + 1) % max;
}

inline void
next_turn()
{
  turn_tracker = !turn_tracker;
  change_state(GameState::WAITING);
}

inline bool
p1_turn()
{
  return !turn_tracker;
}

inline bool
p2_turn()
{
  return turn_tracker;
}

inline void
render_dice(sf::RenderWindow* window,
            std::shared_ptr<std::vector<struct dice_t>> dice,
            std::shared_ptr<std::vector<sf::Sprite>> roll_sprites,
            ur::TimedLatch* animation_frame_timer)
{

  // draw dice
  int dice_r, dice_c, roll_r, roll_c;
  if (p1_turn()) {
    dice_r = 6;
    dice_c = 11;
    roll_r = dice_r + 2;
    roll_c = dice_c;
  } else {
    dice_r = 1;
    dice_c = 11;
    roll_r = dice_r - 1;
    roll_c = dice_c;
  }
  if (state == GameState::PLACING) {
    int result = 0;
    // draw roll result
    int r = dice_r, c = dice_c;
    for (int i = 0; i < 8; i++) {
      auto& die = (*dice)[i];
      if (die.show) {
        die.sprite.setPosition(pos(c, r));
        result += die.value;
        window->draw(die.sprite);

        if (i % 2 == 0) {
          c += 1;
          // reset if we've already bumped it once
          if (c % 2 == 0) {
            c = dice_c;
          }
        } else {
          r += 1;
        }
      }
    }
  } else if (state == GameState::ROLLING) {
    // if completed update dice sprites
    if (animation_frame_timer->is_completed()) {
      // iterate over each pair of dice sprites
      // and show whichever matches the roll
      for (int i = 0; i < 8; i+=2)
      {
        int result = dice_rand.next();
        (*dice)[i].show = result == 0;
        (*dice)[i+1].show = result == 1;
      }
    }
    int c = dice_c, r = dice_r;
    int i = 0;
    for (auto& die : (*dice))
    {
      if (die.show) {
        die.sprite.setPosition(pos(c++, r));
        window->draw(die.sprite);
        if (++i == 2) {
          c = dice_c;
          r += 1;
        }
      }
    }
    // make sure we're started!
    // note - this should come after the completed check otherwise we'll always restart it
    if (!animation_frame_timer->is_running())
    {
      animation_frame_timer->start();
    }

  } else {
    // draw initial values
    // draw the 0s
    int c = dice_c, r = dice_r;
    for (int i = 0; i < 8; i += 2) {
      auto& die = (*dice)[i];
      die.sprite.setPosition(pos(c++, r));
      window->draw(die.sprite);
      if (i == 2) {
        c = dice_c;
        r += 1;
      }
    }
    c = roll_c, r = roll_r;
    for (auto& s : (*roll_sprites)) {
      s.setPosition(pos(c++, r));
    }
  }
}

int
main()
{
  const std::shared_ptr<std::vector<sf::Texture>> textures =
    loadTextures(TEXTURE_PATH);

  const std::shared_ptr<std::vector<sf::Sprite>> board = createBoard(textures);

  const std::shared_ptr<struct player_t> p1 =
    createPlayer((*textures)[P1_PIECE]);

  const std::shared_ptr<struct player_t> p2 =
    createPlayer((*textures)[P2_PIECE]);

  const std::shared_ptr<std::vector<sf::Sprite>> roll_sprites =
    createRollSprites((*textures)[ROLL_TILES[0]], (*textures)[ROLL_TILES[1]]);

  const std::shared_ptr<std::vector<struct dice_t>> dice =
    createAllDice((*textures)[DIE_0], (*textures)[DIE_1]);

  sf::Sprite p1Score;
  p1Score.setPosition(pos(0, SPRITE_ROWS - 1));
  makeNum(&p1Score, 0, textures);

  sf::Sprite p2Score;
  p2Score.setPosition(pos(0, 0));
  makeNum(&p2Score, 0, textures);

  sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), TITLE);
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);

  sf::View view(window.getDefaultView());
  view.zoom(ZOOM);
  view.setSize(view.getSize() * ZOOM);
  view.setCenter(view.getSize() / 2.f);

  ur::TimedLatch rolling_animation_timer(sf::seconds(3));
  ur::TimedLatch rolling_animation_frame_pause_timer(sf::milliseconds(100));

  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        window.close();
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
          !mouse_left_locked) {
        mouse_left_locked = true;
        // check rolling button click
        window.setView(view);
        auto mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (auto& s : (*roll_sprites)) {
          // zoom sprite bounds
          if (s.getGlobalBounds().contains(mPos)) {
            if (state == GameState::WAITING) {
              std::cout << "Roll!" << std::endl;
              // setup for rolling
              rolling_animation_timer.start();
              change_state(GameState::ROLLING);
              for (auto& rs : (*roll_sprites)) {
                rs.setColor(SEMI_TRANSPARENT);
              }
              for (int i = 0; i < 8; i++)
              {
                (*dice)[i].show = i % 2 == 0; // only show the 0s
              }
              break;
            }
          }
        }
        window.setView(window.getDefaultView()); // reset back to main view
      } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        mouse_left_locked = false;
      }
    }

    window.clear(BG_COLOR);
    window.setView(view);

    for (auto s : *(board)) {
      window.draw(s);
    }

    // draw unused pieces
    int p_num = (SPRITE_COLS / 2) - (p1->pieces->size() / 2) - 1;
    for (auto& p : *(p1->pieces)) {
      p.sprite.setPosition(pos(p_num++, SPRITE_ROWS - 1));
      window.draw(p.sprite);
    }

    p_num = (SPRITE_COLS / 2) - (p2->pieces->size() / 2) - 1;
    for (auto& p : *(p2->pieces)) {
      p.sprite.setPosition(pos(p_num++, 0));
      window.draw(p.sprite);
    }

    render_dice(&window, dice, roll_sprites, &rolling_animation_frame_pause_timer);
    for (auto& s : (*roll_sprites)) {
      window.draw(s);
    }

    window.draw(p1Score);
    window.draw(p2Score);
    window.setView(window.getDefaultView());
    window.display();

    // test timer logic - change turn after timer completes
    if (rolling_animation_timer.is_completed()) {
      rolling_animation_timer.reset();
      for (auto& rs : (*roll_sprites)) {
        rs.setColor(sf::Color::White);
      }
      next_turn();
    }
  }

  return EXIT_SUCCESS;
}
