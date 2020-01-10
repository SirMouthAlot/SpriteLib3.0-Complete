#include "Transform.h"

void Transform::SetPosition(vec3 newPosition)
{
	//set local position
	m_localPosition = newPosition;
}

void Transform::SetPosition(float x, float y, float z)
{
	//set local position
	m_localPosition = vec3(x, y, z);
}

void Transform::SetPositionScroll(vec3 newPosition)
{
}

void Transform::SetPositionScroll(float x, float y, float z)
{
}

void Transform::SetPositionX(float x)
{
	m_localPosition.x = x;
}

void Transform::SetPositionY(float y)
{
	m_localPosition.y = y;
}

void Transform::SetPositionZ(float z)
{
	m_localPosition.z = z;
}

void Transform::SetRotationAngleX(Degrees newAngle)
{
	//Set rotation angle around the x axis
	m_rotX = newAngle;
}

void Transform::SetRotationAngleY(Degrees newAngle)
{
	//Set rotation angle around the y axis
	m_rotY = newAngle;
}

void Transform::SetRotationAngleZ(Degrees newAngle)
{
	//Set rotation angle around the z axis
	m_rotZ = newAngle;
}

void Transform::SetScale(vec3 newScale)
{
	//Set the new scale
	m_scale = newScale;
}

void Transform::SetScale(float x, float y, float z)
{
	//Set the new scale
	m_scale = vec3(x, y, z);
}

vec3 Transform::GetPosition() const
{
	//Get the local position
	return m_localPosition;
}

float Transform::GetPositionX() const
{
	return m_localPosition.x;
}

float Transform::GetPositionY() const
{
	return m_localPosition.y;
}

float Transform::GetPositionZ() const
{
	return m_localPosition.z;
}

Degrees Transform::GetRotationAngleX() const
{
	//Get the rotation angle around the x axis
	return m_rotX;
}

Degrees Transform::GetRotationAngleY() const
{
	//Get the rotation angle around the y axis
	return m_rotY;
}

Degrees Transform::GetRotationAngleZ() const
{
	//Get the rotation angle around the z axis
	return m_rotZ;
}

vec3 Transform::GetScale() const
{
	//Get the scale
	return m_scale;
}

mat4 Transform::GetLocalToWorldMatrix()
{
	//Gets the model matrix
	return m_localToWorldMatrix;
}

Degrees Transform::ToDegrees(Radians angle)
{
	//Performs Radians to Degrees conversion
	Degrees temp = angle * (180.f / PI);
	return temp;
}

Radians Transform::ToRadians(Degrees angle)
{
	//Performs Degrees to Radians conversion
	Radians temp = angle * (PI / 180.f);
	return temp;
}

void Transform::Update()
{
	//Manual Rotation Matrix Calculations
	mat4 rx;
	mat4 ry;
	mat4 rz;

	//Creates an X axis rotation matrix
	rx = mat4(
		vec4(1.f, 0.f, 0.f, 0.f),
		vec4(0.f, cosf(m_rotX), -sinf(m_rotX), 0.f),
		vec4(0.f, sinf(m_rotX), cosf(m_rotX), 0.f),
		vec4(0.f, 0.f, 0.f, 1.f)
	);

	//Creates a Y axis rotation matrix
	ry = mat4(
		vec4(cosf(m_rotY), 0.f, sinf(m_rotY), 0.f),
		vec4(0.f, 1.f, 0.f, 0.f),
		vec4(-sinf(m_rotY), 0.f, cosf(m_rotY), 0.f),
		vec4(0.f, 0.f, 0.f, 1.f)
	);

	//Creates a Z axis rotation matrix
	rz = mat4(
		vec4(cosf(m_rotZ), -sinf(m_rotZ), 0.f, 0.f),
		vec4(sinf(m_rotZ), cosf(m_rotZ), 0.f, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(0.f, 0.f, 0.f, 1.f)
	);

	//Multiplies the z and y axis rotations together
	mat4 rzy = mat4(
		vec4(rz[0][0] * ry[0][0] + rz[0][1] * ry[1][0] + rz[0][2] * ry[2][0] + rz[0][3] * ry[3][0], rz[0][0] * ry[0][1] + rz[0][1] * ry[1][1] + rz[0][2] * ry[2][1] + rz[0][3] * ry[3][1], rz[0][0] * ry[0][2] + rz[0][1] * ry[1][2] + rz[0][2] * ry[2][2] + rz[0][3] * ry[3][2], rz[0][0] * ry[0][3] + rz[0][1] * ry[1][3] + rz[0][2] * ry[2][3] + rz[0][3] * ry[3][3]),
		vec4(rz[1][0] * ry[0][0] + rz[1][1] * ry[1][0] + rz[1][2] * ry[2][0] + rz[1][3] * ry[3][0], rz[1][0] * ry[0][1] + rz[1][1] * ry[1][1] + rz[1][2] * ry[2][1] + rz[1][3] * ry[3][1], rz[1][0] * ry[0][2] + rz[1][1] * ry[1][2] + rz[1][2] * ry[2][2] + rz[1][3] * ry[3][2], rz[1][0] * ry[0][3] + rz[1][1] * ry[1][3] + rz[1][2] * ry[2][3] + rz[1][3] * ry[3][3]),
		vec4(rz[2][0] * ry[0][0] + rz[2][1] * ry[1][0] + rz[2][2] * ry[2][0] + rz[2][3] * ry[3][0], rz[2][0] * ry[0][1] + rz[2][1] * ry[1][1] + rz[2][2] * ry[2][1] + rz[2][3] * ry[3][1], rz[2][0] * ry[0][2] + rz[2][1] * ry[1][2] + rz[2][2] * ry[2][2] + rz[2][3] * ry[3][2], rz[2][0] * ry[0][3] + rz[2][1] * ry[1][3] + rz[2][2] * ry[2][3] + rz[2][3] * ry[3][3]),
		vec4(rz[3][0] * ry[0][0] + rz[3][1] * ry[1][0] + rz[3][2] * ry[2][0] + rz[3][3] * ry[3][0], rz[3][0] * ry[0][1] + rz[3][1] * ry[1][1] + rz[3][2] * ry[2][1] + rz[3][3] * ry[3][1], rz[3][0] * ry[0][2] + rz[3][1] * ry[1][2] + rz[3][2] * ry[2][2] + rz[3][3] * ry[3][2], rz[3][0] * ry[0][3] + rz[3][1] * ry[1][3] + rz[3][2] * ry[2][3] + rz[3][3] * ry[3][3])
	);

	//Multiplies the zy rotation matrix by the x axis rotation matrix
	m_localRotation = mat4(
		vec4(rzy[0][0] * rx[0][0] + rzy[0][1] * rx[1][0] + rzy[0][2] * rx[2][0] + rzy[0][3] * rx[3][0], rzy[0][0] * rx[0][1] + rzy[0][1] * rx[1][1] + rzy[0][2] * rx[2][1] + rzy[0][3] * rx[3][1], rzy[0][0] * rx[0][2] + rzy[0][1] * rx[1][2] + rzy[0][2] * rx[2][2] + rzy[0][3] * rx[3][2], rzy[0][0] * rx[0][3] + rzy[0][1] * rx[1][3] + rzy[0][2] * rx[2][3] + rzy[0][3] * rx[3][3]),
		vec4(rzy[1][0] * rx[0][0] + rzy[1][1] * rx[1][0] + rzy[1][2] * rx[2][0] + rzy[1][3] * rx[3][0], rzy[1][0] * rx[0][1] + rzy[1][1] * rx[1][1] + rzy[1][2] * rx[2][1] + rzy[1][3] * rx[3][1], rzy[1][0] * rx[0][2] + rzy[1][1] * rx[1][2] + rzy[1][2] * rx[2][2] + rzy[1][3] * rx[3][2], rzy[1][0] * rx[0][3] + rzy[1][1] * rx[1][3] + rzy[1][2] * rx[2][3] + rzy[1][3] * rx[3][3]),
		vec4(rzy[2][0] * rx[0][0] + rzy[2][1] * rx[1][0] + rzy[2][2] * rx[2][0] + rzy[2][3] * rx[3][0], rzy[2][0] * rx[0][1] + rzy[2][1] * rx[1][1] + rzy[2][2] * rx[2][1] + rzy[2][3] * rx[3][1], rzy[2][0] * rx[0][2] + rzy[2][1] * rx[1][2] + rzy[2][2] * rx[2][2] + rzy[2][3] * rx[3][2], rzy[2][0] * rx[0][3] + rzy[2][1] * rx[1][3] + rzy[2][2] * rx[2][3] + rzy[2][3] * rx[3][3]),
		vec4(rzy[3][0] * rx[0][0] + rzy[3][1] * rx[1][0] + rzy[3][2] * rx[2][0] + rzy[3][3] * rx[3][0], rzy[3][0] * rx[0][1] + rzy[3][1] * rx[1][1] + rzy[3][2] * rx[2][1] + rzy[3][3] * rx[3][1], rzy[3][0] * rx[0][2] + rzy[3][1] * rx[1][2] + rzy[3][2] * rx[2][2] + rzy[3][3] * rx[3][2], rzy[3][0] * rx[0][3] + rzy[3][1] * rx[1][3] + rzy[3][2] * rx[2][3] + rzy[3][3] * rx[3][3])
	);

	//Manual Translation Matrix
	mat4 tran;

	//Creates the translation matrix
	tran = mat4(
		vec4(1.f, 0.f, 0.f, m_localPosition.x),
		vec4(0.f, 1.f, 0.f, m_localPosition.y),
		vec4(0.f, 0.f, 1.f, m_localPosition.z),
		vec4(0.f, 0.f, 0.f, 1.f				 )
	);

	//Manual Scale Matrix
	mat4 scale;

	//Creates the scale matrix
	scale = mat4(
		vec4(m_scale.x, 0.f,     0.f,     0.f),
		vec4(0.f,     m_scale.y, 0.f,     0.f),
		vec4(0.f,     0.f,     m_scale.z, 0.f),
		vec4(0.f,     0.f,     0.f,     1.f)
	);

	//Manual Homogeneous Transformation Matrix
	//Multiplies the translation matrix by the rotation matrix
	mat4 tranRot = mat4(
		vec4(tran[0][0] * m_localRotation[0][0] + tran[0][1] * m_localRotation[1][0] + tran[0][2] * m_localRotation[2][0] + tran[0][3] * m_localRotation[3][0], tran[0][0] * m_localRotation[0][1] + tran[0][1] * m_localRotation[1][1] + tran[0][2] * m_localRotation[2][1] + tran[0][3] * m_localRotation[3][1], tran[0][0] * m_localRotation[0][2] + tran[0][1] * m_localRotation[1][2] + tran[0][2] * m_localRotation[2][2] + tran[0][3] * m_localRotation[3][2], tran[0][0] * m_localRotation[0][3] + tran[0][1] * m_localRotation[1][3] + tran[0][2] * m_localRotation[2][3] + tran[0][3] * m_localRotation[3][3]),
		vec4(tran[1][0] * m_localRotation[0][0] + tran[1][1] * m_localRotation[1][0] + tran[1][2] * m_localRotation[2][0] + tran[1][3] * m_localRotation[3][0], tran[1][0] * m_localRotation[0][1] + tran[1][1] * m_localRotation[1][1] + tran[1][2] * m_localRotation[2][1] + tran[1][3] * m_localRotation[3][1], tran[1][0] * m_localRotation[0][2] + tran[1][1] * m_localRotation[1][2] + tran[1][2] * m_localRotation[2][2] + tran[1][3] * m_localRotation[3][2], tran[1][0] * m_localRotation[0][3] + tran[1][1] * m_localRotation[1][3] + tran[1][2] * m_localRotation[2][3] + tran[1][3] * m_localRotation[3][3]),
		vec4(tran[2][0] * m_localRotation[0][0] + tran[2][1] * m_localRotation[1][0] + tran[2][2] * m_localRotation[2][0] + tran[2][3] * m_localRotation[3][0], tran[2][0] * m_localRotation[0][1] + tran[2][1] * m_localRotation[1][1] + tran[2][2] * m_localRotation[2][1] + tran[2][3] * m_localRotation[3][1], tran[2][0] * m_localRotation[0][2] + tran[2][1] * m_localRotation[1][2] + tran[2][2] * m_localRotation[2][2] + tran[2][3] * m_localRotation[3][2], tran[2][0] * m_localRotation[0][3] + tran[2][1] * m_localRotation[1][3] + tran[2][2] * m_localRotation[2][3] + tran[2][3] * m_localRotation[3][3]),
		vec4(tran[3][0] * m_localRotation[0][0] + tran[3][1] * m_localRotation[1][0] + tran[3][2] * m_localRotation[2][0] + tran[3][3] * m_localRotation[3][0], tran[3][0] * m_localRotation[0][1] + tran[3][1] * m_localRotation[1][1] + tran[3][2] * m_localRotation[2][1] + tran[3][3] * m_localRotation[3][1], tran[3][0] * m_localRotation[0][2] + tran[3][1] * m_localRotation[1][2] + tran[3][2] * m_localRotation[2][2] + tran[3][3] * m_localRotation[3][2], tran[3][0] * m_localRotation[0][3] + tran[3][1] * m_localRotation[1][3] + tran[3][2] * m_localRotation[2][3] + tran[3][3] * m_localRotation[3][3])
	);

	//Multiplies the transRot matrix by the scale matrix
	m_localToWorldMatrix = mat4(
		vec4(tranRot[0][0] * scale[0][0] + tranRot[0][1] * scale[1][0] + tranRot[0][2] * scale[2][0] + tranRot[0][3] * scale[3][0], tranRot[0][0] * scale[0][1] + tranRot[0][1] * scale[1][1] + tranRot[0][2] * scale[2][1] + tranRot[0][3] * scale[3][1], tranRot[0][0] * scale[0][2] + tranRot[0][1] * scale[1][2] + tranRot[0][2] * scale[2][2] + tranRot[0][3] * scale[3][2], tranRot[0][0] * scale[0][3] + tranRot[0][1] * scale[1][3] + tranRot[0][2] * scale[2][3] + tranRot[0][3] * scale[3][3]),
		vec4(tranRot[1][0] * scale[0][0] + tranRot[1][1] * scale[1][0] + tranRot[1][2] * scale[2][0] + tranRot[1][3] * scale[3][0], tranRot[1][0] * scale[0][1] + tranRot[1][1] * scale[1][1] + tranRot[1][2] * scale[2][1] + tranRot[1][3] * scale[3][1], tranRot[1][0] * scale[0][2] + tranRot[1][1] * scale[1][2] + tranRot[1][2] * scale[2][2] + tranRot[1][3] * scale[3][2], tranRot[1][0] * scale[0][3] + tranRot[1][1] * scale[1][3] + tranRot[1][2] * scale[2][3] + tranRot[1][3] * scale[3][3]),
		vec4(tranRot[2][0] * scale[0][0] + tranRot[2][1] * scale[1][0] + tranRot[2][2] * scale[2][0] + tranRot[2][3] * scale[3][0], tranRot[2][0] * scale[0][1] + tranRot[2][1] * scale[1][1] + tranRot[2][2] * scale[2][1] + tranRot[2][3] * scale[3][1], tranRot[2][0] * scale[0][2] + tranRot[2][1] * scale[1][2] + tranRot[2][2] * scale[2][2] + tranRot[2][3] * scale[3][2], tranRot[2][0] * scale[0][3] + tranRot[2][1] * scale[1][3] + tranRot[2][2] * scale[2][3] + tranRot[2][3] * scale[3][3]),
		vec4(tranRot[3][0] * scale[0][0] + tranRot[3][1] * scale[1][0] + tranRot[3][2] * scale[2][0] + tranRot[3][3] * scale[3][0], tranRot[3][0] * scale[0][1] + tranRot[3][1] * scale[1][1] + tranRot[3][2] * scale[2][1] + tranRot[3][3] * scale[3][1], tranRot[3][0] * scale[0][2] + tranRot[3][1] * scale[1][2] + tranRot[3][2] * scale[2][2] + tranRot[3][3] * scale[3][2], tranRot[3][0] * scale[0][3] + tranRot[3][1] * scale[1][3] + tranRot[3][2] * scale[2][3] + tranRot[3][3] * scale[3][3])
	);
}
