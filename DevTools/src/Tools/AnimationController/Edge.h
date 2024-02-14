#pragma once

#include "Condition.h"

#include <vector>

#include <iostream>

class Edge
{
public:
	Edge() = default;
	Edge(std::string destinationName, bool hasExitTime = true);

	void AddCondition(Condition condition);
	void RemoveCondition(Parameter& parameter);

	void SetExitTime(bool value) { hasExitTime = value; }

	bool GetExitTime() const { return hasExitTime; }
private:
	std::string m_DestinationAnimationName;
	std::vector<Condition> m_Conditions;
	bool hasExitTime;

	friend class AnimationController;
};