#include "Sprite.h"

GLuint Sprite::m_spritePlaneVertVBO = GL_NONE;

Sprite::Sprite(std::string& fileName, int width, int height)
{
	//Loads the sprite
	LoadSprite(fileName, width, height);
}

bool Sprite::LoadSprite(std::string& fileName, int width, int height, bool animated, AnimationController* anim)
{
	//If spritePlane not initialized
		//Grab the Vert VBO
		//Init the sprite plane
		//Set to true
	if (!m_spritePlaneInit)
	{
		m_spritePlaneVertVBO = VertexManager::GetPlaneVertVBO();

		InitSpritePlane(animated, ((anim == nullptr) ? -1 : anim->GetUVVBO()));
		m_spritePlaneInit = true;
	}
	
	(anim == nullptr) ? void() : anim->SetVAO(m_spritePlaneVAO);
	
	//Set the file name
	m_fileName = fileName;

	//Get texture handle of filename
	//*If the texture is loaded, we just find it and return the handle
	//*If it isn't we load it and return the handle
	m_handle = TextureManager::GetHandle(fileName);
	//Get the texture size of filename
	//*Returns the texture size
	m_textureSize = TextureManager::GetTextureSize(fileName);

	//Sets the width and height
	m_width = width;
	m_height = height;
	return true;
}

void Sprite::Draw()
{
	//Binds the texture to slot zero
	Bind(0);
	//Bind vertex array
	glBindVertexArray(m_spritePlaneVAO);
	//Draw the array
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//unbind vertex array
	glBindVertexArray(GL_NONE);
	//Unbinds texture at slot zero
	Unbind(0);
}

void Sprite::Bind() const
{
	//Bind texture handle
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Sprite::Bind(int textureSlot) const
{
	//Sets the active texture to textureslot
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	//Binds texture handle
	Bind();
}

void Sprite::Unbind() const
{
	//Unbinds texture 
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Sprite::Unbind(int textureSlot) const
{
	//Sets the active texture to textureslot
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	//Unbinds texture
	Unbind();
}

void Sprite::SetWidth(int width)
{
	m_width = width;
}

int Sprite::GetWidth() const
{
	//grabs width
	return m_width;
}

void Sprite::SetHeight(int height)
{
	m_height = height;
}

int Sprite::GetHeight() const
{
	//grabs height
	return m_height;
}

void Sprite::SetSizeScale(float size)
{
	//Sets the size scale
	m_size = size;
}

float Sprite::GetSizeScale() const
{
	//grabs size scale
	return m_size;
}

void Sprite::SetUVs(vec2 BottomLeft, vec2 TopRight)
{
	m_isSpritesheet = true;

	m_bottomLeft = BottomLeft;
	m_topRight = TopRight;

	//Creates the VBO_DATA
	//*Puts the values within a 0-1 range
	//*Takes just the bottom left and top right and creates all other corners
	float VBO_DATA[]{
		BottomLeft.x / m_textureSize.x, BottomLeft.y / m_textureSize.y,
		TopRight.x / m_textureSize.x, BottomLeft.y / m_textureSize.y,
		BottomLeft.x / m_textureSize.x, TopRight.y / m_textureSize.y,

		TopRight.x / m_textureSize.x, TopRight.y / m_textureSize.y,
		BottomLeft.x / m_textureSize.x, TopRight.y / m_textureSize.y,
		TopRight.x / m_textureSize.x, BottomLeft.y / m_textureSize.y
	};
	int texCoordSize = 6 * 2 * sizeof(float);

	//Bind VAO
	glBindVertexArray(m_spritePlaneVAO);
	//Bind UV VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_spritePlaneUVVBO);

	//Buffer subdata
	glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordSize, VBO_DATA);
}

void Sprite::SetBottomLeft(vec2 bL)
{
	m_bottomLeft = bL;
}

vec2 Sprite::GetBottomLeft() const
{
	return m_bottomLeft;
}

void Sprite::SetTopRight(vec2 tR)
{
	m_topRight = tR;
}

vec2 Sprite::GetTopRight() const
{
	return m_topRight;
}

//Sets up the sprite plane (VAO and UVs)
void Sprite::InitSpritePlane(bool animated, GLuint uvVBO)
{
	//Initializes VBO data as
	//0-1 from bottom left to top right
	float VBO_DATA[]{
		0.f, 1.f,
		1.f, 1.f,
		0.f, 0.f,

		1.f, 0.f,
		0.f, 0.f,
		1.f, 1.f,
	};
	int texCoordSize = 6 * 2 * sizeof(float);

	
	//Creates a new VAO (able to be unloaded upon program termination)
	m_spritePlaneVAO = VertexManager::CreateVAO();
	glBindVertexArray(m_spritePlaneVAO);

	//Enables slot 0 and 1
	glEnableVertexAttribArray(0); //vertices
	glEnableVertexAttribArray(1); //UV coordinates
	
	//Creates a new VBO (able to be unloaded upon program termination)
	if (!animated)
		m_spritePlaneUVVBO = VertexManager::CreateVBO();
	else
		m_spritePlaneUVVBO = uvVBO;

	//Pushes away the warnings
	#pragma warning(push)
	#pragma warning(disable : 4312)
	//Bind Vert VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_spritePlaneVertVBO);
	//Sets the vertex attributes
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));			//send vertex attributes
	//Bind UV VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_spritePlaneUVVBO);
	//Uses dynamic draw so you can rebuffer the data and change the UVs
	if (!animated)
		glBufferData(GL_ARRAY_BUFFER, texCoordSize, VBO_DATA, GL_DYNAMIC_DRAW);
	//Sets the uv attributes
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));		//send uv attributes
	#pragma warning(pop)

	//Unbinds the buffers
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

GLuint Sprite::GetVAO() const
{
	return m_spritePlaneVAO;
}

void Sprite::SetFileName(std::string fileName)
{
	m_fileName = fileName;
}

std::string Sprite::GetFileName() const
{
	return m_fileName;
}

vec2 Sprite::GetTextureSize() const
{
	return m_textureSize;
}

bool Sprite::GetIsSpriteSheet() const
{
	return m_isSpritesheet;
}

void Sprite::SetIsSpriteSheet(bool isSpriteSheet)
{
	m_isSpritesheet = isSpriteSheet;
}
