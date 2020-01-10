#ifndef __PIXELATEEFFECT_H__
#define __PIXELATEEFFECT_H__

#include "PostEffect.h"

class PixelateEffect : public PostEffect
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
	float GetPixelSize() const;

	//Setters
	void SetPixelSize(float size);

private:
	vec2 m_windowSize;
	float m_pixelSize = 16.f;
};

#endif // !__PIXELATEEFFECT_H__
