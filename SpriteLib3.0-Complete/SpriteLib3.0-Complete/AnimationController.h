#ifndef __ANIMATIONCONTROLLER_H__
#define __ANIMATIONCONTROLLER_H__

#include <string>
#include <GL/glew.h>
#include "JSON.h"
#include "TextureManager.h"
#include "VertexManager.h"

//Uv coordinates in pixels on the image
//Later gets converted to 0 to 1 range 
//When being sent to the GPU as an attribute
class UVCoordinates
{
public:
	//Create the uv coordinates frame
	//*Either give all corners
	//*Or only Bottom Left and Top Right
	UVCoordinates(vec2 BL, vec2 BR, vec2 TL, vec2 TR);
	UVCoordinates(vec2 BL, vec2 TR);

	//Holds the different corners
	vec2 m_bottomLeft;
	vec2 m_bottomRight;
	vec2 m_topLeft;
	vec2 m_topRight;
};

//Holds the UV coordinate frames for the animations
//along with the time between frames, and serves the purpose of 
//Updating the animation based on that time
class Animation
{
public:
	//Empty Constructor
	Animation() { }
	//Constructs a new animation using a single UVCoordinate frame.
	Animation(UVCoordinates frame, bool repeating = true);

	//Adds a new frame 
	//*Uses UVCoordinate object as the frame
	void AddFrame(UVCoordinates frame);
	//Adds a new frame
	//*Uses 2 vec2s 
	//**Bottom Left
	//**Top Right
	void AddFrame(vec2 BL, vec2 TR);

	//Updates the animation 
	//*the time 
	//*the current frame
	void Update(float dt);

	//Resets all related data
	//*Time data
	//*Frame data
	void Reset();
	
	//Getters
	//Grabs whether or not the "animation finished" flag has been set
	bool GetAnimationDone() const;
	//Gets the current UVCoordinate frame
	UVCoordinates GetCurrentFrame() const;
	//Get the frames within the animation
	std::vector<UVCoordinates> GetFrames() const;
	//Get if the animation repeats
	bool GetRepeating() const;
	//Get the seconds per frame for the animation
	float GetSecPerFrame() const;

	//Setters
	//Set if the animation repeats
	void SetRepeating(bool repeating);
	//Sets the seconds per frame of the animation
	void SetSecPerFrame(float sec);

private:
	//Frames
	std::vector<UVCoordinates> m_frames;
	//Current frame
	int m_currentFrame = 0;
	
	//Is the animation done?
	bool m_done = false;

	//Seconds that need to pass per frame, by default it's 2.f
	//But that's a bit big and you should really set it to something lower
	float m_secPerFrame = 2.f;
	//Initializes frametime to the seconds per frame
	float m_frameTime = m_secPerFrame;
	//Is the animation repeating?
	bool m_repeating = false;
};

//Animated Sprite class that inherits from base class Sprite
class AnimationController
{
public:
	//Empty constructor
	AnimationController() { }
	
	//Init UVs
	void InitUVs(std::string fileName);

	//Updates the animated sprite
	//*Updates the active animation
	//*Updates the UVs using UpdateUVs()
	//*Also updates the sprite size
	//TODO: need to implement an anchor system so sprite size doesn't need to be changed
	void Update(float dt);

	//Adds an animation to our animation list
	//*Just pushes back the animation parameter to the vector
	void AddAnimation(Animation anim);

	//Updates the UVS
	//*Based on the "current frame" of the active animation
	void UpdateUVs();

	//Getters
	//Grabs a reference to the animation at anim
	//*Grabs a reference so you can edit this object
	//and have the changes be reflected in the version within this list
	Animation& GetAnimation(int anim);
	//Grabs just a copy of the actual animation (can't be a ref if it's used for the output to json file type)
	Animation GetAnimation(int anim) const;
	//Grabs a reference to the vector of animations
	std::vector<Animation> GetAnimations() const;
	//Grabs the filename for the texture the animation references
	std::string GetFileName() const;
	//Gets the actual texture's size
	vec2 GetTextureSize() const;
	//Gets the currently active animation
	int GetActiveAnim() const;
	//Gets the UVs VBO so that you can connect it to the VAO for use
	GLuint GetUVVBO() const;
	
	//Setters
	//Sets the filename for the texture the animation references
	void SetFileName(std::string fileName);
	//Sets the texture size
	void SetTextureSize(vec2 size);
	//Sets the active animation
	//*See Link.h for better use of this
	void SetActiveAnim(int anim);
	//Sets the VAO that the UV VBO is connected to
	void SetVAO(GLuint vao);
private:
	//Active animation
	int m_activeAnimation = 0;
	//Holds the animations for the sprite
	std::vector<Animation> m_animations;

	//Filename
	std::string m_fileName;
	//The texture size
	vec2 m_textureSize = vec2();
	//UV VBO
	GLuint m_spritePlaneUVVBO = GL_NONE;
	//VAO
	GLuint m_spritePlaneVAO = GL_NONE;
};

#ifndef __ANIMATIONCONTROLLERJSON_H__
#define __ANIMATIONCONTROLLERJSON_H__

///Animation Class
//Sends animation TO json file
inline void to_json(nlohmann::json& j, const Animation& anim)
{
	//Stores number of frames (needed for input loop
	j["NumFrames"] = anim.GetFrames().size();
	//Stores whether or not the animation repeats
	j["Repeating"] = anim.GetRepeating();
	//Stores how many seconds each frame of the animation will take
	j["SecsPerFrame"] = anim.GetSecPerFrame();

	//Loops through every frame of the animation
	for (unsigned i = 0; i < anim.GetFrames().size(); i++)
	{
		//Grabs the uv coordinates and stores it in an array
		float uvCoord[4] = { anim.GetFrames()[i].m_bottomLeft.x, anim.GetFrames()[i].m_bottomLeft.y,
							anim.GetFrames()[i].m_topRight.x, anim.GetFrames()[i].m_topRight.y };
		//Stores the array under the name Frame#
		j[("Frame" + std::to_string(i)).c_str()] = uvCoord;
	}
}

//Reads animation in FROM json file
inline void from_json(const nlohmann::json& j, Animation& anim)
{
	//Number of frames
	unsigned int numFrames;
	//Grabs the number of frames (used for the loop later)
	j.at("NumFrames").get_to(numFrames);
	//figures out if this animation repeats
	anim.SetRepeating(j.at("Repeating").get<bool>());
	//Figures out how many seconds each frame of this animation will take
	anim.SetSecPerFrame(j.at("SecsPerFrame").get<float>());

	//Loops through every frame
	for (unsigned i = 0; i < numFrames; i++)
	{
		//to store the uv coordinate array
		float uvCoord[4];
		//Finds the array using the name Frame#
		j.at(("Frame" + std::to_string(i)).c_str()).get_to(uvCoord);

		//Add animation frame using the uv coord array
		anim.AddFrame(vec2(uvCoord[0], uvCoord[1]), vec2(uvCoord[2], uvCoord[3]));
	}

}

///Animation Controller
//Sends animation TO json file
inline void to_json(nlohmann::json& j, const AnimationController& animCon)
{
	//Stores number of animations, filename, and the active animation
	j["NumAnimations"] = animCon.GetAnimations().size();
	j["FileName"] = animCon.GetFileName();
	j["Active Animation"] = animCon.GetActiveAnim();

	//Grabs all the animations and stores them in the json
	for (unsigned i = 0; i < animCon.GetAnimations().size(); i++)
	{
		j["Animation" + std::to_string(i)] = animCon.GetAnimation(i);
	}
}

//Reads animation in FROM json file
inline void from_json(const nlohmann::json& j, AnimationController& animCon)
{
	unsigned numAnims = unsigned(j["NumAnimations"]);

	//Set fileName
	animCon.SetFileName(j["FileName"]);
	
	//Add all the animations back into the controller
	for (unsigned i = 0; i < numAnims; i++)
	{
		animCon.AddAnimation(j["Animation" + std::to_string(i)]);
	}

	//Set Active Animation
	animCon.SetActiveAnim(j["Active Animation"]);
}

#endif // !__ANIMATIONCONTROLLERJSON_H__

#endif // !__ANIMATIONCONTROLLER_H__
