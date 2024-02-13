#pragma once

#include "Parameter.h"
#include "Edge.h"

#include <unordered_map>
#include <vector>
#include <list>
#include <memory>

struct Animation;

class AnimationController
{
public:
	AnimationController();
	AnimationController(const std::initializer_list<std::pair<std::string, Ref<Animation>>>& animations);

	void Update();

	Ref<Animation> GetCurrentAnimation();

	void AddAnimation(std::string name, Ref<Animation> animation);
	void RemoveAnimation(const std::string& name);

	void AddEdge(const std::string& source, const std::string& destination);
	void RemoveEdge(const std::string& source, const std::string& destination);

	void AddParameter(const std::string& name, Type type, void* value);
	void RemoveParameter(const std::string& name);
	void ChangeParameterValue(const std::string& name, void* value);

	void AddConditionOnEdge(const std::string& source, const std::string& destination, const std::string& parameter,
		Operation::OperationFunc operation, void* valueToCompare, Type valueToCompareType);
	void RemoveConditionOffEdge(const std::string& source, const std::string& destination, const std::string& parameter);
private:
	std::string CheckName(std::string name, int num = 1);
	bool DoesLinkExists(const std::string& source, const std::string& destination);
	int FindLink(const std::string& source, const std::string& destination);
private:
	std::string m_Entry;

	std::unordered_map<std::string, Ref<Animation>> m_Animations;
	std::unordered_map<std::string, std::vector<Edge>> m_Links;
	std::unordered_map<std::string, Ref<Parameter>> m_Parameters;

	friend class Edge;
	friend class Condition;
	friend class Parameter;
};