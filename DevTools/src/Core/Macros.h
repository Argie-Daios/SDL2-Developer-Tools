#pragma once

#include <iostream>
#include <memory>
#include <sstream>

#define GAME_ASSERT(x, y) { if(!(x)) { std::cout << y << std::endl; __debugbreak(); } }

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
static std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::move(out).str();
}