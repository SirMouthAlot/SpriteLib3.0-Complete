#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
#include <vector>

#include "Texture.h"
#include "Vector.h"

//Texture manager 
class TextureManager abstract
{
public:
	//Deconstructor for texture manager
	//*Runs at program termination
	~TextureManager();

	//Loads the texture
	//*Adds the filename to the list of files loaded
	//*Lods in the texture and pushes it back
	static Texture* LoadTexture(std::string& fileName);
	//Finds a texture
	//*Searches through the list of files loaded based on filename
	static Texture* FindTexture(std::string& fileName);
	//Gets the handle
	//*Finds if the texture was actually loaded
	//*if it was loaded, we find the texture and return the handle
	//*if it isn't loaded, we load the texture and return the handle
	static GLuint GetHandle(std::string& fileName);
	//Gets the texture size
	//*Finds the texture
	//*Returns the size
	static vec2 GetTextureSize(std::string& fileName);

	//Gets the list of loaded files
	static std::vector<std::string> GetFilesLoaded();
private:
	//List of loaded files
	static std::vector<std::string> m_filesLoaded;
	//List of loaded textures
	static std::vector<Texture*> m_textures;
};

#endif // !__TEXTUREMANAGER_H__
