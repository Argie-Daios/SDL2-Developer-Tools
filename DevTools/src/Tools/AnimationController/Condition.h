#pragma once

#include <iostream>

#include <string>
#include <functional>
#include <memory>

#include "Tools/Enums.h"
#include "Parameter.h"

struct Operation
{
public:
	enum class OperationFunc
	{
		INT_EQUALS,
		INT_NOT_EQUALS,
		INT_LESS,
		INT_GREATER,

		FLOAT_EQUALS,
		FLOAT_NOT_EQUALS,
		FLOAT_LESS,
		FLOAT_GREATER,

		BOOL_IS
	};
private:
	static std::function<bool(int a, int b)> intEquals;
	static std::function<bool(int a, int b)> intNotEquals;
	static std::function<bool(int a, int b)> intLess;
	static std::function<bool(int a, int b)> intGreater;

	static std::function<bool(float a, float b)> floatEquals;
	static std::function<bool(float a, float b)> floatNotEquals;
	static std::function<bool(float a, float b)> floatLess;
	static std::function<bool(float a, float b)> floatGreater;

	static std::function<bool(bool a, bool b)> boolEquals;

	static Type GetRequiredType(std::string str);

	friend class Condition;
};

class Condition
{
public:
	Condition(Parameter& parameter, Operation::OperationFunc operation, void* value, Type valueType);

	Parameter* GetParameter() { return m_Parameter; }

	bool CheckCondition();
private:
	Parameter* m_Parameter;
	Operation::OperationFunc m_Operation;
	void* m_Value;
	Type m_ValueType;
};