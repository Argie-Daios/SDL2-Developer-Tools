#pragma once

#include <iostream>
#include <memory>
#include <sstream>

#include "Tools/Random.h"

#define GAME_ASSERT(x, y) { if(!(x)) { std::cout << y << std::endl; __debugbreak(); } }

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
constexpr std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::move(out).str();
}

constexpr std::string randomStringGenerator(int size = 0)
{
	GAME_ASSERT(size <= 50, "Overflow");

	std::string str;

	for (int i = 0; i < size; i++)
	{
		if (Random::Randrange(1))
		{
			str += '9' - Random::Randrange(9);
		}
		else
		{
			str += 'z' - Random::Randrange(25);
		}
	}
	
	return str;
}