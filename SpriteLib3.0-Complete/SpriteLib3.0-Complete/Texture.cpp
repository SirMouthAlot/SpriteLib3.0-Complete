#include "Texture.h"
#include "SOIL/SOIL.h"
#include <iostream>

//Default texture directory
std::string Texture::m_TextureDirectory = "./assets/sprites/";

//Anisotropy amount is 16x
float Texture::anisotropyAmount = 16.f;

//List of filter modes
GLenum filterModes[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};


Texture::~Texture()
{
	Unload();
}

bool Texture::Load(const std::string & file)
{
	//File name is equal to default directory + file
	m_fileName = m_TextureDirectory + file;

	//Loads in the texture data (ONLY to get the sizeX and sizeY)
	unsigned char* textureData = SOIL_load_image((m_fileName).c_str(),
		&m_sizeX, &m_sizeY, &m_channels, SOIL_LOAD_AUTO);

	//Automatically loads in the texture and creates all OpenGL related handles
	m_handle = SOIL_load_OGL_texture(m_fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

	//Bind the texture
	Bind();

	//Set the parameters
	glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_filter.m_minFilter);
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_filter.m_magFilter);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, m_wrap.s);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, m_wrap.t);
	
	//Unbinds the texture
	Unbind();
	//Free the image data we loaded into textureData
	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::Unload()
{
	//if the handle isn't GL_NONE
	if (m_handle)
	{
		//Delete the texture
		glDeleteTextures(1, &m_handle);
		return true;
	}
	return false;
}

void Texture::CreateTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void * newDataPtr)
{
	//Stores all the parameters
	m_sizeX = w;
	m_sizeY = h;
	m_filter.m_magFilter = filtering;
	m_wrap.s = edgeBehaviour;
	m_wrap.t = edgeBehaviour;
	m_internalFormat = internalFormat;
	m_target = target;

	//Error is false right now
	GLenum error = 0;

	//Unload any previously loaded items
	Unload();

	//Create the texture handle
	glGenTextures(1, &m_handle);
	//Bind it
	glBindTexture(target, m_handle);
	//Get error
	error = glGetError();

	//Set the parameters
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, edgeBehaviour);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, edgeBehaviour);
	error = glGetError();

	//Generate the texture 
	glTexImage2D(m_target, 0, internalFormat, w, h, 0, textureFormat, dataType, newDataPtr);
	error = glGetError();
	
	//If error isn't false
		//give error message
	if (error != 0)
	{
		std::cout << "Texture Error: Error when creating texture." << std::endl;
	}

	//Unbind texture
	glBindTexture(m_target, GL_NONE);
}

void Texture::Bind() const
{
	//Bind the texture
	glBindTexture(m_target, m_handle);
}

void Texture::Bind(int textureSlot) const
{
	//Set active texture to textureslot
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	//Bind the texture
	Bind();
}

void Texture::Unbind() const
{
	//Unbind the texture
	glBindTexture(m_target, GL_NONE);
}

void Texture::Unbind(int textureSlot) const
{
	//Set the active texture to texture slot
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	//Unbind the texture
	Unbind();
}

GLuint Texture::GetID() const
{
	//Gets the texture ID
	return m_handle;
}

int Texture::GetSizeX() const
{
	//returns the width
	return m_sizeX;
}

int Texture::GetSizeY() const
{
	//returns the height
	return m_sizeY;
}

void Texture::SendTexParameters()
{
	//Bind the texture
	Bind();

	//Resends the texture parameters
	glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_filter.m_minFilter);
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_filter.m_magFilter);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, m_wrap.s);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, m_wrap.t);

	//Unbind the texture
	Unbind();
}

void Texture::SetFilterParameters(GLenum magFilter, GLenum minFilter)
{
	//Set texture filters
	m_filter.m_magFilter = magFilter;
	m_filter.m_minFilter = minFilter;
}

void Texture::SetWrapParameters(GLenum wrap)
{
	//Set the wrap types
	m_wrap.x = wrap;
	m_wrap.y = wrap;
	m_wrap.z = wrap;
}

TextureWrap::TextureWrap()
{
	//Initialize texture wrap to GL_REPEAT on all axes
	x = GL_REPEAT;
	y = GL_REPEAT;
	z = GL_REPEAT;
}
