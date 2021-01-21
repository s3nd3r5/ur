#ifndef UR_RANDOM_H
#define UR_RANDOM_H

#include <random>
namespace ur {
class Random
{
public:
  Random(int min, int max);
  int next();

private:
  int min;
  int max;
  std::default_random_engine engine;
  std::normal_distribution<float> distribution;
};
}
#endif
