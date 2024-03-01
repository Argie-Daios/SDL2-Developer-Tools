#pragma once

#include "Core/Scene.h"

#include <unordered_map>
#include <string>
#include <entt.hpp>
#include <glm/glm.hpp>

struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;
struct SDL_Color;
class Entity;
class Scene;
struct Texture
{
	SDL_Texture* texture = nullptr;
	std::string filepath = "-";
	int width = 0;
	int height = 0;
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
	glm::vec3 color;
};

struct AnimationProperties
{
	std::string textureID = "-";
	int totalFrames = 0;
	int totalFramesPerRow = 0;
	int totalRows = 0;
	int firstFrame = 0;
	int lastFrame = 0;
};

enum class ASSET_TYPE
{
	TEXTURE,
	ANIMATION,
	FONT,
	TEXT,
	PREFAB
};

class AssetManager
{
public:
	static void LoadTexture(const std::string& name, const std::string filepath);
	static Texture& GetTexture(const std::string& name);

	static void LoadAnimation(const std::string& name, const AnimationProperties& animationProperties);
	static AnimationProperties& GetAnimation(const std::string& name);

	static void LoadFont(const std::string& name, const std::string& filepath, int fontSize);
	static Font& GetFont(const std::string& name);

	static void LoadText(const std::string& name, const std::string& label, const std::string& font, const glm::vec3& color);
	static bool ChangeTextLabel(const std::string& name, const std::string& label);
	static bool ChangeTextFont(const std::string& name, const std::string font);
	static bool ChangeTextColor(const std::string& name, const glm::vec3& color);
	static TextProperties& GetText(const std::string& name);

	static void CreatePrefab(const std::string& name, Entity entity);
	static Entity Prefab(const std::string& name);

	// TODO : MAKE CLEAR FUNCTION
	static void Clear();
private:
	static bool isAlreadyIn(const std::string& name, const ASSET_TYPE& type);
	static void ChangeTexture(const std::string& name, SDL_Texture* texture, int width, int height);
	static SDL_Texture* GenerateTextTexture(const std::string& label, const std::string& font, const glm::vec3& color);

	static void ClearTextures();
	static void ClearFonts();
	static void ClearText();
	static void ClearPrefabs();
private:
	static std::unordered_map<std::string, Texture> s_Textures;
	static std::unordered_map<std::string, AnimationProperties> s_Animations;
	static std::unordered_map<std::string, Font> s_Fonts;
	static std::unordered_map<std::string, TextProperties> s_Texts;
	static std::unordered_map<std::string, Entity> s_Prefabs;

	static Ref<Scene> s_PrefabDummyScene;

	friend class Entity;
};