#pragma once

#include "Core/Macros.h"

#include <iostream>

class Parameter
{
public:
	Parameter() = default;
	Parameter(Type type, void* value);
	Parameter(const Parameter&);
	~Parameter();

	void SetValue(void* value);

	Type GetType() const { return m_Type; }
	void* GetValue() { return m_Value; }
private:
	Type m_Type;
	void* m_Value;

	friend class AnimationController;
};