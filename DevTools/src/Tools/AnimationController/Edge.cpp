#include "Edge.h"

#include <algorithm>

Edge::Edge(std::string destinationName, bool hasExitTime)
	: m_DestinationAnimationName(destinationName), hasExitTime(hasExitTime)
{

}

Edge::Edge(const Edge& edge)
{
	m_DestinationAnimationName = edge.m_DestinationAnimationName;
	m_Conditions = edge.m_Conditions;
	hasExitTime = edge.hasExitTime;
}

void Edge::AddCondition(Condition condition)
{
	m_Conditions.push_back(condition);
}

void Edge::RemoveCondition(Parameter& parameter)
{
	std::cout << "Vector size before RemoveCondition : " << m_Conditions.size() << std::endl;
	m_Conditions.erase(std::remove_if(m_Conditions.begin(), m_Conditions.end(), [&](Condition condition) {
		return condition.GetParameter() == &parameter;
	}));
	std::cout << "Vector size after RemoveCondition : " << m_Conditions.size() << std::endl;
}