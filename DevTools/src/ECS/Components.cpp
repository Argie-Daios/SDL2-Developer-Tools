#include "Components.h"
#include "Renderer/Renderer.h"
#include "Entity.h"
#include "Tools/Positioner.h"
#include "Tools/Input.h"

#include <SDL_ttf.h>
#include <iostream>

namespace Utils {

	static SDL_Texture* LoadFont(const std::string& label, const std::string& font_path, int font_size, const SDL_Color& color)
	{
		TTF_Font* font = TTF_OpenFont(font_path.c_str(), font_size);

		if (!font)
		{
			std::cout << "FONT NULL" << std::endl;
		}

		auto text_surface = TTF_RenderText_Solid(font, label.c_str(), color);

		if (!text_surface)
		{
			std::cout << "SURFACE NULL" << std::endl;
		}

		auto text_texture = Renderer::CreateTextureFromSurface(text_surface);

		if (!text_texture)
		{
			std::cout << "TEXTURE NULL" << std::endl;
		}

		SDL_FreeSurface(text_surface);

		return text_texture;
	}
}

entt::entity Entity::recentEntity = entt::null;

// Transform

Transform::Transform()
	: Component(Entity::recentEntity), position(0.0f, 0.0f), zValue(0.0f), rotation(0.0f), scale(1.0f, 1.0f), size(0.0f, 0.0f),
	flip(SDL_FLIP_NONE)
{

}

Transform::Transform(const glm::vec2& position, float zValue, float rotation, const glm::vec2& scale)
	: Component(Entity::recentEntity), position(position), zValue(zValue), rotation(rotation), scale(scale), size(0.0f, 0.0f),
	flip(SDL_FLIP_NONE)
{

}

void Transform::SetPosition(const glm::vec2& position)
{
	Entity en = { m_Entity };
	glm::vec2 prevPos = position;
	this->position = position;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child };

		glm::vec2 newPos = ent.GetComponent<Transform>().position - prevPos;

		ent.GetComponent<Transform>().SetPosition(position + newPos);
	}
}

void Transform::Translate(const glm::vec2& translation)
{
	Entity en = { m_Entity };
	position += translation;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child };
		ent.GetComponent<Transform>().Translate(translation);
	}
}

void Transform::SetZValue(float zValue)
{
	Entity en = { m_Entity };
	this->zValue = zValue;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child };
		auto& transformComponent = ent.transform();
		transformComponent.SetZValue(transformComponent.GetZValue() + this->zValue);
	}

	REGISTRY.sort<Transform>([](const Transform& left, const Transform& right) {return left.GetZValue() < right.GetZValue(); });
}

void Transform::SetRotation(float rotation) 
{ 
	Entity en = { m_Entity };
	this->rotation = rotation;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child };
		auto& transformComponent = ent.transform();
		transformComponent.SetRotation(transformComponent.GetRotation() + this->rotation);
	}
}

void Transform::SetScale(const glm::vec2& scale)
{ 
	Entity en = { m_Entity };
	this->scale = scale;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child };
		auto& transformComponent = ent.transform();
		transformComponent.SetScale(transformComponent.GetScale() + this->scale);
	}
} 

void Transform::SetFlip(SDL_RendererFlip flip) 
{ 
	Entity en = { m_Entity };
	this->flip = flip;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child };
		auto& transformComponent = ent.transform();
		transformComponent.SetFlip(this->flip);
	}
}

// Sprite Renderer

SpriteRenderer::SpriteRenderer()
	: Component(Entity::recentEntity), texture_path("-"), texture(nullptr), tintColor(255.0f, 255.0f, 255.0f), source{0,0,0,0}
{

}

SpriteRenderer::SpriteRenderer(const std::string& image_path)
	: Component(Entity::recentEntity), texture_path(image_path)
{
	ChangeTexture(image_path);
}

void SpriteRenderer::ChangeTexture(std::string image_path)
{
	SDL_Texture* temp = texture;
	texture = Renderer::CreateTexture(image_path);
	SDL_DestroyTexture(temp);
	UpdateSprite();
	texture_path = image_path;
}

void SpriteRenderer::ChangeTexture(SDL_Texture* texture)
{
	this->texture = texture;
	UpdateSprite();
}

void SpriteRenderer::UpdateSprite()
{
	if (!texture) return;

	int texWidth, texHeight;
	Entity en = { m_Entity };

	SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);

	source = SDL_Rect{ 0, 0, texWidth, texHeight };
	en.transform().SetSize(glm::vec2(texWidth, texHeight));
}

// Animation

Animation::Animation()
	: Component(Entity::recentEntity), texture_path("-"), currentFrames(0), currentRow(0), totalFrames(0), totalRows(0), delay(0), loop(0)
{

}

Animation::Animation(const std::string& image_path, int currentFrames, int currentRow, int totalFrames, int totalRows, float delay, bool loop)
	: Component(Entity::recentEntity), texture_path(image_path), currentFrames(currentFrames), currentRow(currentRow), totalFrames(totalFrames),
	totalRows(totalRows), delay(delay), loop(loop), timeElapsed(0.0f)
{
	Entity e = { m_Entity };
	auto& transformComponent = e.transform();
	auto& spriteRenderer = e.AddIfNotExistsOrGet<SpriteRenderer>();

	spriteRenderer.ChangeTexture(image_path);
	texWidth = spriteRenderer.GetSource().w;
	texHeight = spriteRenderer.GetSource().h;
	transformComponent.SetSize(glm::vec2(texWidth / totalFrames, texHeight / totalRows));
}

Animation::Animation(const Animation& animation)
{
	m_Entity = animation.m_Entity;
	texture_path = animation.texture_path;
	currentFrames = animation.currentFrames;
	currentRow = animation.currentRow;
	totalFrames = animation.totalFrames;
	totalRows = animation.totalRows;
	delay = animation.delay;
	loop = animation.loop;
	timeElapsed = 0.0f;
	texWidth = animation.texWidth;
	texHeight = animation.texHeight;
}

void Animation::Animate()
{
	if (texture_path.find("Attack") != std::string::npos)
		std::cout << "Time Elapsed : " << timeElapsed << " Current Index : " << CurrentFrameIndex() << std::endl;
	if (!loop && isComplete()) return;

	Entity e = { m_Entity };
	auto& transformComponent = e.transform();
	auto& spriteRenderer = e.GetComponent<SpriteRenderer>();

	int frameIndex;

	if (texture_path != spriteRenderer.GetTexturePath())
	{
		spriteRenderer.ChangeTexture(texture_path);
		texWidth = spriteRenderer.GetSource().w;
		texHeight = spriteRenderer.GetSource().h;
		transformComponent.SetSize(glm::vec2(texWidth / totalFrames, texHeight / totalRows));
	}

	CurrentFrame(frameIndex);

	timeElapsed += Time::SecondsToMilliseconds(Time::DeltaTime());

	spriteRenderer.SetSource(SDL_Rect{ frameIndex, currentRow * (texHeight / totalRows), texWidth / totalFrames, texHeight / totalRows});
}

bool Animation::isComplete()
{
	if(currentFrames <= totalFrames)
		return CurrentFrameIndex() + 1 == currentFrames;

	return CurrentFrameIndex() + (totalFrames * currentRow) == currentFrames - 1;
}

void Animation::CurrentFrame(int& index)
{
	int frameSize;

	if (totalFrames == 0)
	{
		index = 0;
		return;
	}

	frameSize = texWidth / totalFrames;

	index = CurrentFrameIndex() * frameSize;
}

int Animation::CurrentFrameIndex()
{	
	int frames = ((int)timeElapsed / (int)delay);
	int index = (frames % currentFrames);

	if (index >= totalFrames)
	{
		currentRow++;
		index = 0;
		timeElapsed = 0;
		if (currentRow == totalRows)
		{
			currentRow = 0;
		}
	}

	return index;
}

// Animator

Animator::Animator()
	: Component(Entity::recentEntity)
{
	
}

Animator::Animator(const std::initializer_list<AnimationNode>& animations)
	: Component(Entity::recentEntity)
{
	for (auto element : animations)
	{
		element.second->m_Entity = m_Entity;
		controller.AddAnimation(element.first, element.second);
	}
}

void Animator::Update()
{
	controller.Update();
	controller.GetCurrentAnimation()->Animate();
}

void Animator::AddAnimation(std::string name, Ref<Animation> animation)
{
	controller.AddAnimation(name, animation);
}

void Animator::RemoveAnimation(const std::string& name)
{
	controller.RemoveAnimation(name);
}

void Animator::AddEdge(const std::string& source, const std::string& destination, bool hasExitTime)
{
	controller.AddEdge(source, destination, hasExitTime);
}

void Animator::AddTwoSideEdge(const std::string& source, const std::string& destination, bool hasExitTimeSourceToDestination, bool hasExitTimeDestinationToSource)
{
	AddEdge(source, destination, hasExitTimeSourceToDestination);
	AddEdge(destination, source, hasExitTimeDestinationToSource);
}

void Animator::RemoveEdge(const std::string& source, const std::string& destination)
{
	controller.RemoveEdge(source, destination);
}

void Animator::AddParameter(const std::string& name, Type type, void* value)
{
	controller.AddParameter(name, type, value);
}

void Animator::AddIntParameter(const std::string& name, int value)
{
	controller.AddIntParameter(name, value);
}

void Animator::AddFloatParameter(const std::string& name, float value)
{
	controller.AddFloatParameter(name, value);
}

void Animator::AddBoolParameter(const std::string& name, bool value)
{
	controller.AddBoolParameter(name, value);
}

void Animator::RemoveParameter(const std::string& name)
{
	controller.RemoveParameter(name);
}

void Animator::ChangeParameterValue(const std::string& name, void* value)
{
	controller.ChangeParameterValue(name, value);
}

void Animator::ChangeIntParameterValue(const std::string& name, int value)
{
	controller.ChangeIntParameterValue(name, value);
}

void Animator::ChangeFloatParameterValue(const std::string& name, float value)
{
	controller.ChangeFloatParameterValue(name, value);
}

void Animator::ChangeBoolParameterValue(const std::string& name, bool value)
{
	controller.ChangeBoolParameterValue(name, value);
}

void Animator::AddConditionOnEdge(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, void* valueToCompare, Type valueToCompareType)
{
	controller.AddConditionOnEdge(source, destination, parameter, operation, valueToCompare, valueToCompareType);
}

void Animator::AddConditionOnEdgeInt(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, int valueToCompare)
{
	controller.AddConditionOnEdgeInt(source, destination, parameter, operation, valueToCompare);
}

void Animator::AddConditionOnEdgeFloat(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, float valueToCompare)
{
	controller.AddConditionOnEdgeFloat(source, destination, parameter, operation, valueToCompare);
}

void Animator::AddConditionOnEdgeBool(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, bool valueToCompare)
{
	controller.AddConditionOnEdgeBool(source, destination, parameter, operation, valueToCompare);
}

void Animator::RemoveConditionOffEdge(const std::string& source, const std::string& destination, const std::string& parameter)
{
	controller.RemoveConditionOffEdge(source, destination, parameter);
}

// Text

Text::Text()
	: Component(Entity::recentEntity), label("Text"), font_path("assets/fonts/arial.ttf"), font_size(10),
	color(SDL_Color{ 255,255,255 })
{
	UpdateMesh();
}

Text::Text(const std::string& label, const std::string& font_path, int font_size, const SDL_Color& color)
	: Component(Entity::recentEntity), label(label), font_path(font_path), font_size(font_size), color(color)
{
	UpdateMesh();
}

Text::Text(const Text& text)
{
	m_Entity = text.m_Entity;
	label = text.label;
	font_path = text.font_path;
	font_size = text.font_size;
	color = text.color;

	UpdateMesh();
}

void Text::ChangeLabel(const std::string& label)
{
	this->label = label;
	UpdateMesh();
}

void Text::ChangeFontPath(const std::string& font_path)
{
	this->font_path = font_path;
	UpdateMesh();
}

void Text::ChangeFontSize(int font_size)
{
	this->font_size = font_size;
	UpdateMesh();
}

void Text::ChangeColor(const SDL_Color& color)
{
	this->color = color;
	UpdateMesh();
}

void Text::UpdateMesh()
{
	Entity e = { m_Entity };
	auto& transformComponent = e.transform();
	auto& spriteComponent = e.AddIfNotExistsOrGet<SpriteRenderer>();

	int texWidth, texHeight;
	SDL_Texture* texture = Utils::LoadFont(label, font_path, font_size, color);

	SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);

	spriteComponent.ChangeTexture(texture);
	spriteComponent.SetSource(SDL_Rect{ 0, 0, texWidth, texHeight });
	transformComponent.SetSize(glm::vec2(texWidth, texHeight));
}

// Collider

Collider::Collider()
	: Component(Entity::recentEntity), offset(glm::vec2(0.0f, 0.0f)), size(glm::vec2(0.0f, 0.0f)), trigger(false), collides(false)
{
	Entity en = { m_Entity };
	auto& transformComponent = en.transform();

	size.x = transformComponent.GetSize().x * transformComponent.GetScale().x;
	size.y = transformComponent.GetSize().y * transformComponent.GetScale().y;
}

Collider::Collider(const glm::vec2& offset, bool trigger)
	: Component(Entity::recentEntity), offset(offset), size(glm::vec2(0.0f, 0.0f)), trigger(trigger), collides(false)
{
	Entity en = { m_Entity };
	auto& transformComponent = en.transform();

	size.x = transformComponent.GetSize().x * transformComponent.GetScale().x;
	size.y = transformComponent.GetSize().y * transformComponent.GetScale().y;
}

void Collider::SetSize(const glm::vec2& size)
{ 
	Entity en = { m_Entity };
	auto& transformComponent = en.transform();

	this->size.x = size.x * transformComponent.GetScale().x;
	this->size.y = size.y * transformComponent.GetScale().y;
}