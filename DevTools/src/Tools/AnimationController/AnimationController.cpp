#include "AnimationController.h"

#include "ECS/Entity.h"
#include "Tools/AssetManager.h"

#include <algorithm>

AnimationController::AnimationController()
{
	
}

void AnimationController::Update(std::string& current, std::unordered_map<std::string, Ref<Parameter>>& parameters) const
{
	if (current.empty()) return;

	auto elem = m_Animations[current];

	// std::cout << "Current Animation : " << m_Entry << std::endl;

	auto links = m_Links[current];

	if (links.empty()) return;

	for (auto edge : links)
	{
		if (!edge.GetExitTime() && edge.m_Conditions.empty()) continue;
		bool canPass = true;
		for (auto condition : edge.m_Conditions)
		{
			if (!condition.CheckCondition(parameters[GetParameterName(condition.GetParameter())].get()))
			{
				canPass = false;
				break;
			}
		}
		if ((!edge.GetExitTime() && canPass) || (elem.isComplete() && canPass))
		{
			elem.Reset();
			current = edge.m_DestinationAnimationName;
			return;
		}
	}

}

AnimationNode AnimationController::GetCurrentAnimation(std::string current) const
{
	GAME_ASSERT(!current.empty(), "Empty");

	return m_Animations[current];
}

void AnimationController::AddAnimation(std::string name, AnimationNode animation)
{
	std::string finalName = CheckName(name);

	m_Animations.emplace(name, animation);
	m_Links.emplace(name, std::vector<Edge>({}));

	if (m_Animations.size() == 1)
	{
		m_Entry = name;
	}
}

void AnimationController::AddTwoSidedEdge(const std::string& source, const std::string& destination, bool hasExitTimeSrc, bool hasExitTimeDst)
{
	AddEdge(source, destination, hasExitTimeDst);
	AddEdge(destination, source, hasExitTimeSrc);
}

void AnimationController::RemoveAnimation(const std::string& name)
{
	GAME_ASSERT(m_Animations.find(name) != m_Animations.end(), "There is no animation with that name : (" + name + ")");

	m_Animations.erase(name);

	auto it = m_Links.find(name);

	bool firstAnimation = it == m_Links.begin();

	it->second.clear();
	m_Links.erase(it);

	for (auto& link : m_Links)
	{
		link.second.erase(std::remove_if(link.second.begin(), link.second.end(), [&](Edge edge) {
			return edge.GetDestination() == name;
		}));
	}

	if (!firstAnimation)
	{
		m_Entry = m_Animations.begin()->first;
		m_Current = m_Entry;
	}
}

void AnimationController::AddEdge(const std::string& source, const std::string& destination, bool hasExitTime)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(!DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") already exists");

	m_Links[source].emplace_back(destination, hasExitTime);
}

void AnimationController::RemoveEdge(const std::string& source, const std::string& destination)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") does not exist");

	int index = FindLink(source, destination);

	GAME_ASSERT(index != -1, "There is no such link from (" + source + ") to (" + destination + ") does not exist");

	m_Links[source].erase(m_Links[source].begin() + index);
}

void AnimationController::AddParameter(const std::string& name, Type type, void* value)
{
	GAME_ASSERT(m_Parameters.find(name) == m_Parameters.end(), "Parameter (" + name + ") already exists");

	m_Parameters[name] = CreateRef<Parameter>(type, value);
}

void AnimationController::AddIntParameter(const std::string& name, int value)
{
	GAME_ASSERT(m_Parameters.find(name) == m_Parameters.end(), "Parameter (" + name + ") already exists");

	m_Parameters[name] = CreateRef<Parameter>(Type::INT, ValueToVoidPtr<int>(value));
}

void AnimationController::AddFloatParameter(const std::string& name, float value)
{
	GAME_ASSERT(m_Parameters.find(name) == m_Parameters.end(), "Parameter (" + name + ") already exists");

	m_Parameters[name] = CreateRef<Parameter>(Type::FLOAT, ValueToVoidPtr<float>(value));
}

void AnimationController::AddBoolParameter(const std::string& name, bool value)
{
	GAME_ASSERT(m_Parameters.find(name) == m_Parameters.end(), "Parameter (" + name + ") already exists");

	m_Parameters[name] = CreateRef<Parameter>(Type::BOOL, ValueToVoidPtr<bool>(value));
}

void AnimationController::RemoveParameter(const std::string& name)
{
	auto it = m_Parameters.find(name);

	GAME_ASSERT(it != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	for (auto& element : m_Links)
	{
		for (auto& edge : element.second)
		{
			edge.m_Conditions.erase(std::remove_if(edge.m_Conditions.begin(), edge.m_Conditions.end(), [&](Condition cond) {
				return it->second.get() == cond.GetParameter();
			}));
		}
	}

	m_Parameters.erase(it);
}

void AnimationController::ChangeParameterValue(const std::string& name, void* value)
{
	GAME_ASSERT(m_Parameters.find(name) != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	m_Parameters[name]->SetValue(value);
}

void AnimationController::ChangeIntParameterValue(const std::string& name, int value)
{
	auto it = m_Parameters.find(name);
	GAME_ASSERT(it != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	Type type = it->second->GetType();
	GAME_ASSERT(type == Type::INT, "Parameter (" + name + ") is of type " + TypeToString(type) + " but provided int");

	m_Parameters[name]->SetValue(ValueToVoidPtr<int>(value));
}

void AnimationController::ChangeFloatParameterValue(const std::string& name, float value)
{
	auto it = m_Parameters.find(name);
	GAME_ASSERT(it != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	Type type = it->second->GetType();
	GAME_ASSERT(type == Type::FLOAT, "Parameter (" + name + ") is of type " + TypeToString(type) + " but provided float");

	m_Parameters[name]->SetValue(ValueToVoidPtr<float>(value));
}

void AnimationController::ChangeBoolParameterValue(const std::string& name, bool value)
{
	auto it = m_Parameters.find(name);
	GAME_ASSERT(it != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	Type type = it->second->GetType();
	GAME_ASSERT(type == Type::BOOL, "Parameter (" + name + ") is of type " + TypeToString(type) + " but provided bool");

	m_Parameters[name]->SetValue(ValueToVoidPtr<bool>(value));
}

void AnimationController::AddConditionOnEdge(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, void* valueToCompare, Type valueToCompareType)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") does not exist");

	auto it = m_Parameters.find(parameter);
	GAME_ASSERT(m_Parameters.find(parameter) != m_Parameters.end(), "Parameter (" + parameter + ") does not exist");
	GAME_ASSERT(it->second->GetType() == valueToCompareType, "Parameter (" + parameter + ") type does not match the type of the value to compare");

	int index = FindLink(source, destination);

	m_Links[source][index].AddCondition(Condition(*m_Parameters[parameter], operation, valueToCompare, valueToCompareType));
}

void AnimationController::AddConditionOnEdgeInt(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, int valueToCompare)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") does not exist");

	auto it = m_Parameters.find(parameter);
	GAME_ASSERT(m_Parameters.find(parameter) != m_Parameters.end(), "Parameter (" + parameter + ") does not exist");
	GAME_ASSERT(it->second->GetType() == Type::INT, "Parameter (" + parameter + ") type does not match the type of the value to compare");

	GAME_ASSERT(isValidOperation(operation, Type::INT), "Invalid operation");

	int index = FindLink(source, destination);

	m_Links[source][index].AddCondition(Condition(*m_Parameters[parameter], operation, ValueToVoidPtr<int>(valueToCompare),
		Type::INT));
}

void AnimationController::AddConditionOnEdgeFloat(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, float valueToCompare)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") does not exist");

	auto it = m_Parameters.find(parameter);
	GAME_ASSERT(m_Parameters.find(parameter) != m_Parameters.end(), "Parameter (" + parameter + ") does not exist");
	GAME_ASSERT(it->second->GetType() == Type::FLOAT, "Parameter (" + parameter + ") type does not match the type of the value to compare");

	GAME_ASSERT(isValidOperation(operation, Type::FLOAT), "Invalid operation");

	int index = FindLink(source, destination);

	m_Links[source][index].AddCondition(Condition(*m_Parameters[parameter], operation, ValueToVoidPtr<float>(valueToCompare),
		Type::FLOAT));
}

void AnimationController::AddConditionOnEdgeBool(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, bool valueToCompare)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") does not exist");

	auto it = m_Parameters.find(parameter);
	GAME_ASSERT(m_Parameters.find(parameter) != m_Parameters.end(), "Parameter (" + parameter + ") does not exist");
	GAME_ASSERT(it->second->GetType() == Type::BOOL, "Parameter (" + parameter + ") type does not match the type of the value to compare");

	GAME_ASSERT(isValidOperation(operation, Type::BOOL), "Invalid operation");

	int index = FindLink(source, destination);

	m_Links[source][index].AddCondition(Condition(*m_Parameters[parameter], operation, ValueToVoidPtr<bool>(valueToCompare),
		Type::BOOL));
}

void AnimationController::RemoveConditionOffEdge(const std::string& source, const std::string& destination, const std::string& parameter)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") does not exist");
	GAME_ASSERT(m_Parameters.find(parameter) != m_Parameters.end(), "Parameter (" + parameter + ") does not exist");

	int index = FindLink(source, destination);

	m_Links[source][index].RemoveCondition(*m_Parameters[parameter]);
}

std::string AnimationController::CheckName(std::string name, int num)
{
	if (m_Animations.find(name) == m_Animations.end())
	{
		return name;
	}

	if (num == 1)
	{
		name += "(1)";
	}
	else
	{
		size_t first = name.find_last_of('(');
		name.replace(first + 1, first + 2, std::to_string(num));
	}

	return CheckName(name, num++);
}

bool AnimationController::DoesLinkExists(const std::string& source, const std::string& destination)
{
	for (auto element : m_Links[source])
	{
		if (element.m_DestinationAnimationName == destination)
			return true;
	}

	return false;
}

int AnimationController::FindLink(const std::string& source, const std::string& destination)
{
	auto& vec = m_Links[source];
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i].m_DestinationAnimationName == destination)
			return i;
	}

	return -1;
}

std::string AnimationController::GetParameterName(Parameter* parameter) const
{
	for (auto param : m_Parameters)
	{
		if (parameter == param.second.get())
		{
			return param.first;
		}
	}

	GAME_ASSERT(false, "No such a parameter");
	return "-";
}