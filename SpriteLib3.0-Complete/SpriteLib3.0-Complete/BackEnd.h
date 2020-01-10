#ifndef __BACKEND_H__
#define __BACKEND_H__

#include <entt/entity/registry.hpp>

#include "Window.h"
#include "Timer.h"
#include "Input.h"
#include "SceneEditor.h"
#include "RenderingSystem.h"
#include "AnimationSystem.h"
#include "PhysicsSystem.h"

//Mouse Motion event
typedef SDL_MouseMotionEvent MotionEvent;
//Mouse Button event
typedef SDL_MouseButtonEvent ClickEvent;
//Mouse Wheel event
typedef SDL_MouseWheelEvent WheelEvent;

//Abstract class
//Everything is accessed through statics
class BackEnd abstract
{
public:
	//Initializes the backend
	//*Inits GLEW and SDL
	//*Along with rendering system and 
	static void InitBackEnd(float windowWidth, float windowHeight);
	//Initializes GLEW
	//*Finds all gl functions usable by your system
	//*Does all other GL setup
	static void InitGLEW();
	//Initializes SDL
	//*Init Video and events
	//*Sets all SDL attriutes
	static void InitSDL();

	//Polls SDL events
	static void PollEvents(entt::registry* mainReg, bool* closeWindow, 
							bool * mouseMotion, bool * mouseClick, 
								bool * mouseWheel);
	
	//Draws the gamessss
	//*Calls draw on the rendering system
	static void Draw(entt::registry* mainReg);
	//Updates the game
	//*Update timer
	//*Update the rendering system
	//*Update the animation system
	static void Update(entt::registry* mainReg);

	//Runs the GUI
	//*Uses ImGUI for this
	static void GUI(entt::registry* reg, Scene* scene);

	//Reshapes the window
	//*Happens then WINDOW_EVENT_RESHAPE event is polled
	static void ReshapeWindow(int w, int h, entt::registry* mainReg);

	static void SetWindowName(std::string name);

	//Getters
	static Window* GetWindow();
	//Window Width
	static GLsizei GetWindowWidth();
	//Window Height
	static GLsizei GetWindowHeight();
	//Aspect Ratio
	static float GetAspectRatio();
	//Motion Event
	static MotionEvent GetMotionEvent();
	//Button Event
	static ClickEvent GetClickEvent();
	//Wheel Event
	static WheelEvent GetWheelEvent();

	//Setters
	//Window Width
	static void SetWindowWidth(GLsizei windowWidth);
	//Window Height
	static void SetWindowHeight(GLsizei windowHeight);
	//Aspect Ratio
	static void SetAspectRatio(float aspectRatio);

private:
	//Window
	static Window* m_window;

	//Window size and aspect
	static GLsizei m_windowWidth;
	static GLsizei m_windowHeight;
	static float m_aspectRatio;

	//Events
	static MotionEvent m_motion;
	static ClickEvent m_click;
	static WheelEvent m_wheel;
};

#endif // !__BACKEND_H__
