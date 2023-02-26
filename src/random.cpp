#include "random.hpp"
#include <chrono>
#include <cmath>

namespace ur {

Random::Random(int min, int max)
{
  // setup the random stuff
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  this->engine = std::default_random_engine(seed);

  // setup distribution
  auto range = float(max - min);
  this->distribution =
    std::normal_distribution<float>(range / 2.f, range / 4.f);
}

int
Random::next()
{
  return std::round(this->distribution(this->engine));
}

}
