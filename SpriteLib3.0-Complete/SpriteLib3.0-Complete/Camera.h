#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Transform.h"
#include "JSON.h"

//Projection Type Enum
enum ProjType
{
	Perspective,
	Orthographic
};

//Move type enum
//*Just indicates what to do with the camera
//*Uses a switch to Zoom in or out, and then pan in different directions
enum MoveType
{
	ZoomOut,
	ZoomIn,
	PanLeft,
	PanRight,
	PanUp,
	PanDown
};

//Camera class that inherits from the base Transform class
class Camera : public Transform
{
public:
	//Camera constructor designating a projection type
	Camera(ProjType projection=ProjType::Orthographic);

	//Creates a perspective projection matrix
	//*It's implemented but shouldn't be used for a 2D game
	void Perspective(
		float fovy, float aspect,
		float zNear, float zFar);

	//Creates an Orthographic projection matrix
	//*is the default projection type used for the framework
	//*Used for 2D games
	void Orthographic(float aspectRatio,
		float left, float right,
		float bottom, float top,
		float zNear, float zFar,
		bool adjustAspect = true);


	//Updates the camera
	//*Updates the transform that the camera inherits from
	//*Gets the fast inverse of the model matrix of the camera
	//stores it as the view matrix
	virtual void Update();

	//Zooms the camera based on an float value
	void Zoom(float zoom);
	
	//Getters
	float GetAspect() const;
	//Gets the view matrix of the camera
	//*The view matrix is the inversed model matrix of the camera
	mat4 GetView() const;
	//Gets the projection matrix of the camera
	//*The projection matrix that we created earlier
	//*It will always be Orthographic in the case of 
	//2D games
	mat4 GetProjection() const;
	//Gets window size
	vec2 GetWindowSize() const;
	//Gets the ortho size
	vec4 GetOrthoSize() const;
	//Gets the ortho position
	vec4 GetOrthoPos() const;
	//Gets the fov
	vec2 GetFOV() const;
	//Get near
	float GetNear()  const;
	//Get far
	float GetFar() const;
	//Get Projection Type
	ProjType GetProjectionType() const;

	//Setters
	//Sets the position of the camera while adjusting for aspect ratio
	//(ALSO UPDATES THE ORTHOGRAPHIC PROJECTION MATRIX IF NEED BE)
	void SetPosition(vec3 newPosition) override;
	void SetPosition(float x, float y, float z) override;
	//Sets window size
	void SetWindowSize(vec2 window);
	//Sets ortho size
	void SetOrthoSize(vec4 size);
	//Sets ortho position
	void SetOrthoPos(vec4 pos);
	//Sets the fov
	void SetFOV(vec2 fov);
	//Set Near
	void SetNear(float near);
	//Set Far
	void SetFar(float far);
	//Set projection Type
	void SetProjectionType(ProjType type);

protected:
	//View matrix
	mat4 m_viewMatrix;
	//Projection matrix
	mat4 m_projection;

	//WindowWidth
	float m_windowWidth = 700.f;
	//WindowHeight
	float m_windowHeight = 700.f;
	
	//Holds the orthographic size
	//*Left, Right, Bottom, Top
	vec4 m_orthoSize = vec4(-100, 100, -100, 100);
	vec4 m_orthoPos = vec4(-100, 100, -100, 100);
	//Holds the FOV
	vec2 m_fov = vec2(60.f, 0.f);
	//Where the near plane is
	//*In the case of Orthographic, this will be negative
	//*In the case of Perspective, this will be as close to zero as you can get it
	float m_near = 0.f;
	//Where the far plane is
	//*In the case of Orthographic, this will be positive, and you might want this to be the negated version of near
	//*In the case of Perspective, this will be positive, something like 1000.f
	float m_far = 100.f;
	
	//Holds the projection type
	//*Will use the enum
	ProjType m_projectionType = ProjType::Orthographic;
	
	//The aspect ratio of the screen
	float m_aspectRatio = 1.f;
};


#ifndef __CAMERAJSON_H__
#define __CAMERAJSON_H__

//Sends animation TO json file
inline void to_json(nlohmann::json& j, const Camera& cam)
{
	//stores projection type
	j["ProjectionType"] = cam.GetProjectionType();

	//Stores the array for box for Ortho
	//Stored Left, Right, Bottom, Top
	j["OrthoSize"] = { cam.GetOrthoSize().x, cam.GetOrthoSize().y, cam.GetOrthoSize().z, cam.GetOrthoSize().w };
	j["OrthoPos"] = { cam.GetOrthoPos().x, cam.GetOrthoPos().y, cam.GetOrthoPos().z, cam.GetOrthoPos().w };

	//Stores near and far planes
	j["NearPlane"] = cam.GetNear();
	j["FarPlane"] = cam.GetFar();

	//Stores the fov y (x is calculated later)
	j["Fov"] = cam.GetFOV().y;

	//Stores the position and rotaiton of the camera
	j["Position"] = { cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z };
	j["Rotation"] = { cam.GetRotationAngleX(), cam.GetRotationAngleY(), cam.GetRotationAngleZ() };
}

//Reads animation in FROM json file
inline void from_json(const nlohmann::json& j, Camera& cam)
{
	//variables needed to set up the camera type
	float zNear = j["NearPlane"];
	float zFar = j["FarPlane"];
	float fovy = j["Fov"];
	vec4 orthoSize = vec4(j["OrthoSize"][0], j["OrthoSize"][1], j["OrthoSize"][2], j["OrthoSize"][3]);
	vec4 orthoPos = vec4(j["OrthoPos"][0], j["OrthoPos"][1], j["OrthoPos"][2], j["OrthoPos"][3]);
	
	//Figures out what type of projection
	switch (int(j["ProjectionType"]))
	{
	case 0:
		//Perspective camera
		cam.Perspective(fovy, (cam.GetWindowSize().x / cam.GetWindowSize().y), zNear, zFar);
		break;
	case 1:
		//Orthographic camera
		cam.Orthographic((cam.GetWindowSize().x / cam.GetWindowSize().y), orthoPos.x, orthoPos.y, orthoPos.z, orthoPos.w, zNear, zFar);
		break;
	}

	//Set position and rotation, scale should be 1 already
	cam.SetPosition(j["Position"][0], j["Position"][1], j["Position"][2]);
	cam.SetRotationAngleX(j["Rotation"][0]);
	cam.SetRotationAngleY(j["Rotation"][1]);
	cam.SetRotationAngleZ(j["Rotation"][2]);
}

#endif // !__CAMERAJSON_H__

#endif // !__CAMERA_H__
