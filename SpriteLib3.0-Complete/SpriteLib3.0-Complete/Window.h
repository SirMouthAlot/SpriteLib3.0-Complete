#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>

#include <string>
#include <iostream>
#include <GL/glew.h>
#include "Vector.h"

//This is a wrapper for all the raw SDL window stuff
class Window
{
public:
	//creates window
	//*Gives it a name
	//*Width
	//*Height
	//*Sets whether or not you can resize it
	Window(const std::string title, int windowWidth, int windowHeight, bool resizable=true);
	//Deconstructs window
	~Window();

	//Clear the screen with specific color
	void Clear(float r, float g, float b, float a);
	//Clears the screen with specific color
	void Clear(vec4 color);
	//Flips the screen
	//*Uses a double buffer system
	void Flip();

	//Set window name
	void SetWindowName(std::string name);

	//Set whether or not the window is resizable
	void SetWindowResizable(bool resizable);
	//Sets the window being fullscreen
	void SetFullscreen(Uint32 flags);
	//Grabs whether or not the window is fullscreen
	Uint32 GetFullscreen() const;

	//Closes the window
	void Close();

	//returns whether or not the window is still open
	inline bool isOpen() const { return m_open; }
private:
	//We don't need windows being copied
	Window(const Window&);
	
	//SDL window value
	SDL_Window *m_window = nullptr;
	//SDL Window Context 
	SDL_GLContext m_context = NULL;
	//Is the window open?
	bool m_open = false;
	//Is the window fullscreen?
	Uint32 m_fullscreen;
};

#endif
