#ifndef __VERTEXMANAGER_H__
#define __VERTEXMANAGER_H__

#include "GL/glew.h"
#include <vector>

//Basically just handles our Vertex Array/Buffer Objects
class VertexManager abstract
{
public:
	//Runs the unload functions
	~VertexManager();

	//Initializes the Plane Vertices VBO
	static void InitPlaneData();

	//Creates a new VAO and returns the handle
	static GLuint CreateVAO();
	//Creates a new VBO and returns the handle
	static GLuint CreateVBO();
	//Grabs the plane vertices VBO and returns the handle
	static GLuint GetPlaneVertVBO();
	//Grabs the plane UV VBO and returns the handle
	static GLuint GetPlaneUVVBO();

	//Unloads all of the currently loaded VAOs
	static void UnloadVAOs();
	//Unloads all of the currently loaded VBOs
	static void UnloadVBOs();
private:
	//Holds our initialized plane vertices VBO
	static GLuint m_planeVertVBO;
	static GLuint m_planeUVVBO;

	//Holds our loaded VAOs
	static std::vector<GLuint> m_VAOs;
	//Holds our loaded VBOs
	static std::vector<GLuint> m_VBOs;
};

#endif // !__VERTEXMANAGER_H__
