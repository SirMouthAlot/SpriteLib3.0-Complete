#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "GL/glew.h"
#include <string>


//Holds the texture filter
struct TextureFilter
{
	GLenum m_magFilter = GL_NEAREST;
	GLenum m_minFilter = GL_NEAREST_MIPMAP_NEAREST;
};

//Filter modes
GLenum filterModes[];

//Warning disable
#pragma warning(push)
#pragma warning(disable : 4201) //Nameless sturct disable
//Holds the texture wrap type
struct TextureWrap
{
	TextureWrap();

	union
	{
		//Allows both
		//xyz
		//and
		//str
		//be used to access the same data
		struct
		{
			GLenum x, y, z;
		};
		struct
		{
			GLenum s, t, r;
		};
	};
};
#pragma warning(pop)


class Texture
{
public:
	//Allows these classes access to protected and private members
	friend struct DepthTarget;
	friend struct ColorTarget;
	friend class Framebuffer;

	//Empty constructor
	Texture() {};
	~Texture();

	//Loads in the texture
	bool Load(const std::string &file);
	//Unloads the texture
	bool Unload();

	// Description:
	// Creates the texture, allocates memory and uploads the data to the GPU
	// If you do not want to upload the data to the GPU you can simply pass a nullptr for the dataptr value
	void CreateTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void* newDataPtr);

	//Binds the texture to the last active slot
	void Bind() const;
	//Binds the texture to a specific slot
	void Bind(int textureSlot) const;
	//Unbinds the texture from the last active slot
	void Unbind() const;
	//Unbinds the texture from a specific slot
	void Unbind(int textureSlot) const;

	//Gets the texture ID
	GLuint GetID() const;

	//Get the width and height size of the textures
	int GetSizeX() const;
	int GetSizeY() const;

	//Sends the new texture parameters
	void SendTexParameters();
	//Sets the filter parameters
	void SetFilterParameters(GLenum magFilter, GLenum minFilter);
	//Sets the wrap parameters
	void SetWrapParameters(GLenum wrap);

	//Anisotropy amount
	static float anisotropyAmount;
private:
	//Texture file name
	std::string m_fileName = "";
	//Filetype is png
	std::string m_fileType = ".png";

	//Internal format (ends in bit count)
	GLenum m_internalFormat = GL_RGBA8;
	//Actual format (doesn't have bit count)
	GLenum m_format = GL_RGBA;
	//Target is 2D Texture
	GLenum m_target = GL_TEXTURE_2D;
	//Default filters
	TextureFilter m_filter;
	//Default wraps
	TextureWrap m_wrap;

	//Texture handle
	GLuint m_handle = GL_NONE;

	//Width and height
	int m_sizeX = 0;
	int m_sizeY = 0;
	//How many channels (4 = RGBA)
	int m_channels = 4;
	
	//Main texture directory
	static std::string m_TextureDirectory;
};

#endif // !__TEXTURE_H__
