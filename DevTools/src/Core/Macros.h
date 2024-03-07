#pragma once

#include <iostream>
#include <memory>
#include <sstream>

#include "Tools/Random.h"
#include "Tools/Enums.h"

#ifdef GAME_DEBUG
	#define GAME_ASSERT(x, y) { if(!(x)) { std::cout << y << std::endl; __debugbreak(); } }
#else
	#define GAME_ASSERT(x, y) { if(!(x)) {} }
#endif

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

constexpr Type StringToType(std::string type)
{
	if (type == "int") return Type::INT;
	if (type == "float") return Type::FLOAT;
	if (type == "bool") return Type::BOOL;

	GAME_ASSERT(false, "Wrong Type String");
	return Type();
}

constexpr std::string TypeToString(Type type)
{
	switch (type)
	{
	case Type::INT: return "int";
	case Type::FLOAT: return "float";
	case Type::BOOL: return "bool";
	}

	GAME_ASSERT(false, "Wrong Type");
	return std::string();
}

template<typename T>
constexpr void* ValueToVoidPtr(T value)
{
	T* val = (T*)malloc(sizeof(T));

	GAME_ASSERT(val, "Failed Memory Allocation");

	*val = value;

	return (void*)val;
}

constexpr void* CopyPtr(void* value, Type type)
{
	switch (type)
	{
	case Type::INT:
	{
		return ValueToVoidPtr<int>(*((int*)value));
	}
	case Type::FLOAT:
	{
		return ValueToVoidPtr<float>(*((float*)value));
	}
	case Type::BOOL:
	{
		return ValueToVoidPtr<bool>(*((bool*)value));
	}
	}

	GAME_ASSERT(false, "Wrong Type");
	return 0;
}

constexpr size_t SizeOf(Type type)
{
	switch (type)
	{
	case Type::INT: return sizeof(int);
	case Type::FLOAT: return sizeof(float);
	case Type::BOOL: return sizeof(bool);
	}

	GAME_ASSERT(false, "Wrong Type");
	return 0;
}