#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include <math.h>

#pragma comment(lib, "Matrix.lib")

#include "Matrix.h"
#include "JSON.h"

typedef float Degrees;
typedef float Radians;
#define PI 3.14159265358979323846f

class Transform
{
public:
	//Empty Constructor
	Transform() { }

	//Setters
	virtual void SetPosition(vec3 newPosition);
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPositionScroll(vec3 newPosition);
	virtual void SetPositionScroll(float x, float y, float z);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);
	void SetRotationAngleX(Degrees newAngle);
	void SetRotationAngleY(Degrees newAngle);
	void SetRotationAngleZ(Degrees newAngle);
	void SetScale(vec3 newScale);
	void SetScale(float x, float y, float z);

	//Getters
	vec3 GetPosition() const;
	float GetPositionX() const;
	float GetPositionY() const;
	float GetPositionZ() const;
	Degrees GetRotationAngleX() const;
	Degrees GetRotationAngleY() const;
	Degrees GetRotationAngleZ() const;
	vec3 GetScale() const;
	mat4 GetLocalToWorldMatrix();
	
	//Simple conversion functions (written in private so they can't be accessed elsewhere)
	static Degrees ToDegrees(Radians angle);
	static Radians ToRadians(Degrees angle);

	//Updates the transform
	//*Recalculates the model matrix
	//*Creates new Rotation matrix
	//*Creates new Translation matrix
	//*Creates new Scale matrix
	//*Multiplies them all together
	void Update();

	//Translation
	vec3 m_localPosition = vec3(0.f, 0.f, 0.f);

	//Rotation values
	Degrees m_rotX = 0.f;
	Degrees m_rotY = 0.f;
	Degrees m_rotZ = 0.f;
	mat4 m_localRotation;

	//Scale
	vec3 m_scale = vec3(1.f, 1.f, 1.f);

	//Model Matrix
	mat4 m_localToWorldMatrix;
private:

};

#ifndef __TRANSFORMJSON_H__
#define __TRANSFORMJSON_H__

//Sends transform TO json file
inline void to_json(nlohmann::json& j, const Transform& trans)
{
	//Save position
	j["Position"] = { trans.GetPosition().x, trans.GetPosition().y, trans.GetPosition().z };
	//Save rotation
	j["Rotation"] = { trans.GetRotationAngleX(), trans.GetRotationAngleY(), trans.GetRotationAngleZ() };
	//Save scale
	j["Scale"] = { trans.GetScale().x, trans.GetScale().y, trans.GetScale().z };
}

//Reads transform in FROM json file
inline void from_json(const nlohmann::json& j, Transform& trans)
{
	//Set Position
	trans.SetPosition(vec3(j["Position"][0], j["Position"][1], j["Position"][2]));
	//Set Rotation
	trans.SetRotationAngleX(j["Rotation"][0]);
	trans.SetRotationAngleY(j["Rotation"][1]);
	trans.SetRotationAngleZ(j["Rotation"][2]);
	//Set Scale
	trans.SetScale(vec3(j["Scale"][0], j["Scale"][1], j["Scale"][2]));
}

#endif // !__TRANSFORMJSON_H__


#endif // !__TRANSFORM_H__

