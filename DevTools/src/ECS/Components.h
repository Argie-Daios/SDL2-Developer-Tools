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
	Animation(const std::string& animationID, float delay = 100.0f, bool loop = true);
	Animation(const Animation&) = default;

	void Animate();

	bool isComplete();

	std::string GetAnimationID() const { return animationNode.animationID; }
	std::string GetTextureID() const { return AssetManager::GetAnimation(animationNode.animationID).textureID; }
 	int GetTotalFrames() const { return AssetManager::GetAnimation(animationNode.animationID).totalFrames; }
	int GetTotalFramesPerRow() const { return AssetManager::GetAnimation(animationNode.animationID).totalFramesPerRow; }
	int GetTotalRows() const { return AssetManager::GetAnimation(animationNode.animationID).totalRows; }
	int GetFristFrame() const { return AssetManager::GetAnimation(animationNode.animationID).firstFrame; }
	int GetLastFrame() const { return AssetManager::GetAnimation(animationNode.animationID).lastFrame; }
	float GetDelay() const { return animationNode.delay; }
	bool GetLoop() const { return animationNode.loop; }

	void ChangeDelay(float delay) { animationNode.delay = delay; }
	void ChangeLoop(bool loop) { animationNode.loop = loop; }
private:
	glm::ivec2 CalculateCurrentFrame();
	void UpdateMesh();
private:
	AnimationNode animationNode;

	friend struct Animator;
	friend class AnimationController;
};

struct Animator : public Component
{
public:
	Animator();
	Animator(const std::string& name);
	Animator(const Animator&) = default;

	void Update();

	void ChangeParameterValue(const std::string& name, void* value);
	void ChangeIntParameterValue(const std::string& name, int value);
	void ChangeFloatParameterValue(const std::string& name, float value);
	void ChangeBoolParameterValue(const std::string& name, bool value);
private:
	std::string m_AnimationControllerID;
	Animation m_CurrentAnimation;
	std::unordered_map<std::string, Ref<Parameter>> m_Parameters;
	std::string m_Current;
};

//struct Animator : public Component
//{
//public:
//
//private:
//	std::string animationController;
//};

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