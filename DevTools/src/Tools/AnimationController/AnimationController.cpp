#include "AnimationController.h"

#include "ECS/Components.h"

#include <algorithm>

AnimationController::AnimationController()
{
	
}

AnimationController::AnimationController(const std::initializer_list<std::pair<std::string, Ref<Animation>>>& animations)
{
	for (auto element : animations)
	{
		AddAnimation(element.first, element.second);
	}

	if(!m_Animations.empty())
		m_Entry = m_Animations.begin()->first;
}

void AnimationController::Update()
{
	if (m_Entry.empty()) return;

	auto elem = m_Animations[m_Entry];

	// std::cout << "Current Animation : " << m_Entry << std::endl;

	if (elem->isComplete())
	{
		auto links = m_Links[m_Entry];

		if (links.empty()) return;

		for (auto edge : links)
		{
			bool canPass = true;
			for (auto condition : edge.m_Conditions)
			{
				if (!condition.CheckCondition())
				{
					canPass = false;
					break;
				}
			}
			if (canPass)
			{
				elem->timeElapsed = 0.0f;
				m_Entry = edge.m_DestinationAnimationName;
				return;
			}
		}
	}
}

Ref<Animation> AnimationController::GetCurrentAnimation()
{
	if (m_Entry.empty())
	{
		return nullptr;
	}

	return m_Animations[m_Entry];
}

void AnimationController::AddAnimation(std::string name, Ref<Animation> animation)
{
	std::string finalName = CheckName(name);

	m_Animations.emplace(name, animation);
	m_Links.emplace(name, std::vector<Edge>({}));

	if (m_Animations.size() == 1)
	{
		m_Entry = name;
	}
}

void AnimationController::RemoveAnimation(const std::string& name)
{
	GAME_ASSERT(m_Animations.find(name) != m_Animations.end(), "There is no animation with that name : (" + name + ")");

	m_Animations.erase(name);

	auto it = m_Links.find(name);

	it->second.clear();
	m_Links.erase(it);

	// Need to update Entry
}

void AnimationController::AddEdge(const std::string& source, const std::string& destination)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(!DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") already exists");

	m_Links[source].emplace_back(destination);
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

void AnimationController::AddConditionOnEdge(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, void* valueToCompare, Type valueToCompareType)
{
	GAME_ASSERT(m_Animations.find(source) != m_Animations.end(), "There is no animation with that name : (" + source + ")");
	GAME_ASSERT(m_Animations.find(destination) != m_Animations.end(), "There is no animation with that name : (" + destination + ")");
	GAME_ASSERT(DoesLinkExists(source, destination), "Link from (" + source + ") to (" + destination + ") does not exist");
	GAME_ASSERT(m_Parameters.find(parameter) != m_Parameters.end(), "Parameter (" + parameter + ") does not exist");

	int index = FindLink(source, destination);

	m_Links[source][index].AddCondition(Condition(*m_Parameters[parameter], operation, valueToCompare, valueToCompareType));
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
		int first = name.find_last_of('(');
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