#ifndef __SEPIABUFFER_H__
#define __SEPIABUFFER_H__

#include "PostEffect.h"

class SepiaEffect : public PostEffect
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
	float GetIntensity() const;

	//Setters
	void SetIntensity(float intensity);

private:
	float m_intensity = 0.8f;
};

#endif // !__SEPIABUFFER_H__
