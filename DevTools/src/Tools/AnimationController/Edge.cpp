#include "Edge.h"

#include <algorithm>

Edge::Edge(std::string destinationName)
	: m_DestinationAnimationName(destinationName)
{

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