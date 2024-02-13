#include "Components.h"
#include "Renderer/Renderer.h"
#include "Entity.h"
#include "Tools/Positioner.h"

#include <SDL_ttf.h>
#include <iostream>

entt::entity Entity::recentEntity = entt::null;

// Transform

Transform::Transform()
	: Component(Entity::recentEntity)
{

}

Transform::Transform(const glm::vec2& position, float zValue, float rotation, const glm::vec2& scale)
	: Component(Entity::recentEntity)
{

}

void Transform::Translate(const glm::vec2& translation)
{
	Entity en = { m_Entity };
	en.GetComponent<Transform>().position += translation;

	for (auto& child : en.GetComponent<Children>().children)
	{
		Entity ent = { child };
		ent.GetComponent<Transform>().Translate(translation);
	}
}

void Transform::SetPosition(const glm::vec2& position)
{
	Entity en = { m_Entity };
	glm::vec2 prevPos = en.GetComponent<Transform>().position;
	en.GetComponent<Transform>().position = position;

	for (auto& child : en.GetComponent<Children>().children)
	{
		Entity ent = { child };

		glm::vec2 newPos = ent.GetComponent<Transform>().position - prevPos;

		ent.GetComponent<Transform>().SetPosition(position + newPos);
	}
}

// Mesh

Mesh::Mesh()
	: Component(Entity::recentEntity)
{

}

void Mesh::SetTexture(std::string image_path)
{ 
	texture = Renderer::CreateTexture(image_path);
}

// Sprite Renderer

SpriteRenderer::SpriteRenderer()
	: Component(Entity::recentEntity)
{

}

SpriteRenderer::SpriteRenderer(const std::string& image_path)
	: Component(Entity::recentEntity)
{
	Entity e = { m_Entity };

	auto& mesh = e.GetComponent<Mesh>();

	mesh.SetTexture(image_path);

	UpdateMesh();
}

void SpriteRenderer::ChangeTexture(std::string image_path)
{
	Entity e = { m_Entity };

	auto& mesh = e.GetComponent<Mesh>();

	mesh.SetTexture(image_path);

	UpdateMesh();
}

void SpriteRenderer::UpdateMesh()
{
	Entity e = { m_Entity };

	auto& mesh = e.GetComponent<Mesh>();

	int texWidth, texHeight;

	SDL_QueryTexture(mesh.GetTexture(), nullptr, nullptr, &texWidth, &texHeight);

	mesh.SetSource(SDL_Rect{ 0, 0, texWidth, texHeight });
	mesh.SetSize(glm::vec2(texWidth, texHeight));
}

// Animation

Animation::Animation()
	: Component(Entity::recentEntity)
{

}

Animation::Animation(const std::string& image_path, int currentFrames, int currentRow, int totalFrames, int totalRows, float delay, bool loop)
	: Component(Entity::recentEntity), currentFrames(currentFrames), currentRow(currentRow), totalFrames(totalFrames), totalRows(totalRows), delay(delay), loop(loop)
{
	Entity e = { m_Entity };

	auto& mesh = e.GetComponent<Mesh>();

	mesh.SetTexture(image_path);

	Animate();
}

Animation::Animation(const Animation& animation)
{
	m_Entity = animation.m_Entity;
	currentFrames = animation.currentFrames;
	currentRow = animation.currentRow;
	totalFrames = animation.totalFrames;
	totalRows = animation.totalRows;
	delay = animation.delay;
	loop = animation.loop;
	timeElapsed = 0.0f;
}

void Animation::Animate()
{
	Entity e = { m_Entity };

	auto& mesh = e.GetComponent<Mesh>();

	int texWidth, texHeight, frameIndex;

	SDL_QueryTexture(mesh.GetTexture(), nullptr, nullptr, &texWidth, &texHeight);

	CurrentFrame(frameIndex, texWidth);

	if(loop || (timeElapsed <= delay * (currentFrames - 1))) timeElapsed += Time::SecondsToMilliseconds(Time::DeltaTime());

	mesh.SetSize(glm::vec2(texWidth, texHeight / totalRows));
	mesh.SetSource(SDL_Rect{ frameIndex, currentRow * (texHeight / totalRows), texWidth, texHeight / totalRows});
}

void Animation::CurrentFrame(int& index, int& texWidth)
{
	int frameSize;

	if (totalFrames == 0)
	{
		index = 0;
		return;
	}

	frameSize = texWidth / totalFrames;

	index = CurrentFrameIndex() * frameSize;

	texWidth = frameSize;
}

int Animation::CurrentFrameIndex()
{	
	int index = (((int)timeElapsed / (int)delay) % currentFrames);

	return index;
}

// Animator

Animator::Animator()
	: Component(Entity::recentEntity)
{
	
}

Animator::Animator(const std::initializer_list<std::pair<std::string, Ref<Animation>>>& animations)
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

void Animator::AddEdge(const std::string& source, const std::string& destination)
{
	controller.AddEdge(source, destination);
}

void Animator::RemoveEdge(const std::string& source, const std::string& destination)
{
	controller.RemoveEdge(source, destination);
}

void Animator::AddParameter(const std::string& name, Type type, void* value)
{
	controller.AddParameter(name, type, value);
}
void Animator::RemoveParameter(const std::string& name)
{
	controller.RemoveParameter(name);
}
void Animator::ChangeParameterValue(const std::string& name, void* value)
{
	controller.ChangeParameterValue(name, value);
}

void Animator::AddConditionOnEdge(const std::string& source, const std::string& destination, const std::string& parameter,
	Operation::OperationFunc operation, void* valueToCompare, Type valueToCompareType)
{
	controller.AddConditionOnEdge(source, destination, parameter, operation, valueToCompare, valueToCompareType);
}

void Animator::RemoveConditionOffEdge(const std::string& source, const std::string& destination, const std::string& parameter)
{
	controller.RemoveConditionOffEdge(source, destination, parameter);
}

// Text

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

Text::Text()
	: Component(Entity::recentEntity)
{

}

Text::Text(const std::string& label, const std::string& font_path, int font_size, const SDL_Color& color)
	: Component(Entity::recentEntity), label(label), font_path(font_path), font_size(font_size), color(color)
{
	UpdateMesh();
}

Text::Text(const Text& text)
{
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

	auto& mesh = e.GetComponent<Mesh>();

	int texWidth, texHeight;
	SDL_Texture* texture = Utils::LoadFont(label, font_path, font_size, color);

	SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);

	mesh.SetTexture(texture);
	mesh.SetSource(SDL_Rect{ 0, 0, texWidth, texHeight });
	mesh.SetSize(glm::vec2(texWidth, texHeight));
}