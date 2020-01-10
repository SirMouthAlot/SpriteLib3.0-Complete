#ifndef __SPRITE_H__
#define __SPRITE_H__
#include <string>
#include <GL/glew.h>
#include "AnimationController.h"

class Sprite
{
public:
	//Empty constructor
	Sprite() { m_bottomLeft = vec2(0.0, 0.0); m_topRight = vec2(1.0, 1.0); }
	//Honestly you won't use this constructor
	//As when you add the Animated Sprite component
	//You won't be able to give parameters to the component when you create it
	Sprite(std::string& fileName, int width, int height);

	//Loads sprite
	//*Checks if uvs are setup
	//*Initializes sprite plane
	//*Grabs VertVBO from VertexManager
	bool LoadSprite(std::string& fileName, int width, int height, bool Animated=false, AnimationController* anim=nullptr);
	//Draws sprite
	//*Just draws it
	void Draw();

	//Binds the texture of the sprite
	void Bind() const;
	//Binds the texture of the sprite at textureSlot
	void Bind(int textureSlot) const;
	//Unbinds the texture of the sprite
	void Unbind() const;
	//Unbinds the texture of the sprite at textureSlot
	void Unbind(int textureSlot) const;

	//Sets the width of the sprite
	void SetWidth(int width);
	//Gets the width of the sprite
	int GetWidth() const;
	//Sets the height of the sprite
	void SetHeight(int height);
	//Gets the height of the sprite
	int GetHeight() const;

	//Sets the scale of the size
	//*Basically will scale up or down the width and height
	//*Useful for slight adjustments for the automatically set width and height variables
	void SetSizeScale(float size);
	//Gets the scale of the size
	float GetSizeScale() const;

	//Set uvs in pixel space
	//(will be converted to 0-1 within the function
	void SetUVs(vec2 BottomLeft, vec2 TopRight);
	void SetBottomLeft(vec2 bL);
	vec2 GetBottomLeft() const;
	void SetTopRight(vec2 tR);
	vec2 GetTopRight() const;

	//Initailizes the VAO sprite plane
	void InitSpritePlane(bool animated, GLuint uvVBO);

	GLuint GetVAO() const;

	void SetFileName(std::string fileName);
	std::string GetFileName() const;
	vec2 GetTextureSize() const;

	bool GetIsSpriteSheet() const;
	void SetIsSpriteSheet(bool isSpriteSheet);
protected:
	//The texture handle
	GLuint m_handle = GL_NONE;
	//The texture size
	vec2 m_textureSize = vec2();
	std::string m_fileName;

	//The width and height
	int m_width = 0;
	int m_height = 0;
	
	vec2 m_bottomLeft = vec2(0.f, 0.f);
	vec2 m_topRight = vec2(1.f, 1.f);

	//The size scale
	float m_size = 1.f;

	//The sprite plane VAO
	GLuint m_spritePlaneVAO = GL_NONE;
	//The VBO will be the same for all items
	//thus is static
	static GLuint m_spritePlaneVertVBO;

	//has the spriteplane been initialized
	bool m_spritePlaneInit = false;
	//The UV vbo
	GLuint m_spritePlaneUVVBO = GL_NONE;

	//Sprite is spritesheet
	bool m_isSpritesheet = false;
};


#ifndef __SPRITEJSON_H__
#define __SPRITEJSON_H__

//Sends animation TO json file
inline void to_json(nlohmann::json& j, const Sprite& spr)
{
	//Stores width data
	j["Width"] = spr.GetWidth();
	//Stores height data
	j["Height"] = spr.GetHeight();
	//Stores filename
	j["FileName"] = spr.GetFileName();
	//Stores scale data
	j["SizeScale"] = spr.GetSizeScale();

	//Stores if object is a spritesheet (if you adjust the uvs, it's a spritesheet)
	j["IsSpriteSheet"] = spr.GetIsSpriteSheet();

	//Stores uv Data
	j["BottomLeft"] = { spr.GetBottomLeft().x, spr.GetBottomLeft().y };
	j["TopRight"] = { spr.GetTopRight().x, spr.GetTopRight().y };
}

//Reads animation in FROM json file
inline void from_json(const nlohmann::json& j, Sprite& spr)
{
	//Sets width
	spr.SetWidth(j["Width"]);
	//Sets height
	spr.SetHeight(j["Height"]);
	//Sets filename
	spr.SetFileName(j["FileName"]);
	//Sets scale data
	spr.SetSizeScale(j["SizeScale"]);

	//Sets if it's a spritesheet
	spr.SetIsSpriteSheet(j["IsSpriteSheet"]);

	//Sets UV data
	spr.SetBottomLeft(vec2(j["BottomLeft"][0], j["BottomLeft"][1]));
	spr.SetTopRight(vec2(j["TopRight"][0], j["TopRight"][1]));
}

#endif // !__SPRITEJSON_H__

#endif // !__SPRITE_H__
