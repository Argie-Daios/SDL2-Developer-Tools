#pragma once

#include "Parameter.h"
#include "Edge.h"
#include "Tools/AssetManager.h"

#include <unordered_map>
#include <vector>
#include <list>
#include <memory>

struct Animation;
class Entity;

class AnimationController
{
public:
	AnimationController();

	void Update(std::string& current, std::unordered_map<std::string, Ref<Parameter>>& parameters) const;

	AnimationNode GetCurrentAnimation(std::string current) const;

	void AddAnimation(std::string name, AnimationNode animation);
	void RemoveAnimation(const std::string& name);

	void AddEdge(const std::string& source, const std::string& destination, bool hasExitTime = true);
	void AddTwoSidedEdge(const std::string& source, const std::string& destination, bool hasExitTimeSrc = true, bool hasExitTimeDst = true);
	void RemoveEdge(const std::string& source, const std::string& destination);

	void AddParameter(const std::string& name, Type type, void* value);
	void AddIntParameter(const std::string& name, int value);
	void AddFloatParameter(const std::string& name, float value);
	void AddBoolParameter(const std::string& name, bool value);
	void RemoveParameter(const std::string& name);
	void ChangeParameterValue(const std::string& name, void* value);
	void ChangeIntParameterValue(const std::string& name, int value);
	void ChangeFloatParameterValue(const std::string& name, float value);
	void ChangeBoolParameterValue(const std::string& name, bool value);

	void AddConditionOnEdge(const std::string& source, const std::string& destination, const std::string& parameter,
		Operation::OperationFunc operation, void* valueToCompare, Type valueToCompareType);
	void AddConditionOnEdgeInt(const std::string& source, const std::string& destination, const std::string& parameter,
		Operation::OperationFunc operation, int valueToCompare);
	void AddConditionOnEdgeFloat(const std::string& source, const std::string& destination, const std::string& parameter,
		Operation::OperationFunc operation, float valueToCompare);
	void AddConditionOnEdgeBool(const std::string& source, const std::string& destination, const std::string& parameter,
		Operation::OperationFunc operation, bool valueToCompare);
	void RemoveConditionOffEdge(const std::string& source, const std::string& destination, const std::string& parameter);
private:
	std::string CheckName(std::string name, int num = 1);
	bool DoesLinkExists(const std::string& source, const std::string& destination);
	int FindLink(const std::string& source, const std::string& destination);
	std::string GetParameterName(Parameter* parameter) const;
private:
	std::string m_Entry;
	std::string m_Current;

	mutable std::unordered_map<std::string, AnimationNode> m_Animations;
	mutable std::unordered_map<std::string, std::vector<Edge>> m_Links;
	mutable std::unordered_map<std::string, Ref<Parameter>> m_Parameters;

	friend class Edge;
	friend class Condition;                           
	friend class Parameter;
	friend struct Animator;
};