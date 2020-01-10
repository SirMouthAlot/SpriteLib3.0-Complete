#include "BloomEffect.h"

void BloomEffect::Init(unsigned width, unsigned height)
{
	int index = int(m_buffers.size());
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);
	index++;
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(unsigned(width/m_downscale), unsigned(height/m_downscale));
	index++;
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(unsigned(width/m_downscale), unsigned(height/m_downscale));
	index++;
	m_buffers.push_back(new Framebuffer());
	m_buffers[index]->AddColorTarget(GL_RGBA8);
	m_buffers[index]->Init(width, height);

	//check if the shader is initialized
	//Load in the shader
	int index2 = int(m_shaders.size());
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/bloom/PassThrough.frag");
	index2++;
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/bloom/BloomHighPass.frag");
	index2++;
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/bloom/BlurHorizontal.frag");
	index2++;
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/bloom/BlurVertical.frag");
	index2++;
	m_shaders.push_back(new Shader());
	m_shaders[index2]->Load("./assets/shader/PassThrough.vert", "./assets/shader/bloom/BloomComposite.frag");

	//Pixel size
	m_pixelSize = vec2(1.f / width, 1.f / height);
}

void BloomEffect::ApplyEffect(PostEffect * buffer)
{
	//Draws previous buffer to first render target
	BindShader(0);
	
	buffer->BindColorAsTexture(0, 0, 0);

	m_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();


	//Performs high pass on the first render target
	BindShader(1);
	m_shaders[1]->SendUniform("uThreshold", m_threshold);

	BindColorAsTexture(0, 0, 0);

	m_buffers[1]->RenderToFSQ();

	UnbindTexture(0);

	UnbindShader();


	//Computes blur, vertical and horizontal
	for (unsigned i = 0; i < m_passes; i++)
	{
		//Horizontal pass
		BindShader(2);
		m_shaders[2]->SendUniform("uPixelSize", m_pixelSize.x);

		BindColorAsTexture(1, 0, 0);

		m_buffers[2]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();

		//Vertical pass
		BindShader(3);
		m_shaders[3]->SendUniform("uPixelSize", m_pixelSize.y);

		BindColorAsTexture(2, 0, 0);

		m_buffers[1]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();
	}


	//Composite the scene and the bloom
	BindShader(4);

	buffer->BindColorAsTexture(0, 0, 0);
	BindColorAsTexture(1, 0, 1);

	m_buffers[0]->RenderToFSQ();

	UnbindTexture(1);
	UnbindTexture(0);

	UnbindShader();
}

void BloomEffect::Reshape(unsigned width, unsigned height)
{
	m_buffers[0]->Reshape(width, height);
	m_buffers[1]->Reshape(unsigned(width / m_downscale), unsigned(height / m_downscale));
	m_buffers[2]->Reshape(unsigned(width / m_downscale), unsigned(height / m_downscale));
	m_buffers[3]->Reshape(width, height);
}

float BloomEffect::GetDownscale() const
{
	return m_downscale;
}

float BloomEffect::GetThreshold() const
{
	return m_threshold;
}

unsigned BloomEffect::GetPasses() const
{
	return m_passes;
}

void BloomEffect::SetDownscale(float downscale)
{
	m_downscale = downscale;
	Reshape(m_buffers[0]->m_width, m_buffers[0]->m_height);
}

void BloomEffect::SetThreshold(float threshold)
{
	m_threshold = threshold;
}

void BloomEffect::SetPasses(unsigned passes)
{
	m_passes = passes;
}
