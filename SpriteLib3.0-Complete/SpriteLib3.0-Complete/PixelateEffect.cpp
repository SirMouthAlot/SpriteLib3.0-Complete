#include "PixelateEffect.h"

void PixelateEffect::Init(unsigned width, unsigned height)
{
	//Load the buffers
	int index = int(m_buffers.size());
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);

	//Load the shaders
	int index2 = int(m_shaders.size());
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/post/PixelatePost.frag");

	m_windowSize = vec2(float(width), float(height));
}

void PixelateEffect::ApplyEffect(PostEffect * buffer)
{
	BindShader(0);
	m_shaders[0]->SendUniform("uWindowSize", m_windowSize);
	m_shaders[0]->SendUniform("uPixelSize", m_pixelSize);

	buffer->BindColorAsTexture(0, 0, 0);

	m_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

vec2 PixelateEffect::GetWindowSize() const
{
	return m_windowSize;
}

float PixelateEffect::GetPixelSize() const
{
	return m_pixelSize;
}

void PixelateEffect::SetPixelSize(float size)
{
	m_pixelSize = size;
}
