#include "Parameter.h"

#include <iostream>

Parameter::Parameter(Type type, void* value)
	: m_Type(type), m_Value(value)
{

}

Parameter::Parameter(const Parameter& parameter)
{
	m_Value = parameter.m_Value;
	m_Type = parameter.m_Type;
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