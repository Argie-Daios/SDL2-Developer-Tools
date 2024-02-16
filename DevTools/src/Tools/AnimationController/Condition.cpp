#include "Condition.h"
#include "Core/Macros.h"
#include "AnimationController.h"

// Operation

bool Operation::intEquals(int a, int b)
{
	return a == b;
}

bool Operation::intNotEquals(int a, int b)
{
	return a != b;
}

bool Operation::intLess(int a, int b)
{
	return a < b;
}

bool Operation::intGreater(int a, int b)
{
	return a > b;
}

bool Operation::floatEquals(float a, float b)
{
	return a == b;
}

bool Operation::floatNotEquals(float a, float b)
{
	return a != b;
}

bool Operation::floatLess(float a, float b)
{
	return a < b;
}
bool Operation::floatGreater(float a, float b)
{
	return a > b;
}

bool Operation::boolEquals(bool a, bool b)
{
	return a == b;
}

static Type OperationFuncToType(Operation::OperationFunc opFunc)
{
	switch (opFunc)
	{
	case Operation::OperationFunc::INT_EQUALS:
	case Operation::OperationFunc::INT_NOT_EQUALS:
	case Operation::OperationFunc::INT_LESS:
	case Operation::OperationFunc::INT_GREATER: return Type::INT;

	case Operation::OperationFunc::FLOAT_EQUALS:
	case Operation::OperationFunc::FLOAT_NOT_EQUALS:
	case Operation::OperationFunc::FLOAT_LESS:
	case Operation::OperationFunc::FLOAT_GREATER: return Type::FLOAT;

	case Operation::OperationFunc::BOOL_IS: return Type::BOOL;
	}

	GAME_ASSERT(false, "Wrong Operation Function");
	return Type();
}

Type Operation::GetRequiredType(std::string str)
{
	size_t first = str.find_first_of('(');
	size_t comma = str.find_first_of(',', first);

	std::string type = str.substr(first + 1, comma - first - 1);

	return StringToType(type);
}

// Condition

Condition::Condition(Parameter& parameter, Operation::OperationFunc operation, void* value, Type valueType)
	: m_Parameter(&parameter), m_Operation(operation), m_Value(value), m_ValueType(valueType)
{
	// Validate types
	GAME_ASSERT(m_Parameter->GetType() == m_ValueType && m_ValueType == OperationFuncToType(m_Operation), "Condition Type Differs");
}

bool Condition::CheckCondition()
{
	switch (m_ValueType)
	{
	case Type::INT:
	{
		int a = *(int*)m_Parameter->GetValue();
		int b = *(int*)m_Value;

		switch (m_Operation)
		{
		case Operation::OperationFunc::INT_EQUALS: return Operation::intEquals(a, b);
		case Operation::OperationFunc::INT_NOT_EQUALS: return Operation::intNotEquals(a, b);
		case Operation::OperationFunc::INT_LESS: return Operation::intLess(a, b);
		case Operation::OperationFunc::INT_GREATER: return Operation::intGreater(a, b);
		}

		GAME_ASSERT(false, "Wrong Operation Function While Checking Condition (type : int)");
	}
	case Type::FLOAT:
	{
		float a = *(float*)m_Parameter->GetValue();
		float b = *(float*)m_Value;

		switch (m_Operation)
		{
		case Operation::OperationFunc::FLOAT_EQUALS: return Operation::floatEquals(a, b);
		case Operation::OperationFunc::FLOAT_NOT_EQUALS: return Operation::floatNotEquals(a, b);
		case Operation::OperationFunc::FLOAT_LESS: return Operation::floatLess(a, b);
		case Operation::OperationFunc::FLOAT_GREATER: return Operation::floatGreater(a, b);
		}

		GAME_ASSERT(false, "Wrong Operation Function While Checking Condition (type : float)");
	}
	case Type::BOOL:
	{
		bool a = *(bool*)m_Parameter->GetValue();
		bool b = *(bool*)m_Value;

		switch (m_Operation)
		{
		case Operation::OperationFunc::BOOL_IS: return Operation::boolEquals(a, b);
		}

		GAME_ASSERT(false, "Wrong Operation Function While Checking Condition (type : bool)");
	}
	}

	GAME_ASSERT(false, "Wrong Type While Checking Condtion");
	return false;
}