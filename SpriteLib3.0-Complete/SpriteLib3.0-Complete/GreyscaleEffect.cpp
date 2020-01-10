#include "GreyscaleEffect.h"

void GreyscaleEffect::Init(unsigned width, unsigned height)
{
	//Load the buffers
	int index = int(m_buffers.size());
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);

	//Load the shaders
	int index2 = int(m_shaders.size());
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/post/GreyscalePost.frag");
}

void GreyscaleEffect::ApplyEffect(PostEffect * buffer)
{
	BindShader(0);
	m_shaders[0]->SendUniform("uIntensity", m_intensity);

	buffer->BindColorAsTexture(0, 0, 0);

	m_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

float GreyscaleEffect::GetIntensity() const
{
	return m_intensity;
}

void GreyscaleEffect::SetIntensity(float intensity)
{
	m_intensity = intensity;
}