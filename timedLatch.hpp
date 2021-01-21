#ifndef UR_TIMEDLATCH_H
#define UR_TIMEDLATCH_H
#include <SFML/System.hpp>

namespace ur {
class TimedLatch
{
public:
  TimedLatch(sf::Time duration);
  void start();
  void reset();
  bool is_running();
  bool is_completed();

private:
  bool isStarted;
  sf::Time duration;
  sf::Clock clock; // internal sfml clock to manage time
};
}

#endif
