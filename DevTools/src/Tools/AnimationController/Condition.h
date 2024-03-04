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
	static bool intEquals(int a, int b);
	static bool intNotEquals(int a, int b);
	static bool intLess(int a, int b);
	static bool intGreater(int a, int b);

	static bool floatEquals(float a, float b);
	static bool floatNotEquals(float a, float b);
	static bool floatLess(float a, float b);
	static bool floatGreater(float a, float b);

	static bool boolEquals(bool a, bool b);

	static Type GetRequiredType(std::string str);

	friend class Condition;
};

constexpr bool isValidOperation(Operation::OperationFunc func, Type type)
{
	switch (type)
	{
	case Type::INT: return func == Operation::OperationFunc::INT_EQUALS || func == Operation::OperationFunc::INT_NOT_EQUALS
		|| func == Operation::OperationFunc::INT_LESS || func == Operation::OperationFunc::INT_GREATER;
	case Type::FLOAT: return func == Operation::OperationFunc::FLOAT_EQUALS || func == Operation::OperationFunc::FLOAT_NOT_EQUALS
		|| func == Operation::OperationFunc::FLOAT_LESS || func == Operation::OperationFunc::FLOAT_GREATER;
	case Type::BOOL: return func == Operation::OperationFunc::BOOL_IS;
	}

	GAME_ASSERT(false, "Not such type");
	return false;
}

class Condition
{
public:
	Condition(Parameter& parameter, Operation::OperationFunc operation, void* value, Type valueType);
	Condition(const Condition&) = default;
	~Condition();
		
	Parameter* GetParameter() { return m_Parameter; }

	bool CheckCondition();
	bool CheckCondition(Parameter* parameter);
private:
	Parameter* m_Parameter;
	Operation::OperationFunc m_Operation;
	void* m_Value;
	Type m_ValueType;

	friend class AnimationController;
};