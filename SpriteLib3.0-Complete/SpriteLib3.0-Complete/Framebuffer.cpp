#include "Framebuffer.h"

GLuint Framebuffer::m_fullScreenQuadVBO = 0;
GLuint Framebuffer::m_fullScreenQuadVAO = 0;

int Framebuffer::m_maxColorAttachments = 0;
bool Framebuffer::m_isInitFSQ = false;


DepthTarget::~DepthTarget()
{
	//Unloads the depth target
	Unload();
}

void DepthTarget::Unload()
{
	//Deletes the texture at the specific handle
	glDeleteTextures(1, &m_texture.m_handle);
}

ColorTarget::~ColorTarget()
{
	//Unloads the color target
	Unload();
}

void ColorTarget::Unload()
{
	//Deletes all the textures within the list
	glDeleteTextures(m_numAttachments, &m_textures[0].m_handle);
}

Framebuffer::~Framebuffer()
{
}

void Framebuffer::Unload()
{
	//Deletes the framebuffer
	glDeleteFramebuffers(1, &m_FBO);
	//Sets "is initialized" to false
	m_isInit = false;
}

void Framebuffer::Init(unsigned width, unsigned height)
{
	//Sets the size to width and height
	SetSize(width, height);
	//Initializes the framebuffer
	Init();
}

void Framebuffer::Init()
{
	//Generates the FBO
	glGenFramebuffers(1, &m_FBO);
	//Binds the created framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	//If depth is active
		//We create the depth target
	if (m_depthActive)
	{
		//because we have depth, we include depth buffer bit into the clearflag
		m_clearFlag |= GL_DEPTH_BUFFER_BIT;
		
		//Create depth texture
		m_depth.m_texture.m_target = GL_TEXTURE_2D;

		//Generates the texture
		glGenTextures(1, &m_depth.m_texture.m_handle);
		//Binds the texture
		glBindTexture(GL_TEXTURE_2D, m_depth.m_texture.m_handle);
		//Sets the texture storage
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, m_width, m_height);

		//Set texture parameters
		glTextureParameteri(m_depth.m_texture.m_handle, GL_TEXTURE_MIN_FILTER, m_filter);
		glTextureParameteri(m_depth.m_texture.m_handle, GL_TEXTURE_MAG_FILTER, m_filter);
		glTextureParameteri(m_depth.m_texture.m_handle, GL_TEXTURE_WRAP_S, m_wrap);
		glTextureParameteri(m_depth.m_texture.m_handle, GL_TEXTURE_WRAP_T, m_wrap);

		//Bind texture to the FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth.m_texture.m_handle, 0);
	}

	//if there is more than zero color attachments
		//We create the color attachments
	if (m_color.m_numAttachments)
	{
		//Because we have a color target, we include color buffer bit into the clear flag
		m_clearFlag |= GL_COLOR_BUFFER_BIT;
		//Creates the things to holds the new texture handles
		GLuint* textureHandles = new GLuint[m_color.m_numAttachments];

		//Generate the texture handles within the texture handles
		glGenTextures(m_color.m_numAttachments, textureHandles);
		//Loops through all the attachments
		for (unsigned i = 0; i < m_color.m_numAttachments; i++)
		{
			//Stores the texture handles within the color target's textures
			m_color.m_textures[i].m_handle = textureHandles[i];
			m_color.m_textures[i].m_target = GL_TEXTURE_2D;
			//Binds the texture
			glBindTexture(GL_TEXTURE_2D, m_color.m_textures[i].m_handle);
			//Sets the texture storage
			glTexStorage2D(GL_TEXTURE_2D, 1, m_color.m_formats[i], m_width, m_height);

			//Set texture parameters
			glTextureParameteri(m_color.m_textures[i].m_handle, GL_TEXTURE_MIN_FILTER, m_filter);
			glTextureParameteri(m_color.m_textures[i].m_handle, GL_TEXTURE_MAG_FILTER, m_filter);
			glTextureParameteri(m_color.m_textures[i].m_handle, GL_TEXTURE_WRAP_S, m_wrap);
			glTextureParameteri(m_color.m_textures[i].m_handle, GL_TEXTURE_WRAP_T, m_wrap);

			//Bind texture to the FBO
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_color.m_textures[i].m_handle, 0);
		}
		//Clears out the texture handles you created
		delete textureHandles;
	}

	CheckFBO();
	//Unbinds the framebuffer
	Unbind();
	//tells the program the framebuffer has been initialized
	m_isInit = true;
}

void Framebuffer::AddDepthTarget()
{
	//if there IS a handle within depth target
		//Unload it
	if (m_depth.m_texture.m_handle)
	{
		m_depth.Unload();
	}
	//Make the depth active true
	m_depthActive = true;
}

void Framebuffer::AddColorTarget(GLenum format)
{
	//Resize the textures to number of attachments + 1
	m_color.m_textures.resize(m_color.m_numAttachments + 1);
	//Add the format parameter to m_formats
	m_color.m_formats.push_back(format);
	//Add the colorattachment buffer number to the Color Target 
	m_color.m_buffers.push_back(GL_COLOR_ATTACHMENT0 + m_color.m_numAttachments);
	//Increments number of attachments
	m_color.m_numAttachments++;
}

void Framebuffer::BindColorAsTexture(unsigned colorBuffer, int textureSlot) const
{
	//Binds the texture at colorbuffer to textureslot
	m_color.m_textures[colorBuffer].Bind(textureSlot);
}

void Framebuffer::BindDepthAsTexture(int textureSlot) const
{
	//Binds the depth texture to textureslot
	m_depth.m_texture.Bind(textureSlot);
}

void Framebuffer::UnbindTexture(int textureSlot) const
{
	//Binds texture to GL_NONE
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Framebuffer::Reshape(unsigned width, unsigned height)
{
	//Sets the size
	SetSize(width, height);
	//Unloads the framebuffer
	Unload();
	//Unloads the depth target
	m_depth.Unload();
	//Unloads the color target
	m_color.Unload();
	//Initializes the framebuffer
	Init();
}

void Framebuffer::SetSize(unsigned width, unsigned height)
{
	//Sets the width and height
	m_width = width;
	m_height = height;
}

void Framebuffer::SetViewport() const
{
	//Sets the viewport
	glViewport(0, 0, m_width, m_height);
}

void Framebuffer::Bind() const
{
	//Binds the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	//Sets up these buffers to be drawn to
	if (m_color.m_numAttachments)
	{
		glDrawBuffers(m_color.m_numAttachments, &m_color.m_buffers[0]);
	}
}

void Framebuffer::Unbind() const
{
	//Unbinds the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Framebuffer::RenderToFSQ() const
{
	//Sets viewport
	SetViewport();
	//Binds the framebuffer
	Bind();
	//Draws the full screen quad
	DrawFullscreenQuad();
	//Unbinds the framebuffer
	Unbind();
}

void Framebuffer::DrawToBackbuffer()
{
	//Binds the read and draw framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	//Blits the framebuffer to the back buffer
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);
}

void Framebuffer::Clear()
{
	//Clears the framebuffer based on the clear flag
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(m_clearFlag);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

bool Framebuffer::CheckFBO()
{
	//Binds the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	//Checks framebuffer status to make sure it's complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer broke yo");
		return false;
	}
	return true;
}

void Framebuffer::InitFullscreenQuad()
{
	//A vbo with UVs and Verts going 
	//-1 to 1 for verts
	//0 to 1 for UVs
	float VBO_DATA[]{
		-1.f, -1.f, 0.f,
		1.f, -1.f, 0.f,
		-1.f, 1.f, 0.f,

		1.f, 1.f, 0.f,
		-1.f, 1.f, 0.f,
		1.f, -1.f, 0.f,

		0.f, 0.f,
		1.f, 0.f,
		0.f, 1.f,

		1.f, 1.f,
		0.f, 1.f,
		1.f, 0.f,
	};
	//Vertex size is 6 points * 3 data points * sizeof(1 float)
	int vertexSize = 6 * 3 * sizeof(float);
	//texCoordSize is 6 points * 2 data points * sizeof(1 float)
	int texCoordSize = 6 * 2 * sizeof(float);

	//generates the vertex array
	//TODO: remove later and replace with VertexManager stuff
	glGenVertexArrays(1, &m_fullScreenQuadVAO);
	//Binds VAO
	glBindVertexArray(m_fullScreenQuadVAO);

	//Enables 2 vertex attrib array slots
	glEnableVertexAttribArray(0); //vertices
	glEnableVertexAttribArray(1); //UV coordinates

	//Generates a single VBO
	glGenBuffers(1, &m_fullScreenQuadVBO);

	//Binds the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_fullScreenQuadVBO);
	//Buffers all of the VBO_data within the one VBO
	glBufferData(GL_ARRAY_BUFFER, vertexSize + texCoordSize, VBO_DATA, GL_STATIC_DRAW);

	//TODO: For some reason on lab computer it isn't liking the "reinterpret_cast<void*>(vertexSize)"
	//Working fine on my own laptop though
#pragma warning(push)
#pragma warning(disable : 4312)
	//Sets the first attrib array to point to the beginning of the data
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));			//send vertex attributes
	//Sets the second attrib array to point to an offset within the data equal to vertexSize
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(vertexSize)); //send uv attributes
#pragma warning(pop)

	//Unbinds everything
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void Framebuffer::DrawFullscreenQuad()
{
	//Draws the fullscreen quad
	glBindVertexArray(m_fullScreenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}
