#include "Components.h"
#include "Renderer/Renderer.h"
#include "Entity.h"
#include "Tools/Positioner.h"
#include "Tools/Input.h"

#include <SDL_ttf.h>
#include <iostream>

EntityStructure Entity::recentEntity;

// Transform

Transform::Transform()
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), position(0.0f, 0.0f), zValue(0.0f),
	rotation(0.0f), scale(1.0f, 1.0f), size(0.0f, 0.0f), flip(SDL_FLIP_NONE)
{

}

Transform::Transform(const glm::vec2& position, float zValue, float rotation, const glm::vec2& scale)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), position(position), zValue(zValue),
	rotation(rotation), scale(scale), size(0.0f, 0.0f), flip(SDL_FLIP_NONE)
{

}

void Transform::SetPosition(const glm::vec2& position)
{
	Entity en = { m_Entity, m_Scene };
	glm::vec2 prevPos = position;
	this->position = position;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child, m_Scene };

		glm::vec2 newPos = ent.GetComponent<Transform>().position - prevPos;

		ent.GetComponent<Transform>().SetPosition(position + newPos);
	}
}

void Transform::Translate(const glm::vec2& translation)
{
	Entity en = { m_Entity, m_Scene };
	position += translation;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child, m_Scene };
		ent.GetComponent<Transform>().Translate(translation);
	}
}

void Transform::SetZValue(float zValue)
{
	Entity en = { m_Entity, m_Scene };
	this->zValue = zValue;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child, m_Scene };
		auto& transformComponent = ent.transform();
		transformComponent.SetZValue(transformComponent.GetZValue() + this->zValue);
	}

	REGISTRY.sort<Transform>([](const Transform& left, const Transform& right) {return left.GetZValue() < right.GetZValue(); });
}

void Transform::SetRotation(float rotation) 
{ 
	Entity en = { m_Entity, m_Scene };
	this->rotation = rotation;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child, m_Scene };
		auto& transformComponent = ent.transform();
		transformComponent.SetRotation(transformComponent.GetRotation() + this->rotation);
	}
}

void Transform::SetScale(const glm::vec2& scale)
{ 
	Entity en = { m_Entity, m_Scene };
	this->scale = scale;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child, m_Scene };
		auto& transformComponent = ent.transform();
		transformComponent.SetScale(transformComponent.GetScale() + this->scale);
	}
} 

void Transform::SetFlip(SDL_RendererFlip flip) 
{ 
	Entity en = { m_Entity, m_Scene };
	this->flip = flip;

	for (auto& child : en.GetChildren())
	{
		Entity ent = { child, m_Scene };
		auto& transformComponent = ent.transform();
		transformComponent.SetFlip(this->flip);
	}
}

// Sprite Renderer

SpriteRenderer::SpriteRenderer()
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), textureID("-"), tintColor(255.0f, 255.0f, 255.0f), source{0,0,0,0}
{

}

SpriteRenderer::SpriteRenderer(const std::string& textureID)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), textureID(textureID)
{
	
}

void SpriteRenderer::ChangeTextureID(std::string textureID)
{
	Entity en = { m_Entity, m_Scene };
	this->textureID = textureID;
	UpdateSprite();
}

void SpriteRenderer::UpdateSprite()
{
	Entity en = { m_Entity, m_Scene };
	Texture texture = AssetManager::GetTexture(textureID);
	int texWidth = texture.width;
	int texHeight = texture.height;

	source = SDL_Rect{ 0, 0, texWidth, texHeight };
	en.transform().SetSize(glm::vec2(texWidth, texHeight));
}

// Animation

Animation::Animation()
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), textureID("-"), currentFrames(0), currentRow(0), totalFrames(0), totalRows(0), delay(0), loop(0), defaultRow(0)
{

}

Animation::Animation(const std::string& textureID, int currentFrames, int currentRow, int totalFrames, int totalRows, float delay, bool loop)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), textureID(textureID), currentFrames(currentFrames), currentRow(currentRow), totalFrames(totalFrames),
	totalRows(totalRows), delay(delay), loop(loop), defaultRow(currentRow), timeElapsed(0.0f)
{
	Entity e = { m_Entity, m_Scene };
	auto& transformComponent = e.transform();
	auto& spriteRenderer = e.AddIfNotExistsOrGet<SpriteRenderer>();

	spriteRenderer.ChangeTextureID(textureID);
	texWidth = spriteRenderer.GetSource().w;
	texHeight = spriteRenderer.GetSource().h;
	transformComponent.SetSize(glm::vec2(texWidth / totalFrames, texHeight / totalRows));
}

Animation::Animation(const Animation& animation)
	: Component(animation.m_Entity, animation.m_Scene)
{
	textureID = animation.textureID;
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
	if (!loop && isComplete()) return;

	Entity e = { m_Entity, m_Scene };
	auto& transformComponent = e.transform();
	auto& spriteRenderer = e.GetComponent<SpriteRenderer>();

	int frameIndex;

	if (textureID != spriteRenderer.GetTextureID())
	{
		spriteRenderer.ChangeTextureID(textureID);
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
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene)
{
	
}

Animator::Animator(const std::initializer_list<AnimationNode>& animations)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene)
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
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), textID("-")
{
	
}

Text::Text(const std::string& label, const std::string& font, const glm::vec3& color)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene)
{
	std::string textID = randomStringGenerator(15);
	AssetManager::LoadText(textID, label, font, color);
	UpdateMesh();
}

Text::Text(const std::string& textID, const std::string& label, const std::string& font, const glm::vec3& color)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), textID(textID)
{
	AssetManager::LoadText(textID, label, font, color);
	UpdateMesh();
}

std::string Text::GetLabel() const
{
	return AssetManager::GetText(textID).label;
}

std::string Text::GetFont() const
{
	return AssetManager::GetText(textID).font;
}

std::string Text::GetFontPath() const
{
	TextProperties textProperties = AssetManager::GetText(textID);
	return AssetManager::GetFont(textProperties.font).fontpath;
}

int Text::GetFontSize() const
{
	TextProperties textProperties = AssetManager::GetText(textID);
	return AssetManager::GetFont(textProperties.font).font_size;
}

glm::vec3 Text::GetColor() const
{
	return AssetManager::GetText(textID).color;
}

void Text::ChangeLabel(const std::string& label)
{
	if(AssetManager::ChangeTextLabel(textID, label))
		UpdateMesh();
}

void Text::ChangeFont(const std::string& font)
{
	if(AssetManager::ChangeTextFont(textID, font))
		UpdateMesh();
}

void Text::ChangeColor(const glm::vec3& color)
{
	if(AssetManager::ChangeTextColor(textID, color))
		UpdateMesh();
}

void Text::UpdateMesh()
{
	Entity e = { m_Entity, m_Scene };
	Texture texture = AssetManager::GetTexture(textID);
	auto& spriteRenderer = e.AddIfNotExistsOrGet<SpriteRenderer>();
	spriteRenderer.ChangeTextureID(textID);
}

// Collider

Collider::Collider()
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), offset(glm::vec2(0.0f, 0.0f)), size(glm::vec2(0.0f, 0.0f)), trigger(false), collides(false)
{
	Entity en = { m_Entity, m_Scene };
	auto& transformComponent = en.transform();

	size.x = transformComponent.GetSize().x * transformComponent.GetScale().x;
	size.y = transformComponent.GetSize().y * transformComponent.GetScale().y;
}

Collider::Collider(const glm::vec2& offset, bool trigger)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), offset(offset), size(glm::vec2(0.0f, 0.0f)), trigger(trigger), collides(false)
{
	Entity en = { m_Entity, m_Scene };
	auto& transformComponent = en.transform();

	size.x = transformComponent.GetSize().x * transformComponent.GetScale().x;
	size.y = transformComponent.GetSize().y * transformComponent.GetScale().y;
}

void Collider::SetSize(const glm::vec2& size)
{ 
	Entity en = { m_Entity, m_Scene };
	auto& transformComponent = en.transform();

	this->size.x = size.x * transformComponent.GetScale().x;
	this->size.y = size.y * transformComponent.GetScale().y;
}