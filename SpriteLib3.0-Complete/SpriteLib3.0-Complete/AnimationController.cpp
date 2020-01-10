#include "AnimationController.h"

////////////UVCOORDINATES STUFF STARTS HERE//////////////

UVCoordinates::UVCoordinates(vec2 BL, vec2 BR, vec2 TL, vec2 TR) :
	m_bottomLeft(BL), m_bottomRight(BR), m_topLeft(TL), m_topRight(TR)
{
	//Just initializes all the members
}

UVCoordinates::UVCoordinates(vec2 BL, vec2 TR)
{
	//Calculates the other 2 coordinates using just the two
	m_bottomLeft = BL;
	m_bottomRight = vec2(TR.x, BL.y);
	m_topLeft = vec2(BL.x, TR.y);
	m_topRight = TR;
}

//////////ANIMATION STUFF STARTS HERE/////////////

Animation::Animation(UVCoordinates frame, bool repeating)
{
	//Adds the initial frame
	AddFrame(frame);
	//Sets repeating to the passed parameter
	m_repeating = repeating;
}

void Animation::AddFrame(UVCoordinates frame)
{
	//Pushes back frame to m_frames
	m_frames.push_back(frame);
}

void Animation::AddFrame(vec2 BL, vec2 TR)
{
	//Pushes back a new UVCoordinate frame object created using the vectors
	m_frames.push_back(UVCoordinates(BL, TR));
}

void Animation::Update(float dt)
{
	//Subtract delta time from
	//Frame time
	m_frameTime -= dt;
	
	//If frametime is less than or equal to zero
		//Increase the frame and reset frametime
	if (m_frameTime <= 0.f)
	{
		m_currentFrame++;
		m_frameTime = m_secPerFrame;
		//If animation repeating
			//Check for reset
		if (m_repeating)
		{
			//If current frame is greater than or equal to total num frames
				//Reset current frame to zero
			if (m_currentFrame >= m_frames.size())
			{
				m_currentFrame = 0;
			}
		}
		else
		{
			//If current frame is greater than or equal to total num frames
				//Reset to last frame
			if (m_currentFrame >= m_frames.size())
			{
				m_currentFrame = int(m_frames.size() - 1);
				m_done = true;
			}
		}
	}
}

void Animation::Reset()
{
	//Resets the frametime and sets current frame to zero
	m_frameTime = m_secPerFrame;
	m_currentFrame = 0;
	m_done = false;
}

bool Animation::GetAnimationDone() const
{
	//Returns the m_done bool
	return m_done;
}

UVCoordinates Animation::GetCurrentFrame() const
{
	//Grabs the current frame from our list of frames
	return m_frames[m_currentFrame];
}

std::vector<UVCoordinates> Animation::GetFrames() const
{
	return m_frames;
}

bool Animation::GetRepeating() const
{
	return m_repeating;
}

float Animation::GetSecPerFrame() const
{
	return m_secPerFrame;
}

void Animation::SetRepeating(bool repeating)
{
	m_repeating = repeating;
}

void Animation::SetSecPerFrame(float sec)
{
	//Sets m_secsPerFrame to sec
	m_secPerFrame = sec;
	//Resets the animation to accomodate this new timing
	Reset();
}

///////ANIMATION CONTROLLER STUFF STARTS/////////

void AnimationController::InitUVs(std::string fileName)
{
	//VBO data set
	float VBO_DATA[]{
		0.f, 0.f,
		1.f, 0.f,
		0.f, 1.f,

		1.f, 1.f,
		0.f, 1.f,
		1.f, 0.f,
	};
	int texCoordSize = 6 * 2 * sizeof(float);
	
	//Creates a new VBO
	m_spritePlaneUVVBO = VertexManager::CreateVBO();

	//Sets filename
	m_fileName = fileName;
	//Grabs texture size
	m_textureSize = TextureManager::GetTextureSize(fileName);

	//Bind UV VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_spritePlaneUVVBO);
	//Uses dynamic draw so you can rebuffer the data and change the UVs
	glBufferData(GL_ARRAY_BUFFER, texCoordSize, VBO_DATA, GL_DYNAMIC_DRAW);
}

void AnimationController::Update(float dt)
{
	//Updates the active animation
	m_animations[m_activeAnimation].Update(dt);
	
	//TODO: Add some sort of custom anchor system
	//You would define the point on the object that would be defined as an anchor


	//Updates the UVs based on the current frame of animation
	UpdateUVs();
}

void AnimationController::AddAnimation(Animation anim)
{
	//Pushes back the new animation to m_animations
	m_animations.push_back(anim);
}

void AnimationController::UpdateUVs()
{
	//grabs the uv data from the current frame
	auto uv = m_animations[m_activeAnimation].GetCurrentFrame();

	//Converts the uvs to 0 - 1
	vec2 botL = vec2(uv.m_bottomLeft.x / m_textureSize.x, uv.m_bottomLeft.y / m_textureSize.y);
	vec2 botR = vec2(uv.m_bottomRight.x / m_textureSize.x, uv.m_bottomRight.y / m_textureSize.y);
	vec2 topL = vec2(uv.m_topLeft.x / m_textureSize.x, uv.m_topLeft.y / m_textureSize.y);
	vec2 topR = vec2(uv.m_topRight.x / m_textureSize.x, uv.m_topRight.y / m_textureSize.y);

	//Updated UV data
	float VBO_DATA[]{
		botL.x, botL.y,
		botR.x, botR.y,
		topL.x, topL.y,

		topR.x, topR.y,
		topL.x, topL.y,
		botR.x, botR.y,
	};
	int texCoordSize = 6 * 2 * sizeof(float);
	
	//Binds vertex array object and UV buffer
	glBindVertexArray(m_spritePlaneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_spritePlaneUVVBO);
	
	//We just buffer subdata now
	glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordSize, VBO_DATA);
}

Animation& AnimationController::GetAnimation(int anim)
{
	//Grabs the animation at anim
	return m_animations[anim];
}

Animation AnimationController::GetAnimation(int anim) const
{
	return m_animations[anim];
}

std::vector<Animation> AnimationController::GetAnimations() const
{
	return m_animations;
}

std::string AnimationController::GetFileName() const
{
	return m_fileName;
}

vec2 AnimationController::GetTextureSize() const
{
	return m_textureSize;
}

int AnimationController::GetActiveAnim() const
{
	return m_activeAnimation;
}

GLuint AnimationController::GetUVVBO() const
{
	return m_spritePlaneUVVBO;
}

void AnimationController::SetFileName(std::string fileName)
{
	m_fileName = fileName;
}

void AnimationController::SetTextureSize(vec2 size)
{
	m_textureSize = size;
}

void AnimationController::SetVAO(GLuint vao)
{
	m_spritePlaneVAO = vao;
}

void AnimationController::SetActiveAnim(int anim)
{
	//Sets the active animation to anim
	m_activeAnimation = anim;
}