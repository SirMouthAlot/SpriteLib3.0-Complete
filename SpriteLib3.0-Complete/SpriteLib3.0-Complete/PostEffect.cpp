#include "PostEffect.h"

void PostEffect::Init(unsigned width, unsigned height)
{
	int index = int(m_buffers.size());
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);

	int index2 = int(m_shaders.size());
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/PassThrough.frag");
}

void PostEffect::ApplyEffect(PostEffect * buffer)
{
	BindShader(0);

	buffer->BindColorAsTexture(0, 0, 0);

	m_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

void PostEffect::DrawToScreen()
{
	BindShader(0);

	BindColorAsTexture(0, 0, 0);

	m_buffers[0]->DrawFullscreenQuad();

	UnbindTexture(0);

	UnbindShader();
}

void PostEffect::Reshape(unsigned width, unsigned height)
{
	for (unsigned int i = 0; i < m_buffers.size(); i++)
	{
		m_buffers[i]->Reshape(width, height);
	}
}

void PostEffect::Clear()
{
	for (int i = 0; i < m_buffers.size(); i++)
	{
		m_buffers[i]->Clear();
	}
}

void PostEffect::Unload()
{
	for (unsigned i = 0; i < m_buffers.size(); i++)
	{
		if (m_buffers[i] != nullptr)
		{
			m_buffers[i]->Unload();
			delete m_buffers[i];
			m_buffers[i] = nullptr;
		}
	}

	for (unsigned i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i] != nullptr)
		{
			delete m_shaders[i];
			m_shaders[i] = nullptr;
		}
	}
}

void PostEffect::BindBuffer(int index)
{
	m_buffers[index]->Bind();
}

void PostEffect::UnbindBuffer()
{
	//Unbinds the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void PostEffect::BindColorAsTexture(int index, int colorBuffer, int textureSlot)
{
	m_buffers[index]->BindColorAsTexture(colorBuffer, textureSlot);
}

void PostEffect::BindDepthAsTexture(int index, int textureSlot)
{
	m_buffers[index]->BindDepthAsTexture(textureSlot);
}

void PostEffect::UnbindTexture(int textureSlot)
{
	//Binds texture to GL_NONE
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void PostEffect::BindShader(int index)
{
	m_shaders[index]->Bind();
}

void PostEffect::UnbindShader()
{
	//Unbind program
	glUseProgram(GL_NONE);
}
