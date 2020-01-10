#ifndef __NOISEEFFECT_H__
#define __NOISEEFFECT_H__

#include "PostEffect.h"

class GrainEffect : public PostEffect
{
public:
	//Initializes the framebuffer with extra steps
	//*Sets size
	//*Initializes the framebuffer afterwards
	void Init(unsigned width, unsigned height) override;

	//Applies the affect to this screen
	//*Passes the framebuffer with the texture to apply as a parameter
	void ApplyEffect(PostEffect* buffer) override;

	//Getters
	vec2 GetWindowSize() const;
	float GetStrength() const;
	float GetTime() const;

	//Setters
	void SetStrength(float strength);
private:
	vec2 m_windowSize;
	float m_strength = 32.f;
	float m_time;
};

#endif // !__NOISEEFFECT_H__
