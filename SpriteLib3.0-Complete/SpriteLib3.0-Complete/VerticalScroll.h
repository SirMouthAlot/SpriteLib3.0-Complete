#ifndef __VERTICALSCROLL_H__
#define __VERTICALSCROLL_H__

#include "Camera.h"

class VerticalScroll
{
public:
	VerticalScroll();

	void Update();

	//Getters
	Camera* GetCam() const;
	Transform* GetFocus() const;
	float GetOffset() const;

	//Setters
	void SetCam(Camera* cam);
	void SetFocus(Transform* focus);
	void SetOffset(float offset);
		
private:
	Camera* m_cam = nullptr;
	Transform* m_focus = nullptr;

	float m_offset = 5.f;
};

//Sends vertical scrolling camera TO json file
inline void to_json(nlohmann::json& j, const VerticalScroll& vertScroll)
{
	j["Offset"] = vertScroll.GetOffset();
}

//Sends vertical scrolling camera TO json file
inline void from_json(const nlohmann::json& j, VerticalScroll& vertScroll)
{
	vertScroll.SetOffset(j["Offset"]);
	
}

#endif // !__VERTICALSCROLL_H__
