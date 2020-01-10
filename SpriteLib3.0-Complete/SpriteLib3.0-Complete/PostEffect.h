#ifndef __POSTEFFECT_H__
#define __POSTEFFECT_H__

#include <vector>

#include "Framebuffer.h"
#include "Shader.h"

class PostEffect
{
public:
	//Initialize this effect (will be overriden in each derrived class)
	virtual void Init(unsigned width, unsigned height);

	//Applies the effect
	virtual void ApplyEffect(PostEffect* buffer);
	virtual void DrawToScreen();

	//Reshapes the buffer
	virtual void Reshape(unsigned width, unsigned height);

	//Clears the buffers
	void Clear();

	//Unloads all the buffers and deletes the shaders
	void Unload();

	//Bind buffers
	void BindBuffer(int index);
	void UnbindBuffer();

	//Bind textures
	void BindColorAsTexture(int index, int colorBuffer, int textureSlot);
	void BindDepthAsTexture(int index, int textureSlot);
	void UnbindTexture(int textureSlot);

	//Bind shaders
	void BindShader(int index);
	void UnbindShader();

protected:
	//Holds all our buffers for the effects
	std::vector<Framebuffer*> m_buffers;

	//Holds all our shaders for the effects
	std::vector<Shader*> m_shaders;
};

#endif // !__POSTEFFECT_H__
