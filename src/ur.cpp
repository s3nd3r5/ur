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
struct piece_t* grabbed_piece = nullptr;
sf::Vector2f grabbed_piece_origin;

int turn_roll = 0;
bool mouse_left_locked = false;

inline void
change_state(GameState next)
{
  std::cout << "GameState == " << next << std::endl;
  prev_state = state;
  state = next;
}

// tracks the turn pids
int turn_pid = P1_ID;
int rolling_frame = 0;

inline void
next(int* i, int max)
{
  (*i) = ((*i) + 1) % max;
}

inline void
next_turn(std::shared_ptr<std::vector<sf::Sprite>> roll_sprites)
{
  turn_roll = 0;
  turn_pid = turn_pid == P1_ID ? P2_ID : P1_ID;
  for (auto& s : (*roll_sprites)) s.setColor(sf::Color::White);
  change_state(GameState::WAITING);
}

inline bool
p1_turn()
{
  return turn_pid == P1_ID;
}

inline bool
p2_turn()
{
  return turn_pid == P2_ID;
}

inline void
render_dice(sf::RenderWindow* window,
            std::shared_ptr<std::vector<struct dice_t>> dice,
            std::shared_ptr<std::vector<sf::Sprite>> roll_sprites,
            std::shared_ptr<std::vector<sf::Sprite>> pass_sprites,
            std::shared_ptr<std::vector<sf::Texture>> textures,
            sf::Sprite* roll_result,
            ur::TimedLatch* animation_timer,
            ur::TimedLatch* animation_frame_timer)
{

  if (animation_timer->is_completed()) {
    animation_timer->reset();
    int rolls[4] = {
      dice_rand.next(), dice_rand.next(), dice_rand.next(), dice_rand.next()
    };
    // draw roll result
    for (int i = 0; i < 8; i++) {
      auto& die = (*dice)[i];
      die.show = die.value == rolls[i / 2];
    }
    // set roll result
    for (int r : rolls) {
      turn_roll += r;
    }
    makeNum(roll_result, turn_roll, textures);
    if (turn_roll == 0) {
      change_state(GameState::PASSING);
    } else {
      change_state(GameState::PLACING);
    }
  }

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

  if (state == GameState::PLACING || state == GameState::PASSING) {
    // draw roll text
    window->draw(*roll_result);
    if (turn_roll == 0) {
      int psi = SPRITE_COLS / 2 - 1;
      for (auto& ps : (*pass_sprites)) {
        ps.setPosition(pos(psi++, SPRITE_ROWS / 2));
        window->draw(ps);
      }
    }
  } else if (state == GameState::ROLLING) {
    // if completed update dice sprites
    if (animation_frame_timer->is_completed()) {
      // iterate over each pair of dice sprites
      // and show whichever matches the roll
      for (int i = 0; i < 8; i += 2) {
        int result = dice_rand.next();
        (*dice)[i].show = result == 0;
        (*dice)[i + 1].show = result == 1;
      }
    }
    // make sure we're started!
    // note - this should come after the completed check otherwise we'll always
    // restart it
    if (!animation_frame_timer->is_running()) {
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
  }

  int c = dice_c, r = dice_r;
  int i = 0;
  for (auto& die : (*dice)) {
    if (die.show) {
      die.sprite.setPosition(pos(c++, r));
      window->draw(die.sprite);
      if (++i == 2) {
        c = dice_c;
        r += 1;
      }
    }
  }
  c = roll_c, r = roll_r;
  for (auto& s : (*roll_sprites)) {
    s.setPosition(pos(c++, r));
  }
}

int
main()
{
  const std::shared_ptr<std::vector<sf::Texture>> textures =
    loadTextures(TEXTURE_PATH);

  const std::shared_ptr<std::vector<struct board_t>> board =
    createBoard(textures);

  const std::shared_ptr<struct player_t> p1 =
    createPlayer(P1_ID, (*textures)[P1_PIECE]);

  const std::shared_ptr<struct player_t> p2 =
    createPlayer(P2_ID, (*textures)[P2_PIECE]);

  const std::shared_ptr<std::vector<sf::Sprite>> roll_sprites =
    createRollSprites((*textures)[ROLL_TILES[0]], (*textures)[ROLL_TILES[1]]);

  const std::shared_ptr<std::vector<struct dice_t>> dice =
    createAllDice((*textures)[DIE_0], (*textures)[DIE_1]);

  const std::shared_ptr<std::vector<sf::Sprite>> pass_sprites =
    createPassSprites(textures);

  const std::shared_ptr<std::vector<sf::Sprite>> start_sprites =
    createStartSprites(textures);

  sf::Sprite p1Score;
  p1Score.setPosition(pos(0, SPRITE_ROWS - 1));
  makeNum(&p1Score, 0, textures);

  sf::Sprite p2Score;
  p2Score.setPosition(pos(0, 0));
  makeNum(&p2Score, 0, textures);

  // init piece positions
  int p_num = (SPRITE_COLS / 2) - (p1->pieces->size() / 2) - 1;
  for (auto& p : *(p1->pieces)) {
    p.sprite.setPosition(pos(p_num++, SPRITE_ROWS - 1));
    p.origin = p.sprite.getPosition();
  }

  p_num = (SPRITE_COLS / 2) - (p2->pieces->size() / 2) - 1;
  for (auto& p : *(p2->pieces)) {
    p.sprite.setPosition(pos(p_num++, 0));
    p.origin = p.sprite.getPosition();
  }

  sf::Sprite roll_result;
  roll_result.setPosition(pos(12, 4));

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

        // was roll clicked
        if (state == GameState::WAITING) {
          for (auto& s : (*roll_sprites)) {
            // zoom sprite bounds
            if (s.getGlobalBounds().contains(mPos)) {
              std::cout << "Roll!" << std::endl;
              // setup for rolling
              rolling_animation_timer.start();
              change_state(GameState::ROLLING);
              for (auto& rs : (*roll_sprites)) {
                rs.setColor(SEMI_TRANSPARENT);
              }
              for (int i = 0; i < 8; i++) {
                (*dice)[i].show = i % 2 == 0; // only show the 0s
              }
              break;
            }
          }
        } else if (state == GameState::PLACING) {
          // is a piece being clicked
          std::shared_ptr<std::vector<struct piece_t>> pieces;
          if (p1_turn()) {
            pieces = p1->pieces;
          } else {
            pieces = p2->pieces;
          }

          for (auto& p : (*pieces)) {
            if (p.sprite.getGlobalBounds().contains(mPos)) {
              grabbed_piece = &p;
              grabbed_piece_origin = grabbed_piece->sprite.getPosition();
              break;
            }
          }
        } else if (state == GameState::PASSING) {
          for (auto& s : (*pass_sprites)) {
            // zoom sprite bounds
            if (s.getGlobalBounds().contains(mPos)) {
              next_turn(roll_sprites);
              break;
            }
          }
        }
        window.setView(window.getDefaultView()); // reset back to main view
      } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        mouse_left_locked = false;
        // is a piece being clicked
        std::shared_ptr<std::vector<struct piece_t>> pieces, enemyPieces;
        if (p1_turn()) {
          pieces = p1->pieces;
          enemyPieces = p2->pieces;
        } else {
          pieces = p2->pieces;
          enemyPieces = p1->pieces;
        }
        if (state == GameState::PLACING && grabbed_piece != nullptr) {
          // did the piece drop into place
          bool in_place = false;
          sf::FloatRect intersect;
          for (auto& bp : (*board)) {
            auto s = bp.sprite;
            if (s.getGlobalBounds().intersects(
                  grabbed_piece->sprite.getGlobalBounds(), intersect)) {
              if (intersect.width > SPRITE_SIZE / 2 &&
                  intersect.height > SPRITE_SIZE / 2) {
                // check valid placement
                int takenPieceId = -1;
                if (canPlace(grabbed_piece,
                             turn_pid,
                             bp,
                             pieces,
                             enemyPieces,
                             takenPieceId)) {
                  // did we take a piece
                  if (takenPieceId >= 0) {
                    for (auto& ep : (*enemyPieces)) {
                      if (ep.id = takenPieceId) {
                        ep.sprite.setPosition(ep.origin);
                        ep.position = -1;
                      }
                    }
                  }
                  grabbed_piece->sprite.setPosition(s.getPosition());
                  grabbed_piece->position = bp.position;
                  in_place = true;
                  break;
                }
              }
            }
          }

          if (in_place) {
            next_turn(roll_sprites);
          } else {
            grabbed_piece->sprite.setPosition(grabbed_piece_origin);
          }
          grabbed_piece = nullptr;
        }
      }
    }

    window.clear(BG_COLOR);
    window.setView(view);

    for (auto s : *(board)) {
      window.draw(s.sprite);
    }

    auto mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (grabbed_piece != nullptr) {
      float x = mPos.x - (grabbed_piece->sprite.getGlobalBounds().width / 2);
      float y = mPos.y - (grabbed_piece->sprite.getGlobalBounds().height / 2);
      grabbed_piece->sprite.setPosition(x, y);
    }

    // draw pieces (draw own pieces last to ensure "on top")
    if (p1_turn()) {
      for (auto& p : *(p2->pieces)) {
        window.draw(p.sprite);
      }
      for (auto& p : *(p1->pieces)) {
        window.draw(p.sprite);
      }
    } else {
      for (auto& p : *(p1->pieces)) {
        window.draw(p.sprite);
      }
      for (auto& p : *(p2->pieces)) {
        window.draw(p.sprite);
      }
    }

    render_dice(&window,
                dice,
                roll_sprites,
                pass_sprites,
                textures,
                &roll_result,
                &rolling_animation_timer,
                &rolling_animation_frame_pause_timer);
    for (auto& s : (*roll_sprites)) {
      window.draw(s);
    }

    window.draw(p1Score);
    window.draw(p2Score);
    window.setView(window.getDefaultView());
    window.display();
  }

  return EXIT_SUCCESS;
}
