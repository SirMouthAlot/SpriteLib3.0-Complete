#include "VignetteEffect.h"

void VignetteEffect::Init(unsigned width, unsigned height)
{
	//Load the buffers
	int index = int(m_buffers.size());
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);

	//Load the shaders
	int index2 = int(m_shaders.size());
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/post/VignettePost.frag");
}

void VignetteEffect::ApplyEffect(PostEffect * buffer)
{
	BindShader(0);
	m_shaders[0]->SendUniform("uInnerRadius", m_innerRadius);
	m_shaders[0]->SendUniform("uOuterRadius", m_outerRadius);
	m_shaders[0]->SendUniform("uOpacity", m_opacity);

	buffer->BindColorAsTexture(0, 0, 0);

	m_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

float VignetteEffect::GetInnerRadius() const
{
	return m_innerRadius;
}

float VignetteEffect::GetOuterRadius() const
{
	return m_outerRadius;
}

float VignetteEffect::GetOpacity() const
{
	return m_opacity;
}

void VignetteEffect::SetInnerRadius(float innerRadius)
{
	m_innerRadius = innerRadius;
}

void VignetteEffect::SetOuterRadius(float outerRadius)
{
	m_outerRadius = outerRadius;
}

void VignetteEffect::SetOpacity(float opacity)
{
	m_opacity = opacity;
}
