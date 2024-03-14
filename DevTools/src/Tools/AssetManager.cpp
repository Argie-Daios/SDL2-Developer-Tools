#include "AssetManager.h"

#include "Core/Macros.h"
#include "Renderer/Renderer.h"
#include "ECS/Entity.h"
#include "AnimationController/AnimationController.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

std::unordered_map<std::string, Texture> AssetManager::s_Textures;
std::unordered_map<std::string, Spritesheet> AssetManager::s_Spritesheets;
std::unordered_map<std::string, AnimationProperties> AssetManager::s_Animations;
std::unordered_map<std::string, Font> AssetManager::s_Fonts;
std::unordered_map<std::string, TextProperties> AssetManager::s_Texts;
std::unordered_map<std::string, Entity> AssetManager::s_Prefabs;
std::unordered_map<std::string, AnimationController> AssetManager::s_AnimationControllers;
Ref<Scene> AssetManager::s_PrefabDummyScene = CreateRef<Scene>();

void AnimationNode::Reset()
{
	currentFrame = 0;
	timeElapsed = 0.0f;
}

void AnimationNode::Update()
{
	int index = (int)(timeElapsed / delay);
	auto it = AssetManager::GetAnimation(animationID);
	currentFrame = index % it.frames + it.firstFrame;
	timeElapsed += Time::SecondsToMilliseconds(Time::DeltaTime());
}

bool AnimationNode::isComplete()
{
	int frames = AssetManager::GetAnimation(animationID).frames;
	return timeElapsed >= delay * frames;
}

namespace Utils
{
	struct Sides
	{
		int Up = 0;
		int Down = 0;
		int Left = 0;
		int Right = 0;
	};

	enum class Side
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	static bool CheckSurfaceBound(SDL_Surface* surface, int BoundX, int BoundY)
	{
		Uint32 pixel = ((Uint32*)surface->pixels)[BoundY * surface->w + BoundX];
		Uint8 r, g, b, a;
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		if (a > 30)
		{
			return true;
		}

		//std::cout << "Transparen" << std::endl;

		return false;
	}

	static bool CheckSurfaceBounds(SDL_Surface* surface, int firstBoundX, int firstBoundY, int secondBoundX, int secondBoundY)
	{
		return CheckSurfaceBound(surface, firstBoundX, firstBoundY) || CheckSurfaceBound(surface, secondBoundX, secondBoundY);
	}

	static int GetShrinkToFitSide(SDL_Surface* surface, const Side& side)
	{
		int w = surface->w;
		int h = surface->h;
		int decreasedSide = 0;

		switch (side)
		{
		case Side::UP:
		{
			while (decreasedSide < h / 2)
			{
				for (int i = 0; i < w; i++)
				{
					if (Utils::CheckSurfaceBound(surface, i, decreasedSide))
					{
						return decreasedSide;
					}
				}
				decreasedSide++;
			}
			return decreasedSide;
		}
		case Side::DOWN:
		{
			while (decreasedSide < h / 2)
			{
				for (int i = 0; i < w; i++)
				{
					if (Utils::CheckSurfaceBound(surface, i, h - decreasedSide - 1))
					{
						return decreasedSide;
					}
				}
				decreasedSide++;
			}
			return decreasedSide;
		}
		case Side::LEFT:
		{
			while (decreasedSide < w / 2)
			{
				for (int j = 0; j < h; j++)
				{
					if (Utils::CheckSurfaceBound(surface, decreasedSide, j))
					{
						return decreasedSide;
					}
				}
				decreasedSide++;
			}
			return decreasedSide;
		}
		case Side::RIGHT:
		{
			while (decreasedSide < w / 2)
			{
				for (int j = 0; j < h; j++)
				{
					if (Utils::CheckSurfaceBound(surface, w - decreasedSide - 1, j))
					{
						return decreasedSide;
					}
				}
				decreasedSide++;
			}
			return decreasedSide;
		}
		}

		GAME_ASSERT(false, "There is no such side");
		return 0;
	}

	static Sides GetShrinkToFitSides(SDL_Surface* surface)
	{
		return Sides{ GetShrinkToFitSide(surface, Side::UP), GetShrinkToFitSide(surface, Side::DOWN),
		GetShrinkToFitSide(surface, Side::LEFT), GetShrinkToFitSide(surface, Side::RIGHT) };
	}
}

void AssetManager::LoadTexture(const std::string& name, const std::string filepath)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is already a texture with the name : " + name);

	SDL_Texture* texture = IMG_LoadTexture(Renderer::s_Renderer, filepath.c_str());
	GAME_ASSERT(texture, "Failed to load texture!!");

	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	s_Textures.emplace(name, Texture{ texture, filepath, width, height});
}

void AssetManager::LoadTextureAndShrinkToFit(const std::string& name, const std::string filepath)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is already a texture with the name : " + name);

	SDL_Surface* surface = IMG_Load(filepath.c_str());

	int w = surface->w;
	int h = surface->h;
	Utils::Sides sides = Utils::GetShrinkToFitSides(surface);

	int trimedW = w - sides.Right - sides.Left;
	int trimedH = h - sides.Down - sides.Up;
	SDL_Rect trimedRect = { sides.Left, sides.Up, trimedW, trimedH };
	SDL_Surface* croppedSurface = SDL_CreateRGBSurface(0, trimedW, trimedH, 32, 0, 0, 0, 0);

	SDL_BlitSurface(surface, &trimedRect, croppedSurface, NULL);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::s_Renderer, croppedSurface);
	GAME_ASSERT(texture, "Failed to load texture!!");

	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	s_Textures.emplace(name, Texture{ texture, filepath, width, height });

	SDL_FreeSurface(surface);
	SDL_FreeSurface(croppedSurface);
}

const Texture& AssetManager::GetTexture(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is no such texture with the name : " + name);

	return s_Textures[name];
}

void AssetManager::LoadSpritesheet(const std::string& name, const std::string& filepath, int framesPerRow, int rows)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::SPRITESHEET), "There is already a spritesheet with the name : " + name);
	GAME_ASSERT(framesPerRow > 0, "Invalid framesPerRow value!!");
	GAME_ASSERT(rows > 0, "Invalid rows value!!");

	SDL_Surface* surface = IMG_Load(filepath.c_str());

	int w = surface->w;
	int h = surface->h;

	int frameWidth = w / framesPerRow;
	int frameHeight = h / rows;

	s_Spritesheets.emplace(name, Spritesheet{std::vector<std::string>(), framesPerRow * rows});
	Spritesheet& spriteSheet = s_Spritesheets[name];

	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < framesPerRow; i++)
		{
			std::string name = randomStringGenerator(20);
			SDL_Rect trimedRect = SDL_Rect{i * frameWidth, j * frameHeight, frameWidth, frameHeight};
			SDL_Surface* croppedSurface = SDL_CreateRGBSurface(0, frameWidth, frameHeight, 32, 0, 0, 0, 0);
			SDL_BlitSurface(surface, &trimedRect, croppedSurface, NULL);
			LoadTexture(name, croppedSurface);
			spriteSheet.frameIDs.push_back(name);
			SDL_FreeSurface(croppedSurface);
		}
	}

	SDL_FreeSurface(surface);
}

void AssetManager::LoadSpritesheetAndShrinkToFit(const std::string& name, const std::string& filepath, int framesPerRow, int rows)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::SPRITESHEET), "There is already a spritesheet with the name : " + name);
	GAME_ASSERT(framesPerRow > 0, "Invalid framesPerRow value!!");
	GAME_ASSERT(rows > 0, "Invalid rows value!!");

	SDL_Surface* surface = IMG_Load(filepath.c_str());

	int w = surface->w;
	int h = surface->h;

	int frameWidth = w / framesPerRow;
	int frameHeight = h / rows;

	s_Spritesheets.emplace(name, Spritesheet{ std::vector<std::string>(), framesPerRow * rows });
	Spritesheet& spriteSheet = s_Spritesheets[name];

	int minW = 0;
	int minH = 0;

	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < framesPerRow; i++)
		{
			std::string name = randomStringGenerator(20);
			SDL_Rect trimedRect = SDL_Rect{ i * frameWidth, j * frameHeight, frameWidth, frameHeight };
			SDL_Surface* croppedSurface = SDL_CreateRGBSurfaceWithFormat(0, frameWidth, frameHeight, 32, surface->format->format);
			SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
			SDL_BlitSurface(surface, &trimedRect, croppedSurface, NULL);
			const auto& tex = LoadTextureAndShrinkToFit(name, croppedSurface);

			minW = std::min(tex.width, minW);
			minH = std::min(tex.height, minH);

			spriteSheet.frameIDs.push_back(name);
			SDL_FreeSurface(croppedSurface);
		}
	}

	SDL_FreeSurface(surface);
}

const Spritesheet& AssetManager::GetSpritesheet(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::SPRITESHEET), "There is no such a spritesheet with the name : " + name);

	return s_Spritesheets[name];
}

std::string AssetManager::GetTextureIDFromSpritesheet(const std::string& name, int index)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::SPRITESHEET), "There is no such a spritesheet with the name : " + name);

	auto it = s_Spritesheets[name];
	GAME_ASSERT(!it.frameIDs.empty() && index < it.frames && index >= 0, "Invalid index value");

	return it.frameIDs[index];
}

void AssetManager::LoadAnimation(const std::string& name, const std::string& spritesheetID, int firstFrame, int frames)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::ANIMATION), "There is already an animation with the name : " + name);
	GAME_ASSERT(isAlreadyIn(spritesheetID, ASSET_TYPE::SPRITESHEET), "There is no such a spritesheet with the name : " + spritesheetID);
	
	auto it = s_Spritesheets[spritesheetID];
	GAME_ASSERT(firstFrame < it.frames && firstFrame >= 0, "Invalid value for firstFrame");
	GAME_ASSERT(frames <= it.frames && frames > 0, "Invalid value for firstFrame");
	GAME_ASSERT(firstFrame + frames <= it.frames, "Overflow");

	s_Animations.emplace(name, AnimationProperties{ spritesheetID, firstFrame, frames });
}

const AnimationProperties& AssetManager::GetAnimation(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::ANIMATION), "There is no such an animation with the name : " + name);

	return s_Animations[name];
}

void AssetManager::LoadFont(const std::string& name, const std::string& filepath, int fontSize)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::FONT), "There is already a font with the name : " + name);

	TTF_Font* font = TTF_OpenFont(filepath.c_str(), fontSize);
	if (font == nullptr) std::cout << "Failed to load font in path : " << filepath.c_str() << std::endl;
	GAME_ASSERT(font, "Failed to load font!!");

	s_Fonts.emplace(name, Font{ font, filepath, fontSize });
}

const Font& AssetManager::GetFont(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::FONT), "There is no such font with the name : " + name);

	return s_Fonts[name];
}

void AssetManager::LoadText(const std::string& name, const std::string& label, const std::string& font, const SDL_Color& color)
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

bool AssetManager::ChangeTextColor(const std::string& name, const SDL_Color& color)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXT), "There is no such a text with the name : " + name);

	TextProperties& text = s_Texts[name];
	if (color.r == text.color.r && color.g == text.color.g && color.b == text.color.b) return false;

	text.color = color;
	SDL_Texture* texture = GenerateTextTexture(text.label, text.font, text.color);
	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	ChangeTexture(name, texture, width, height);
	return true;
}

const TextProperties& AssetManager::GetText(const std::string& name)
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

void AssetManager::CreateAnimationController(const std::string& name, const AnimationController& animationController)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::ANIMATION_CONTROLLER), "There is already an animation controller with the name : " + name);

	s_AnimationControllers.emplace(name, animationController);
}

const AnimationController& AssetManager::GetAnimationController(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::ANIMATION_CONTROLLER), "There is no such an animation controller with the name : " + name);

	return s_AnimationControllers[name];
}

void AssetManager::Clear()
{
	ClearTextures();
	ClearFonts();
	ClearText();
	ClearPrefabs();
}

void AssetManager::LoadTexture(const std::string& name, SDL_Surface* surface)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is already a texture with the name : " + name);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::s_Renderer, surface);
	GAME_ASSERT(texture, "Failed to load texture!!");

	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	s_Textures.emplace(name, Texture{ texture, "-", width, height });
}

const Texture& AssetManager::LoadTextureAndShrinkToFit(const std::string& name, SDL_Surface* surface)
{
	GAME_ASSERT(!isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is already a texture with the name : " + name);

	int w = surface->w;
	int h = surface->h;

	Utils::Sides sides = Utils::GetShrinkToFitSides(surface);

	int trimedW = w - sides.Right - sides.Left;
	int trimedH = h - sides.Down - sides.Up;
	SDL_Rect trimedRect = { sides.Left, sides.Up, trimedW, trimedH };
	SDL_Surface* croppedSurface = SDL_CreateRGBSurfaceWithFormat(0, trimedW, trimedH, 32, surface->format->format);

	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
	SDL_BlitSurface(surface, &trimedRect, croppedSurface, NULL);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::s_Renderer, croppedSurface);
	GAME_ASSERT(texture, "Failed to load texture!!");

	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	s_Textures.emplace(name, Texture{ texture, "-", width, height});

	SDL_FreeSurface(croppedSurface);

	return s_Textures[name];
}

Texture& AssetManager::GetTextureRef(const std::string& name)
{
	GAME_ASSERT(isAlreadyIn(name, ASSET_TYPE::TEXTURE), "There is no such a texture with the name : " + name);

	return s_Textures[name];
}

bool AssetManager::isAlreadyIn(const std::string& name, const ASSET_TYPE& type)
{
	switch (type)
	{
	case ASSET_TYPE::TEXTURE: return s_Textures.find(name) != s_Textures.end();
	case ASSET_TYPE::SPRITESHEET: return s_Spritesheets.find(name) != s_Spritesheets.end();
	case ASSET_TYPE::ANIMATION: return s_Animations.find(name) != s_Animations.end();
	case ASSET_TYPE::FONT: return s_Fonts.find(name) != s_Fonts.end();
	case ASSET_TYPE::TEXT: return s_Texts.find(name) != s_Texts.end();
	case ASSET_TYPE::PREFAB: return s_Prefabs.find(name) != s_Prefabs.end();
	case ASSET_TYPE::ANIMATION_CONTROLLER: return s_AnimationControllers.find(name) != s_AnimationControllers.end();
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

SDL_Texture* AssetManager::GenerateTextTexture(const std::string& label, const std::string& font, const SDL_Color& color)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(s_Fonts[font].font, label.c_str(), color);
	GAME_ASSERT(text_surface, "Failed to create surface!!");

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(Renderer::s_Renderer, text_surface);
	GAME_ASSERT(text_texture, "Failed to load texture!!");

	SDL_FreeSurface(text_surface);
	return text_texture;
}

void AssetManager::ClearTextures()
{
	
}

void AssetManager::ClearFonts()
{

}

void AssetManager::ClearText()
{

}

void AssetManager::ClearPrefabs()
{

}