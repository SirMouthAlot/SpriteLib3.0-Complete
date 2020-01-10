#ifndef __VIGNETTEEFFECT_H__
#define __VIGNETTEEFFECT_H__

#include "PostEffect.h"

class VignetteEffect : public PostEffect
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
	float GetInnerRadius() const;
	float GetOuterRadius() const;
	float GetOpacity() const;

	//Setters
	void SetInnerRadius(float innerRadius);
	void SetOuterRadius(float outerRadius);
	void SetOpacity(float opacity);

private:
	float m_innerRadius = 0.2f;
	float m_outerRadius = 1.0f;
	float m_opacity = 1.0f;
};

#endif