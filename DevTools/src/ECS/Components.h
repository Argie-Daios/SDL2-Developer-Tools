#pragma once

#include <SDL.h>
#include <string>
#include <glm/glm.hpp>
#include <functional>
#include <list>
#include <entt.hpp>

#include "Tools/AnimationController/AnimationController.h"

struct Children
{
	std::list<entt::entity> children;
};

struct Component
{
public:
	Component(entt::entity entity = entt::null) { m_Entity = entity; }
protected:
	entt::entity m_Entity;
};

struct Transform : public Component
{
public:
	Transform();
	Transform(const glm::vec2& position, float zValue = 0.0f, float rotation = 0.0f, const glm::vec2& scale = glm::vec2(1.0f, 1.0f));
	Transform(const Transform&) = default;

	void Translate(const glm::vec2& translation);

	void SetPosition(const glm::vec2& position);
	void SetZValue(float zValue); // Need to make for the children
	void SetRotation(float rotation) { this->rotation = rotation; } // Need to make for the children
	void SetScale(const glm::vec2& scale) { this->scale = scale; } // Need to make for the children
	void SetFlip(SDL_RendererFlip flip) { this->flip = flip; }

	glm::vec2 GetPosition() const { return position; }
	float GetZValue() const { return zValue; }
	float GetRotation() const { return rotation; }
	glm::vec2 GetScale() const { return scale; }
	SDL_RendererFlip GetFlip() const { return flip; }
private:
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	float zValue = 0.0f;
	float rotation = 0.0f;
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	friend class Entity;
};

struct Mesh : public Component
{
public:
	Mesh();
	Mesh(const Mesh&) = default;
	~Mesh() { SDL_DestroyTexture(texture); }

	void SetTexture(std::string image_path);
	void SetTexture(SDL_Texture* texture) { this->texture = texture; }
	void SetTintColor(glm::vec3 color) { tintColor = color; }
	void SetSource(SDL_Rect src) { source = src; }
	void SetSize(glm::vec2 size) { this->size = size; }

	SDL_Texture* GetTexture() const { return texture; }
	glm::vec3 GetColor() const { return tintColor; }
	SDL_Color GetSDLColor() const { return SDL_Color{ (unsigned char)(int)tintColor.r, (unsigned char)(int)tintColor.g, (unsigned char)(int)tintColor.b }; }
	SDL_Rect GetSource() const { return source; }
	glm::vec2 GetSize() const { return size; }
private:
	SDL_Texture* texture = nullptr;
	glm::vec3 tintColor = glm::ivec3(255.0f, 255.0f, 255.0f);
	SDL_Rect source;
	glm::vec2 size;
};

struct SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	SpriteRenderer(const std::string& image_path);
	SpriteRenderer(const SpriteRenderer&) = default;

	void ChangeTexture(std::string image_path);
private:
	void UpdateMesh();
};

struct Animation : public Component
{
public:
	Animation();
	Animation(const std::string& image_path, int currentFrames = 0, int currentRow = 0, int totalFrames = 0, int totalRows = 0, float delay = 100.0f, bool loop = true);
	Animation(const Animation&);

	void Animate();

	bool isComplete() { return CurrentFrameIndex() + 1 == currentFrames; }

	int GetCurrentFrames() const { return currentFrames; }
	int GetCurrentRow() const { return currentRow; }
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
	void CurrentFrame(int& index, int& texWidth);
	int CurrentFrameIndex();
private:
	int currentFrames = 0;
	int currentRow = 0;
	int totalFrames = 0;
	int totalRows = 0;
	float delay = 0.0f;
	bool loop = true;

	float timeElapsed = 0.0f;

	friend struct Animator;
	friend class AnimationController;
};

struct Animator : public Component
{
public:
	Animator();
	Animator(const std::initializer_list<std::pair<std::string, Ref<Animation>>>& animations);
	Animator(const Animator&) = default;

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
	Text(const std::string& label, const std::string& font_path, int font_size, const SDL_Color& color);
	Text(const Text& text);

	std::string GetLabel() const { return label; }
	std::string GetFontPath() const { return font_path; }
	int GetFontSize() const { return font_size; }
	SDL_Color GetColor() const { return color; }
	
	void ChangeLabel(const std::string& label);
	void ChangeFontPath(const std::string& font_path);
	void ChangeFontSize(int font_size);
	void ChangeColor(const SDL_Color& color);
private:
	void UpdateMesh();
private:
	std::string label;
	std::string font_path;
	int font_size;
	SDL_Color color;
};

class ControlledEntity;
struct Behaviour
{
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