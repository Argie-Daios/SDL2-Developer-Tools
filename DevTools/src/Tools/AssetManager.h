#pragma once

#include "Core/Scene.h"
#include "Tools/Color.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <entt.hpp>
#include <glm/glm.hpp>
#include <SDL_pixels.h>

struct SDL_Texture;
struct SDL_Surface;
typedef struct _TTF_Font TTF_Font;
class Entity;
class Scene;
class AnimationController;
class Parameter;
struct Texture
{
	SDL_Texture* texture = nullptr;
	std::string filepath = "-";
	int width = 0;
	int height = 0;
};

struct Spritesheet
{
	std::vector<std::string> frameIDs;
	int frames = 0;
	int minW = 0;
	int minH = 0;
};

struct Font
{
	TTF_Font* font = nullptr;
	std::string fontpath = "-";
	int font_size = 0;
};

struct TextProperties
{
	std::string label = "-";
	std::string font = "-";
	SDL_Color color;
};

struct AnimationProperties
{
	std::string spritesheetID;
	int firstFrame = 0;
	int frames = 0;
};

struct AnimationNode
{
	std::string animationID;
	int currentFrame = 0;
	float delay = 100.0f;
	bool loop = true;
	float timeElapsed = 0.0f;

	void Reset();
	void Update();
	bool isComplete();
};

struct AnimationControllerProperties
{
	std::string currentAnimation = "-";
	std::unordered_map<std::string, Parameter> parameters;
};

enum class ASSET_TYPE
{
	TEXTURE,
	SPRITESHEET,
	ANIMATION,
	FONT,
	TEXT,
	PREFAB,
	ANIMATION_CONTROLLER
};

class AssetManager
{
public:
	static void LoadTexture(const std::string& name, const std::string filepath);
	static void LoadTextureAndShrinkToFit(const std::string& name, const std::string filepath);
	static const Texture& GetTexture(const std::string& name);

	static void LoadSpritesheet(const std::string& name, const std::string& filepath, int framesPerRow, int rows);
	static void LoadSpritesheetAndShrinkToFit(const std::string& name, const std::string& filepath, int framesPerRow, int rows);
	static const Spritesheet& GetSpritesheet(const std::string& name);
	static std::string GetTextureIDFromSpritesheet(const std::string& name, int index);

	static void LoadAnimation(const std::string& name, const std::string& spritesheetID, int firstFrame, int frames);
	static const AnimationProperties& GetAnimation(const std::string& name);

	static void LoadFont(const std::string& name, const std::string& filepath, int fontSize);
	static const Font& GetFont(const std::string& name);

	static void LoadText(const std::string& name, const std::string& label, const std::string& font, const SDL_Color& color);
	static bool ChangeTextLabel(const std::string& name, const std::string& label);
	static bool ChangeTextFont(const std::string& name, const std::string font);
	static bool ChangeTextColor(const std::string& name, const SDL_Color& color);
	static const TextProperties& GetText(const std::string& name);

	static void CreatePrefab(const std::string& name, Entity entity);
	static Entity Prefab(const std::string& name);

	static void CreateAnimationController(const std::string& name, const AnimationController& animationController);
	static const AnimationController& GetAnimationController(const std::string& name);

	// TODO : MAKE CLEAR FUNCTION
	static void Clear();
private:
	static void LoadTexture(const std::string& name, SDL_Surface* surface);
	static const Texture& LoadTextureAndShrinkToFit(const std::string& name, SDL_Surface* surface);
	static Texture& GetTextureRef(const std::string& name);

	static bool isAlreadyIn(const std::string& name, const ASSET_TYPE& type);
	static void ChangeTexture(const std::string& name, SDL_Texture* texture, int width, int height);
	static SDL_Texture* GenerateTextTexture(const std::string& label, const std::string& font, const SDL_Color& color);

	static void ClearTextures();
	static void ClearFonts();
	static void ClearText();
	static void ClearPrefabs();
private:
	static std::unordered_map<std::string, Texture> s_Textures;
	static std::unordered_map<std::string, Spritesheet> s_Spritesheets;
	static std::unordered_map<std::string, AnimationProperties> s_Animations;
	static std::unordered_map<std::string, Font> s_Fonts;
	static std::unordered_map<std::string, TextProperties> s_Texts; // Need to rethink if this should be an asset
	static std::unordered_map<std::string, Entity> s_Prefabs;
	static std::unordered_map<std::string, AnimationController> s_AnimationControllers;

	static Ref<Scene> s_PrefabDummyScene;

	friend class Entity;
};