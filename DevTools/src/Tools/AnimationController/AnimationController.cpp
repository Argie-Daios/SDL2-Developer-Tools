#include "AnimationController.h"

#include "ECS/Entity.h"

#include <algorithm>

AnimationController::AnimationController()
{
	
}

void AnimationController::Copy(const AnimationController& animationController, const Entity& entity)
{
	m_Entry = m_Current = animationController.m_Entry;

	for (auto& elem : animationController.m_Animations)
	{
		m_Animations.emplace(elem.first, CreateRef<Animation>(*elem.second));
		m_Animations[elem.first]->m_Entity = entity.m_EntityHandle;
		m_Animations[elem.first]->m_Scene = entity.m_Scene;
	}

	for (auto& elem : animationController.m_Parameters)
	{
		m_Parameters.emplace(elem.first, CreateRef<Parameter>(elem.second->m_Type, CopyPtr(elem.second->m_Value, elem.second->m_Type)));
	}

	for (auto& elem : animationController.m_Links)
	{
		m_Links.emplace(elem.first, std::vector<Edge>());
		for (auto& edge : elem.second)
		{
			m_Links[elem.first].emplace_back(edge.m_DestinationAnimationName, edge.hasExitTime);
			for (auto& cond : edge.m_Conditions)
			{
				std::string parameterName = FindParameter(animationController, cond.m_Parameter);
				m_Links[elem.first].back().m_Conditions.emplace_back(*m_Parameters[parameterName], cond.m_Operation,
					CopyPtr(cond.m_Value, cond.m_ValueType), cond.m_ValueType);
			}
		}
	}
}

void AnimationController::Update()
{
	if (m_Current.empty()) return;

	auto elem = m_Animations[m_Current];

	// std::cout << "Current Animation : " << m_Entry << std::endl;

	auto links = m_Links[m_Current];

	if (links.empty()) return;

	for (auto edge : links)
	{
		if (!edge.GetExitTime() && edge.m_Conditions.empty()) continue;
		bool canPass = true;
		for (auto condition : edge.m_Conditions)
		{
			if (!condition.CheckCondition())
			{
				canPass = false;
				break;
			}
		}
		if ((!edge.GetExitTime() && canPass) || (elem->isComplete() && canPass))
		{
			elem->timeElapsed = 0.0f;
			m_Current = edge.m_DestinationAnimationName;
			return;
		}
	}

}

Ref<Animation> AnimationController::GetCurrentAnimation()
{
	if (m_Current.empty())
	{
		return nullptr;
	}

	return m_Animations[m_Current];
}

void AnimationController::AddAnimation(std::string name, Ref<Animation> animation)
{
	std::string finalName = CheckName(name);

	m_Animations.emplace(name, animation);
	m_Links.emplace(name, std::vector<Edge>({}));

	if (m_Animations.size() == 1)
	{
		m_Entry = name;
		m_Current = m_Entry;
	}
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

std::string AnimationController::FindParameter(const AnimationController& animationController, Parameter* parameter)
{
	for (auto& param : animationController.m_Parameters)
	{
		if (param.second.get() == parameter)
		{
			return param.first;
		}
	}

	GAME_ASSERT(false, "There is no such parameter!!");
	return "";
}