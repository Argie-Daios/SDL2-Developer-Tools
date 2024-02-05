#pragma once

#include <SDL.h>
#include <string>
#include <glm/glm.hpp>
#include <functional>
#include <list>
#include <entt.hpp>

struct Children
{
	std::list<entt::entity> children;
};

struct Sprite
{
	SDL_Texture* texture = nullptr;
	glm::vec3 tintColor = glm::ivec3(255.0f, 255.0f, 255.0f);
	glm::vec2 texSize = glm::ivec2(0.0f, 0.0f);

	Sprite() = default;
	Sprite(const Sprite&) = default;
	~Sprite() { SDL_DestroyTexture(texture); }
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
	void SetZValue(float zValue) { this->zValue = zValue; } // Need to make for the children
	void SetRotation(float rotation) { this->rotation = rotation; } // Need to make for the children
	void SetScale(const glm::vec2& scale) { this->scale = scale; } // Need to make for the children

	glm::vec2 GetPosition() const { return position; }
	float GetZValue() const { return zValue; }
	float GetRotation() const { return rotation; }
	glm::vec2 GetScale() const { return scale; }
private:
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	float zValue = 0.0f;
	float rotation = 0.0f;
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);

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
	Animation(const std::string& image_path, int frames = 0, float delay = 0.0f);
	Animation(const Animation&) = default;

	void Animate();

	int GetFrames() const { return frames; }
	float GetDelay() const { return delay; }

	void ChangeFrames(int frames) { this->frames = frames; }
	void ChangeDelay(float delay) { this->delay = delay; }
private:
	void CurrentFrame(int& index, int& texWidth);
private:
	int frames = 0;
	float delay = 0.0f;
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