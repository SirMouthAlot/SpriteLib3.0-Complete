#include "TextureManager.h"

std::vector<std::string> TextureManager:: m_filesLoaded;
std::vector<Texture*> TextureManager::m_textures;

TextureManager::~TextureManager()
{
	//Loops through all textures loaded
	for (unsigned i = 0; i < m_textures.size(); i++)
	{
		//If the texture isn't nullptr
		if (m_textures[i] != nullptr)
		{
			//Delete the texture
			delete m_textures[i];
			//Set it to nullptr
			m_textures[i] = nullptr;
		}
	}
}

Texture* TextureManager::LoadTexture(std::string & fileName)
{
	//Adds the filename to the loaded list, and then loads the texture to the textures list
	m_filesLoaded.push_back(fileName);
	//Push back a new dynamic texture
	m_textures.push_back(new Texture());
	//Loads the new texture
	m_textures[m_textures.size() - 1]->Load(fileName);

	//Returns the handle of the new texture
	return m_textures[m_textures.size() - 1];
}

Texture* TextureManager::FindTexture(std::string & fileName)
{
	//Searches the list of strings and stores the iterator result
	auto iter = std::find(m_filesLoaded.begin(), m_filesLoaded.end(), fileName);
	int index = 0;

	//Checks if the filename was actually found in the loaded files list
	if (iter != m_filesLoaded.end())
		index = int(std::distance(m_filesLoaded.begin(), iter));
	else
		index = -1;

	//If the index isn't equal to -1
		//return the texture
	//Else
		//Load the texture
	if (index != -1)
		return m_textures[index];
	else
		return LoadTexture(fileName);
}

GLuint TextureManager::GetHandle(std::string & fileName)
{
	//Finds the texture and stores it within the variable
	Texture* tex = FindTexture(fileName);

	//Returns the handle of this texture
	return tex->GetID();
}

vec2 TextureManager::GetTextureSize(std::string & fileName)
{
	//Finds the texture and stores it within the variable
	Texture* tex = FindTexture(fileName);

	//Returns a vector and grabs the sizes
	return vec2(float(tex->GetSizeX()), float(tex->GetSizeY()));
}

std::vector<std::string> TextureManager::GetFilesLoaded()
{
	//Grab the list of loaded files
	return m_filesLoaded;
}
