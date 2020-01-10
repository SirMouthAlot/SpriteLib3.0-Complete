#ifndef __SELECTIVECOLOREFFECT_H__
#define __SELECTIVECOLOREFFECT_H__

#include "PostEffect.h"

class SelectiveColorEffect : public PostEffect
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
	vec3 GetColorRange1() const;
	vec3 GetColorRange2() const;

	//Setters
	void SetColorRange1(vec3 colorRange);
	void SetColorRange2(vec3 colorRange);

private:
	vec3 m_colorRange1 = vec3(0.f, 0.f, 0.f);
	vec3 m_colorRange2 = vec3(1.f, 1.f, 1.f);
};


#endif // !__SELECTIVECOLOREFFECT_H__
