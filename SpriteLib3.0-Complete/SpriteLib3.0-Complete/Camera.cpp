#include "Camera.h"

vec3 activeCameraPosition = vec3();

Camera::Camera(ProjType projection)
{
	//Switch projection
	switch (projection)
	{
		//In the case of perspective, run the perspective function
	case ProjType::Perspective:
		Perspective(60.f, 1.f, 0.01f, 1000.f);
		break;
		//In the case of Orthographic, run the orthographic function
	case ProjType::Orthographic:
		Orthographic(1.f, -100.f, 100.f, -100.f, 100.f, 0.f, 100.f);
		break;
	default:
		break;
	}
}

void Camera::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	//Sets the projection type to Perspective
	m_projectionType = ProjType::Perspective;
	
	//Calculating Orthographic Projection Matrix Manually
	float ys = 1.0f / tanf(ToRadians(fovy) * 0.5f);
	float xs = ys / aspect;
	float nmf = zNear - zFar;
	float B = (zNear + zFar) / nmf;
	float C = (2.0f * zNear * zFar) / nmf;
	m_projection = mat4(
		vec4(xs,  0.f, 0.f, 0.f),
		vec4(0.f, ys,  0.f, 0.f),
		vec4(0.f, 0.f, B,  -1.f),
		vec4(0.f, 0.f, C,   0.f)
	);

	//Set aspect ratio to aspect
	m_aspectRatio = aspect;
	//Set near plane to zNear
	m_near = zNear;
	//Set far plane to zFar
	m_far = zFar;
	//Set y axis of the field of view to fovy
	m_fov.y = fovy;
	//Calculates the x axis of field of view using fovy
	m_fov.x = ToDegrees(2.f * atanf(tanf(ToRadians(m_fov.y) * 0.5f) * aspect));
}

void Camera::Orthographic(float aspectRatio, float left, float right, float bottom, float top, float zNear, float zFar, bool adjustAspect)
{
	//Sets the projection type to Orthographic
	m_projectionType = ProjType::Orthographic;

	float asLeft = left * aspectRatio;
	float asRight = right * aspectRatio;

	if (!adjustAspect)
	{
		//Calculating Orthographic Projection Matrix Manually
		m_projection = mat4(
			vec4( 2.f/(right - left),		0.f,					0.f,					-((right + left) / (right - left))),
			vec4( 0.f,						2.f/(top - bottom),		0.f,					-((top + bottom) / (top - bottom))),
			vec4( 0.f,						0.f,					-2.f/(zFar - zNear),	-((zFar + zNear) / (zFar - zNear))),
			vec4( 0.f,						0.f,					0.f,					1.f)
		);
	}
	else
	{
		//Calculating Orthographic Projection Matrix Manually
		m_projection = mat4(
			vec4(2.f / (asRight - asLeft),	0.f,					0.f,					-((asRight + asLeft) / (asRight - asLeft))),
			vec4(0.f,						2.f / (top - bottom),	0.f,					-((top + bottom)	 / (top - bottom))),
			vec4(0.f,						0.f,					-2.f / (zFar - zNear),	-((zFar + zNear)	 / (zFar - zNear))),
			vec4(0.f,						0.f,					0.f,					1.f)
		);
	}

	//Set ortho size
	m_orthoPos = vec4(left, right, bottom, top);
	//Set near plane to zNear
	m_near = zNear;
	//Set far plane to zFar
	m_far = zFar;
	//Set aspect ratio
	m_aspectRatio = aspectRatio;
}

void Camera::Update()
{
	//Calculates the local to world matrix for this frame
	Transform::Update();
	//The view matrix is the inverse of the model matrix for a camera
	m_viewMatrix = mat4::FastInverse(m_localToWorldMatrix);
}


void Camera::Zoom(float zoom)
{
	m_orthoSize = vec4(m_orthoSize.x + (zoom), m_orthoSize.y - (zoom), m_orthoSize.z + (zoom), m_orthoSize.w - (zoom));

	//Updates Left, Right
	m_orthoPos.x = m_localPosition.x + m_orthoSize.x;
	m_orthoPos.y = m_localPosition.x + m_orthoSize.y;

	//Updates Top, Bottom
	m_orthoPos.z = m_localPosition.y + m_orthoSize.z;
	m_orthoPos.w = m_localPosition.y + m_orthoSize.w;

	Orthographic(m_aspectRatio, m_orthoPos.x, m_orthoPos.y, m_orthoPos.z, m_orthoPos.w, m_near, m_far);
}

float Camera::GetAspect() const
{
	return m_aspectRatio;
}

mat4 Camera::GetView() const
{
	//Returns the view matrix
	return m_viewMatrix;
}

mat4 Camera::GetProjection() const
{
	//Returns the projection matrix
	return m_projection;
}

vec2 Camera::GetWindowSize() const
{
	//Returns the window size
	return vec2(m_windowWidth, m_windowHeight);
}

vec4 Camera::GetOrthoSize() const
{
	//Returns the size of the orthographic matrix (left, right, bottom, top)
	return m_orthoSize;
}

vec4 Camera::GetOrthoPos() const
{
	return m_orthoPos;
}

vec2 Camera::GetFOV() const
{
	//Returns the fov (using the fovy, the fovx was calculated within the perspective function)
	return m_fov;
}

float Camera::GetNear() const
{
	//Returns the near plane value
	return m_near;
}

float Camera::GetFar() const
{
	//Returns the far plane value
	return m_far;
}

ProjType Camera::GetProjectionType() const
{
	//Returns the projection type
	return m_projectionType;
}

void Camera::SetPosition(vec3 newPosition)
{
	//If the camera is an Orthographic camera
	if (m_projectionType == ProjType::Orthographic)
	{
		//Updates Left, Right
		m_orthoPos.x = newPosition.x + (m_orthoSize.x * m_aspectRatio);
		m_orthoPos.y = newPosition.x + (m_orthoSize.y * m_aspectRatio);

		//Updates Top, Bottom
		m_orthoPos.z = newPosition.y + m_orthoSize.z;
		m_orthoPos.w = newPosition.y + m_orthoSize.w;

		//Recreates the Orthographic matrix (EXPENSIVE)
		Orthographic(m_aspectRatio, m_orthoPos.x, m_orthoPos.y, m_orthoPos.z, m_orthoPos.w, m_near, m_far, false);
	}

	//Makes sure to perform the Transform's version of SetPosition too
	Transform::SetPosition(newPosition);
}

void Camera::SetPosition(float x, float y, float z)
{
	SetPosition(vec3(x, y, z));
}

void Camera::SetWindowSize(vec2 window)
{
	//Sets the window size
	m_windowWidth = window.x;
	m_windowHeight = window.y;
}

void Camera::SetOrthoSize(vec4 size)
{
	//Sets the orthographic matrix size 
	//Ordered (left, Right, Bottom, Top)
	m_orthoSize = size;
}

void Camera::SetOrthoPos(vec4 pos)
{
	m_orthoPos = pos;
}

void Camera::SetFOV(vec2 fov)
{
	//Set the fov
	m_fov = fov;
}

void Camera::SetNear(float near)
{
	//Set the near plane
	m_near = near;
}

void Camera::SetFar(float far)
{
	//Set the far plane
	m_far = far;
}

void Camera::SetProjectionType(ProjType type)
{
	//Set the projection type
	m_projectionType = type;
}
