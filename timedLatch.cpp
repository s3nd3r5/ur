#include "timedLatch.hpp"

namespace ur {

TimedLatch::TimedLatch(sf::Time duration)
{
  this->duration = duration;
  this->clock = sf::Clock();
};

void
TimedLatch::start()
{
  this->clock.restart();
  this->isStarted = true;
};

bool
TimedLatch::is_running()
{
  return this->isStarted && this->clock.getElapsedTime() < duration;
};

bool
TimedLatch::is_completed()
{

  return this->isStarted && this->clock.getElapsedTime() >= duration;
};

void
TimedLatch::reset()
{
  this->isStarted = false;
};

}
