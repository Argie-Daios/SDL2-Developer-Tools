#pragma once

#include <random>
#include <assert.h>

class Random {
public:
    template<typename Type>
    static Type Randrange(Type max_value)
    {
        assert(false);
        return Type();
    }

    template<>
    static float Randrange<float>(float max_value)
    {
        assert(max_value >= 0.0f);
        std::uniform_real_distribution<float> Distribution(0.0f, max_value);
        return Distribution(generator);
    }

    template<>
    static double Randrange<double>(double max_value)
    {
        assert(max_value >= 0.0);
        std::uniform_real_distribution<double> Distribution(0.0, max_value);
        return Distribution(generator);
    }
private:
    static std::mt19937 generator;
    static std::uniform_real_distribution<double> distribution;
};

std::mt19937 Random::generator = std::mt19937(std::random_device()());
std::uniform_real_distribution<double> Random::distribution = std::uniform_real_distribution<double>(0, 1);

class Propability
{
public:
    static bool Precentage(float precentage)
    {
        precentage = precentage > 100.0f ? 100.0f : precentage;
        return Random::Randrange(100.0f) <= precentage;
    }
};