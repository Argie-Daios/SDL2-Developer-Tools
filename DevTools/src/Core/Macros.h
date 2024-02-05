#pragma once

#include <iostream>
#include <memory>

#define GAME_ASSERT(x, y) { if(!(x)) { std::cout << y << std::endl; __debugbreak(); } }

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}