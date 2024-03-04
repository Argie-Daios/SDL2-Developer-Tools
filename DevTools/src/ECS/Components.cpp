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
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), position(position), rotation(rotation),
	scale(scale), size(0.0f, 0.0f), flip(SDL_FLIP_NONE)
{
	SetZValue(zValue);
}

Transform::Transform(const Transform& transform)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene)
{
	position = transform.position;
	size = transform.size;
	zValue = transform.zValue;
	rotation = transform.rotation;
	scale = transform.scale;
	flip = transform.flip;
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

	//m_Scene->m_Registry.sort<Transform>([](const Transform& left, const Transform& right) {return left.GetZValue() < right.GetZValue(); });
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

SpriteRenderer::SpriteRenderer(const SpriteRenderer& spriteRenderer)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), textureID(textureID)
{
	textureID = spriteRenderer.textureID;
	tintColor = spriteRenderer.tintColor;
	source = spriteRenderer.source;
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
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), animationNode(AnimationNode{"-"})
{

}

Animation::Animation(const std::string& animationID, float delay, bool loop)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), animationNode(AnimationNode{animationID, 0, delay, loop})
{
	UpdateMesh();
}

void Animation::Animate()
{
	if (!animationNode.loop && isComplete()) return;

	Entity e = { m_Entity, m_Scene };
	auto& transformComponent = e.transform();
	auto& spriteRenderer = e.GetComponent<SpriteRenderer>();

	if (GetTextureID() != spriteRenderer.GetTextureID())
	{
		UpdateMesh();
	}

	glm::ivec2 indexes = CalculateCurrentFrame();
	glm::vec2 size = transformComponent.GetSize();

	animationNode.timeElapsed += Time::SecondsToMilliseconds(Time::DeltaTime());

	//std::cout << "Current Frame : " << currentFrame << " Col Index : " << indexes.x << " Row Index : " << indexes.y << std::endl;

	spriteRenderer.SetSource(SDL_Rect{indexes.x * (int)size.x, indexes.y * (int)size.y, (int)size.x, (int)size.y});
}

bool Animation::isComplete()
{

	return animationNode.isComplete();
}

glm::ivec2 Animation::CalculateCurrentFrame()
{	
	AnimationProperties animationProps = AssetManager::GetAnimation(animationNode.animationID);
	int index = (int)(animationNode.timeElapsed / animationNode.delay);
	int colIndex, rowIndex;

	animationNode.currentFrame = index % animationProps.totalFrames;

	if (isComplete())
	{
		animationNode.currentFrame = 0;
		animationNode.timeElapsed = 0.0f;
		index = (int)(animationNode.timeElapsed / animationNode.delay);
	}

	colIndex = index % animationProps.totalFramesPerRow;
	rowIndex = animationNode.currentFrame / animationProps.totalFramesPerRow;

	// std::cout << "Row Index : " << rowIndex << " currentFrame : " << currentFrame << " totalFrames : " << animationProps.totalFrames << std::endl;

	return glm::ivec2(colIndex, rowIndex);
}

void Animation::UpdateMesh()
{
	Entity e = { m_Entity, m_Scene };
	AnimationProperties& animationProps = AssetManager::GetAnimation(animationNode.animationID);
	Texture& texture = AssetManager::GetTexture(animationProps.textureID);
	auto& transformComponent = e.transform();
	auto& spriteRenderer = e.AddIfNotExistsOrGet<SpriteRenderer>();
	spriteRenderer.ChangeTextureID(animationProps.textureID);
	transformComponent.SetSize(glm::vec2(texture.width / animationProps.totalFramesPerRow, texture.height / animationProps.totalRows));
}

// Animator

Animator::Animator()
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene)
{
	
}

Animator::Animator(const std::string& name)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene), m_AnimationControllerID(name)
{
	AnimationController& controller = AssetManager::GetAnimationController(name);
	m_Current = controller.m_Entry;
	m_CurrentAnimation.m_Entity = m_Entity;
	m_CurrentAnimation.m_Scene = m_Scene;
	m_CurrentAnimation.animationNode.animationID = controller.GetCurrentAnimation(m_Current).animationID;
	m_CurrentAnimation.animationNode.currentFrame = controller.GetCurrentAnimation(m_Current).currentFrame;
	m_CurrentAnimation.animationNode.delay = controller.GetCurrentAnimation(m_Current).delay;
	m_CurrentAnimation.animationNode.loop = controller.GetCurrentAnimation(m_Current).loop;
	m_CurrentAnimation.animationNode.timeElapsed = controller.GetCurrentAnimation(m_Current).timeElapsed;
	m_CurrentAnimation.UpdateMesh();

	for (auto& param : controller.m_Parameters)
	{
		m_Parameters.emplace(param.first, CreateRef<Parameter>(param.second->GetType(), CopyPtr(param.second->GetValue(), param.second->GetType())));
	}
}

void Animator::Update()
{
	AnimationController& controller = AssetManager::GetAnimationController(m_AnimationControllerID);
	controller.Update(m_Current, m_Parameters);
	if (controller.GetCurrentAnimation(m_Current).animationID != m_CurrentAnimation.animationNode.animationID)
	{
		m_CurrentAnimation.m_Entity = m_Entity;
		m_CurrentAnimation.m_Scene = m_Scene;
		m_CurrentAnimation.animationNode.animationID = controller.GetCurrentAnimation(m_Current).animationID;
		m_CurrentAnimation.animationNode.currentFrame = controller.GetCurrentAnimation(m_Current).currentFrame;
		m_CurrentAnimation.animationNode.delay = controller.GetCurrentAnimation(m_Current).delay;
		m_CurrentAnimation.animationNode.loop = controller.GetCurrentAnimation(m_Current).loop;
		m_CurrentAnimation.animationNode.timeElapsed = controller.GetCurrentAnimation(m_Current).timeElapsed;
		m_CurrentAnimation.UpdateMesh();
	}
	m_CurrentAnimation.Animate();
}

void Animator::ChangeParameterValue(const std::string& name, void* value)
{
	GAME_ASSERT(m_Parameters.find(name) != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	m_Parameters[name]->SetValue(value);
}

void Animator::ChangeIntParameterValue(const std::string& name, int value)
{
	auto it = m_Parameters.find(name);
	GAME_ASSERT(it != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	Type type = it->second->GetType();
	GAME_ASSERT(type == Type::INT, "Parameter (" + name + ") is of type " + TypeToString(type) + " but provided int");

	m_Parameters[name]->SetValue(ValueToVoidPtr<int>(value));
}

void Animator::ChangeFloatParameterValue(const std::string& name, float value)
{
	auto it = m_Parameters.find(name);
	GAME_ASSERT(it != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	Type type = it->second->GetType();
	GAME_ASSERT(type == Type::FLOAT, "Parameter (" + name + ") is of type " + TypeToString(type) + " but provided float");

	m_Parameters[name]->SetValue(ValueToVoidPtr<float>(value));
}

void Animator::ChangeBoolParameterValue(const std::string& name, bool value)
{
	auto it = m_Parameters.find(name);
	GAME_ASSERT(it != m_Parameters.end(), "Parameter (" + name + ") does not exist");

	Type type = it->second->GetType();
	GAME_ASSERT(type == Type::BOOL, "Parameter (" + name + ") is of type " + TypeToString(type) + " but provided bool");

	m_Parameters[name]->SetValue(ValueToVoidPtr<bool>(value));
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

Text::Text(const Text& text)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene)
{
	textID = text.textID;
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
	auto& transformComponent = e.transform();
	auto& spriteRenderer = e.AddIfNotExistsOrGet<SpriteRenderer>();
	spriteRenderer.ChangeTextureID(textID);
	transformComponent.SetSize(glm::vec2(texture.width, texture.height));
}

// Behaviour
Behaviour::Behaviour(const Behaviour& behaviour)
{

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

Collider::Collider(const Collider& collider)
	: Component(Entity::recentEntity.handle, Entity::recentEntity.scene)
{
	offset = collider.offset;
	size = collider.size;
	trigger = collider.trigger;
	collides = collider.collides;
}

void Collider::SetSize(const glm::vec2& size)
{ 
	Entity en = { m_Entity, m_Scene };
	auto& transformComponent = en.transform();

	this->size.x = size.x * transformComponent.GetScale().x;
	this->size.y = size.y * transformComponent.GetScale().y;
}