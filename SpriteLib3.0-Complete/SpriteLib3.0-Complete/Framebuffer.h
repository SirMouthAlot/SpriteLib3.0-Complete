#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <vector>
#include <GL/glew.h>
#include "Texture.h"
#include "Shader.h"

struct DepthTarget
{
	//Deconstructor for Depth Target
	//*Unloads texture
	~DepthTarget();
	//Deletes the texture of the depth target
	void Unload();
	//Holds the depth texture
	Texture m_texture;
};

struct ColorTarget
{
	//Deconstructor for Color Target
	//*Unloads all the color targets
	~ColorTarget();
	//Deletes the texture of the color targets
	void Unload();
	//Holds the color textures
	std::vector<Texture> m_textures;
	std::vector<GLenum> m_formats;
	std::vector<GLenum> m_buffers;
	//stores the number of color attachments this framebuffer has
	unsigned int m_numAttachments = 0;
};

class Framebuffer
{
public:
	//Deconstructs the Framebuffer
	//*Unloads the framebuffer
	~Framebuffer();
	//Deletes the framebuffer
	void Unload();

	//Initializes the framebuffer with extra steps
	//*Sets size
	//*Initializes the framebuffer afterwards
	virtual void Init(unsigned width, unsigned height);
	//Initializes the framebuffer
	//*creates the framebuffer
	//*creates depth and color attachments based on variables you set
	//*stores the textures within the depth and color variables
	void Init();

	//Adds a depth target
	//*There can only EVER be one depth target
	void AddDepthTarget();
	//Adds a color target
	//*You can have as many color targets as you want
	//*Remember to give formats corresponding to what you need the target to do
	void AddColorTarget(GLenum format);

	//Binds our depth buffer as a texture to a specific texture slot
	void BindDepthAsTexture(int textureSlot) const;
	//Binds our color buffer as a texture to a specific texture slot
	void BindColorAsTexture(unsigned colorBuffer, int textureSlot) const;
	//Unbinds texture from a specific texture slot
	void UnbindTexture(int textureSlot) const;

	//Resizes the framebuffer
	//*Sets the new size
	//*Unloads the current framebuffer
	//*Unloads the depth and color targets
	//*Reinitializes the new framebuffer
	void Reshape(unsigned width, unsigned height);
	//Sets the size of the framebuffer
	//*just sets the width and height variables
	void SetSize(unsigned width, unsigned height);

	//Sets the viewport of the screen to fullscreen (for the size of this Framebuffer)
	void SetViewport() const;
	//Binds the framebuffer
	void Bind() const;
	//Unbinds the framebuffer
	void Unbind() const;
	//Renders the framebuffer to our FullScreenQuad
	void RenderToFSQ() const;
	//Draws the content of the framebuffer to the back buffer (the screen)
	void DrawToBackbuffer();

	//Clears the framebuffer using our clear flag
	void Clear();
	//Checks to make sure the framebuffer is... OK
	bool CheckFBO();

	//Initializes full screen quad
	//*Creates a VAO for full screen quad
	//*Covers -1 to 1 range
	static void InitFullscreenQuad();
	//Draws our full screen quad
	static void DrawFullscreenQuad();


	//Initial width and height is zero
	unsigned int m_width = 0;
	unsigned int m_height = 0;
protected:
	//OpenGL framebuffer handle
	GLuint m_FBO;
	//Depth attachment (either one or none)
	DepthTarget m_depth;
	//Color attachments (can be 1 or above)
	ColorTarget m_color;

	//Default filter is GL_NEAREST
	GLenum m_filter = GL_NEAREST;
	//Default wrap is GL_CLAMP_TO_EDGE
	GLenum m_wrap = GL_CLAMP_TO_EDGE;

	//Clearflag is nothing by default
	GLbitfield m_clearFlag = 0;

	//Is the framebuffer initalized
	bool m_isInit = false;
	//Is the depth attachment active
	bool m_depthActive = false;

	//Full screen quad VBO handle
	static GLuint m_fullScreenQuadVBO;
	//Full screen quad VAO handle
	static GLuint m_fullScreenQuadVAO;

	//The maximum amount of color attachments a framebuffer can hold
	static int m_maxColorAttachments;
	//Is the full screen quad initialized
	static bool m_isInitFSQ;
};

#endif // !__FRAMEBUFFER_H__
