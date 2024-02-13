#pragma once

#include "Condition.h"

#include <vector>

#include <iostream>

class Edge
{
public:
	Edge() = default;
	Edge(std::string destinationName);

	void AddCondition(Condition condition);
	void RemoveCondition(Parameter& parameter);
private:
	std::string m_DestinationAnimationName;
	std::vector<Condition> m_Conditions;

	friend class AnimationController;
};