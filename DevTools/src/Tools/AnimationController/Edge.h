#pragma once

#include "Condition.h"

#include <vector>

#include <iostream>

class Edge
{
public:
	Edge() = default;
	Edge(std::string destinationName, bool hasExitTime = true);
	Edge(const Edge& edge);

	void AddCondition(Condition condition);
	void RemoveCondition(Parameter& parameter);

	void SetExitTime(bool value) { hasExitTime = value; }

	std::string GetDestination() const { return m_DestinationAnimationName; }
	bool GetExitTime() const { return hasExitTime; }
private:
	std::string m_DestinationAnimationName;
	std::vector<Condition> m_Conditions;
	bool hasExitTime;

	friend class AnimationController;
};