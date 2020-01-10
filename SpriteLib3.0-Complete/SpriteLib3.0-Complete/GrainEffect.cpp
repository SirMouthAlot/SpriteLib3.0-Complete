#include "GrainEffect.h"
#include "Timer.h"

void GrainEffect::Init(unsigned width, unsigned height)
{
	//Load the buffers
	int index = int(m_buffers.size());
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);

	//Load the shaders
	int index2 = int(m_shaders.size());
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/post/FilmGrainPost.frag");

	m_windowSize = vec2(float(width), float(height));
}

void GrainEffect::ApplyEffect(PostEffect * buffer)
{
	m_time = Timer::time;

	BindShader(0);
	m_shaders[0]->SendUniform("uWindowSize", m_windowSize);
	m_shaders[0]->SendUniform("uStrength", m_strength);
	m_shaders[0]->SendUniform("uTime", m_time);

	buffer->BindColorAsTexture(0, 0, 0);

	m_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

vec2 GrainEffect::GetWindowSize() const
{
	return m_windowSize;
}

float GrainEffect::GetStrength() const
{
	return m_strength;
}

float GrainEffect::GetTime() const
{
	return m_time;
}

void GrainEffect::SetStrength(float strength)
{
	m_strength = strength;
}
