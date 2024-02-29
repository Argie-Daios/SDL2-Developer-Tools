#pragma once

#include "Tools/AnimationController/AnimationController.h"
#include "Tools/AssetManager.h"
#include "Core/Scene.h"

#include <SDL.h>
#include <string>
#include <glm/glm.hpp>
#include <functional>
#include <list>
#include <entt.hpp>

class Entity;

struct Component
{
public:
	Component(entt::entity entity, Scene* scene) { m_Entity = entity; m_Scene = scene; }

	void SetEntity(entt::entity entity) { m_Entity = entity; }
protected:
	entt::entity m_Entity;
	Scene* m_Scene;
	friend class Entity;
};

struct Children
{
	Children() {};
	Children(const Children& childrenComponent)
	{
		children = childrenComponent.children;
	}
	std::list<entt::entity> children;
}; // Need to rewrite and also handle it when I create prefab

struct Information
{
	std::string name;
	std::vector<std::string> tags;
};

struct Transform : public Component
{
public:
	Transform();
	Transform(const glm::vec2& position, float zValue = 0.0f, float rotation = 0.0f, const glm::vec2& scale = glm::vec2(1.0f, 1.0f));
	Transform(const Transform&);

	void SetPosition(const glm::vec2& position);
	void Translate(const glm::vec2& translation);
	void SetZValue(float zValue);
	void SetRotation(float rotation);
	void SetScale(const glm::vec2& scale);
	void SetFlip(SDL_RendererFlip flip);
	void SetSize(glm::vec2 size) { this->size = size; } // May need to make for the children

	glm::vec2 GetPosition() const { return position; }
	float GetZValue() const { return zValue; }
	float GetRotation() const { return rotation; }
	glm::vec2 GetScale() const { return scale; }
	SDL_RendererFlip GetFlip() const { return flip; }
	glm::vec2 GetSize() const { return size; }
private:
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 size = glm::vec2(0.0f, 0.0f);
	float zValue = 0.0f;
	float rotation = 0.0f;
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	friend class Entity;
};

struct SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	SpriteRenderer(const std::string& textureID);
	SpriteRenderer(const SpriteRenderer&);

	void ChangeTextureID(std::string textureID);
	void SetTintColor(glm::vec3 color) { tintColor = color; }
	void SetSource(SDL_Rect src) { source = src; }

	std::string GetTextureID() const { return textureID; }
	glm::vec3 GetColor() const { return tintColor; }
	SDL_Color GetSDLColor() const { return SDL_Color{ (unsigned char)(int)tintColor.r, (unsigned char)(int)tintColor.g, (unsigned char)(int)tintColor.b }; }
	SDL_Rect GetSource() const { return source; }
private:
	void UpdateSprite();
private:
	std::string textureID;
	glm::vec3 tintColor = glm::ivec3(255.0f, 255.0f, 255.0f);
	SDL_Rect source;
};

struct Animation : public Component
{
public:
	Animation();
	Animation(const std::string& textureID, int currentFrames = 0, int currentRow = 0, int totalFrames = 0, int totalRows = 0, float delay = 100.0f, bool loop = true);
	Animation(const Animation&);

	void Animate();

	bool isComplete();

	std::string GetTextureID() const { return textureID; }
	int GetCurrentFrames() const { return currentFrames; }
	int GetCurrentRow() const { return currentRow; }
	int GetDefaultRow() const { return defaultRow; }
	int GetTotalFrames() const { return totalFrames; }
	int GetTotalRows() const { return totalRows; }
	float GetDelay() const { return delay; }
	bool GetLoop() const { return loop; }

	void ChangeCurrentFrames(int frames) { this->currentFrames = frames; }
	void ChangeCurrentRow(int currentRow) { this->currentRow = currentRow; }
	void ChangeTotalFrames(int totalFrames) { this->totalFrames = totalFrames; }
	void ChangeTotalRows(int totalRows) { this->totalRows = totalRows; }
	void ChangeDelay(float delay) { this->delay = delay; }
	void ChangeLoop(bool loop) { this->loop = loop; }
private:
	void CurrentFrame(int& index);
	int CurrentFrameIndex();
private:
	std::string textureID;
	int currentFrames = 0;
	int currentRow = 0;
	int totalFrames = 0;
	int totalRows = 0;
	float delay = 0.0f;
	bool loop = true;

	float timeElapsed = 0.0f;

	int texWidth = 0;
	int texHeight = 0;
	int defaultRow = 0;

	friend struct Animator;
	friend class AnimationController;
};

struct Animator : public Component
{
public:
	Animator();
	Animator(const std::initializer_list<std::pair<std::string, Ref<Animation>>>& animations);
	Animator(const Animator&);
	void Copy(const Animator& animator);

	void Update();

	void AddAnimation(std::string name, Ref<Animation> animation);
	void RemoveAnimation(const std::string& name);

	void AddEdge(const std::string& source, const std::string& destination, bool hasExitTime = true);
	void AddTwoSideEdge(const std::string& source, const std::string& destination, bool hasExitTimeSourceToDestination = true, bool hasExitTimeDestinationToSource = true);
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
	AnimationController controller;
};

struct Text : public Component
{
public:
	Text();
	Text(const std::string& label, const std::string& font, const glm::vec3& color);
	Text(const std::string& textID, const std::string& label, const std::string& font, const glm::vec3& color);
	Text(const Text& text);

	std::string GetLabel() const;
	std::string GetFont() const;
	std::string GetFontPath() const;
	int GetFontSize() const;
	glm::vec3 GetColor() const;
	
	void ChangeLabel(const std::string& label);
	void ChangeFont(const std::string& font);
	void ChangeColor(const glm::vec3& color);
private:
	void UpdateMesh();
private:
	std::string textID;
};

class ControlledEntity;
struct Behaviour
{
	Behaviour() = default;
	Behaviour(const Behaviour& behvaiour);

	ControlledEntity* Instance = nullptr;

	ControlledEntity* (*InstantiateScript)();
	void(*DestroyInstanceScript)(Behaviour*);

	template<typename T>
	void Bind()
	{
		InstantiateScript = []() { return static_cast<ControlledEntity*>(new T()); };
		DestroyInstanceScript = [](Behaviour* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
	}
};

struct Collider : public Component
{
public:
	Collider();
	Collider(const glm::vec2& offset, bool trigger = false);
	Collider(const Collider&);

	void SetOffset(const glm::vec2& offset) { this->offset = offset; }
	void SetSize(const glm::vec2& size);
	void SetTrigger(bool trigger) { this->trigger = trigger; }
	void SetCollision(bool collides) { this->collides = collides; }

	glm::vec2 GetOffset() const { return offset; }
	glm::vec2 GetSize() const { return size; }
	bool GetTrigger() const { return trigger; }
	bool Collides() const { return collides; }

	std::function<void(Entity)> onCollision = nullptr;
private:
	glm::vec2 offset;
	glm::vec2 size;
	bool trigger;
	bool collides;
};