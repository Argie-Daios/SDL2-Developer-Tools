#include "AssetManager.h"

#include "Core/Macros.h"
#include "Renderer/Renderer.h"
#include "ECS/Entity.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

std::unordered_map<std::string, Texture> AssetManager::s_Textures;
std::unordered_map<std::string, Font> AssetManager::s_Fonts;
std::unordered_map<std::string, TextProperties> AssetManager::s_Texts;
std::unordered_map<std::string, Entity> AssetManager::s_Prefabs;
Ref<Scene> AssetManager::s_PrefabDummyScene = CreateRef<Scene>();

void AssetManager::LoadTexture(const std::string& name, const std::string filepath)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is already a texture with the name : " + name);

	SDL_Texture* texture = IMG_LoadTexture(Renderer::s_Renderer, filepath.c_str());
	GAME_ASSERT(texture, "Failed to load texture!!");

	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	s_Textures.emplace(name, Texture{texture, filepath, width, height});
}

Texture AssetManager::GetTexture(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is no such texture with the name : " + name);

	return s_Textures[name];
}

void AssetManager::LoadFont(const std::string& name, const std::string& filepath, int fontSize)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::FONT), "There is already a font with the name : " + name);

	TTF_Font* font = TTF_OpenFont(filepath.c_str(), fontSize);
	GAME_ASSERT(font, "Failed to load font!!");

	s_Fonts.emplace(name, Font{ font, filepath, fontSize });
}

Font AssetManager::GetFont(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::FONT), "There is no such font with the name : " + name);

	return s_Fonts[name];
}

void AssetManager::LoadText(const std::string& name, const std::string& label, const std::string& font, const glm::vec3& color)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::TEXT), "There is already a text with the name : " + name);
	GAME_ASSERT(isAlreadyIn(font, ASSET_TYPE::FONT), "There is no such font with the name : " + font);
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is already a texture with the name : " + name);

	SDL_Texture* texture = GenerateTextTexture(label, font, color);
	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	s_Texts.emplace(name, TextProperties{ label, font, color });
	s_Textures.emplace(name, Texture{ texture, "-", width, height });
}

bool AssetManager::ChangeTextLabel(const std::string& name, const std::string& label)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXT), "There is no such a text with the name : " + name);

	TextProperties& text = s_Texts[name];
	if (label == text.label) return false;

	text.label = label;
	SDL_Texture* texture = GenerateTextTexture(text.label, text.font, text.color);
	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	ChangeTexture(name, texture, width, height);
	return true;
}

bool AssetManager::ChangeTextFont(const std::string& name, const std::string font)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXT), "There is no such a text with the name : " + name);
	GAME_ASSERT(isAlreadyIn(font, ASSET_TYPE::FONT), "There is no such font with the name : " + font);

	TextProperties& text = s_Texts[name];
	if (font == text.font) return false;

	text.font = font;
	SDL_Texture* texture = GenerateTextTexture(text.label, text.font, text.color);
	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	ChangeTexture(name, texture, width, height);
	return true;
}

bool AssetManager::ChangeTextColor(const std::string& name, const glm::vec3& color)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXT), "There is no such a text with the name : " + name);

	TextProperties& text = s_Texts[name];
	if (color.x == text.color.x && color.y == text.color.y && color.z == text.color.z) return false;

	text.color = color;
	SDL_Texture* texture = GenerateTextTexture(text.label, text.font, text.color);
	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	ChangeTexture(name, texture, width, height);
	return true;
}

TextProperties AssetManager::GetText(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXT), "There is no such a text with the name : " + name);

	return s_Texts[name];
}

void AssetManager::CreatePrefab(const std::string& name, Entity entity)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::PREFAB), "There is already a prefab with the name : " + name);

	Entity prefabEntity = entity.CreatePrefabAndCopyComponents();

	std::cout << "Name : " << entity.name() << std::endl;

	s_Prefabs.emplace(name, prefabEntity);
}

Entity AssetManager::Prefab(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::PREFAB), "There is no such a prefab with the name : " + name);

	return s_Prefabs[name];
}

bool AssetManager::isAlreadyIn(const std::string& name, const ASSET_TYPE& type)
{
	switch (type)
	{
	case ASSET_TYPE::TEXTURE: return s_Textures.find(name) != s_Textures.end();
	case ASSET_TYPE::FONT: return s_Fonts.find(name) != s_Fonts.end();
	case ASSET_TYPE::TEXT: return s_Texts.find(name) != s_Texts.end();
	case ASSET_TYPE::PREFAB: return s_Prefabs.find(name) != s_Prefabs.end();
	}

	GAME_ASSERT(false, "No such an asset type");
	return false;
}

void AssetManager::ChangeTexture(const std::string& name, SDL_Texture* texture, int width, int height)
{
	Texture& textureRef = s_Textures[name];

	SDL_DestroyTexture(textureRef.texture);
	textureRef.texture = texture;
	textureRef.width = width;
	textureRef.height = height;
}

SDL_Texture* AssetManager::GenerateTextTexture(const std::string& label, const std::string& font, const glm::vec3& color)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(s_Fonts[font].font, label.c_str(), SDL_Color{ (unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z });
	GAME_ASSERT(text_surface, "Failed to create surface!!");

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(Renderer::s_Renderer, text_surface);
	GAME_ASSERT(text_texture, "Failed to load texture!!");

	SDL_FreeSurface(text_surface);
	return text_texture;
}