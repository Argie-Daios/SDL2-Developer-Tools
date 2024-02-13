#include "Parameter.h"

#include <iostream>

Parameter::Parameter(Type type, void* value)
	: m_Type(type), m_Value(value)
{

}

Parameter::~Parameter()
{
	free(m_Value);
}

void Parameter::SetValue(void* value)
{
	if (m_Value != nullptr) delete m_Value;

	m_Value = value;
}