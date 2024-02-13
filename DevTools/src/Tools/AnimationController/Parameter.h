#pragma once

#include "Tools/Enums.h"
#include "Core/Macros.h"

#include <iostream>

constexpr Type StringToType(std::string type)
{
	if (type == "int") return Type::INT;
	if (type == "float") return Type::FLOAT;
	if (type == "bool") return Type::BOOL;

	GAME_ASSERT(false, "Wrong Type String");
	return Type();
}

template<typename T>
constexpr void* ValueToVoidPtr(T value)
{
	T* val = (T*)malloc(sizeof(T));

	GAME_ASSERT(val, "Failed Memory Allocation");

	*val = value;

	return (void*)val;
}

class Parameter
{
public:
	Parameter() = default;
	Parameter(Type type, void* value);
	~Parameter();

	void SetValue(void* value);

	Type GetType() const { return m_Type; }
	void* GetValue() { return m_Value; }
private:
	Type m_Type;
	void* m_Value;
};