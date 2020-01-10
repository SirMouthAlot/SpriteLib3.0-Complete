#include "SelectiveColorEffect.h"

void SelectiveColorEffect::Init(unsigned width, unsigned height)
{
	//Load the buffers
	int index = int(m_buffers.size());
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);

	//Load the shaders
	int index2 = int(m_shaders.size());
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/post/SelectiveColorPost.frag");
}

void SelectiveColorEffect::ApplyEffect(PostEffect * buffer)
{
	BindShader(0);
	m_shaders[0]->SendUniform("uColorRange1", m_colorRange1);
	m_shaders[0]->SendUniform("uColorRange2", m_colorRange2);

	buffer->BindColorAsTexture(0, 0, 0);

	m_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

vec3 SelectiveColorEffect::GetColorRange1() const
{
	return m_colorRange1;
}

vec3 SelectiveColorEffect::GetColorRange2() const
{
	return m_colorRange2;
}

void SelectiveColorEffect::SetColorRange1(vec3 colorRange)
{
	m_colorRange1 = colorRange;
}

void SelectiveColorEffect::SetColorRange2(vec3 colorRange)
{
	m_colorRange2 = colorRange;
}
