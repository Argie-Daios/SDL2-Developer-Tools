#include "Random.h"

std::mt19937 Random::generator = std::mt19937(std::random_device()());