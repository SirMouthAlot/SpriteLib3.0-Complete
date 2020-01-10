#include "VertexManager.h"

//Initialized Plane Vertices VBO
GLuint VertexManager::m_planeVertVBO = GL_NONE;
GLuint VertexManager::m_planeUVVBO = GL_NONE;

//VAOs and VBOs loaded
std::vector<GLuint> VertexManager::m_VAOs = std::vector<GLuint>(GL_NONE);
std::vector<GLuint> VertexManager::m_VBOs = std::vector<GLuint>(GL_NONE);

VertexManager::~VertexManager()
{
	//Unloads the VAOs
	UnloadVAOs();
	//Unloads the VBOs
	UnloadVBOs();
}

void VertexManager::InitPlaneData()
{
	//The vertices cooresponding to our sprite plane
	//It's a 1x1 unit square
	float VBO_DATA[]{
	-0.5f, -0.5f, 0.f,	//Bottom left
	0.5f, -0.5f, 0.f,	//Bottom right
	-0.5f, 0.5f, 0.f,	//Top left

	0.5f, 0.5f, 0.f,	//Top right
	-0.5f, 0.5f, 0.f,	//Top left
	0.5f, -0.5f, 0.f,	//Bottom right
	};
	int vertexSize = 6 * 3 * sizeof(float);

	//Initializes VBO data as
	//0-1 from bottom left to top right
	float UV_DATA[]{
		0.f, 0.f,
		1.f, 0.f,
		0.f, 1.f,

		1.f, 1.f,
		0.f, 1.f,
		1.f, 0.f,
	};
	int texCoordSize = 6 * 2 * sizeof(float);
	
	//Creates the Vertice VBO
	glGenBuffers(1, &m_planeVertVBO);

	//Binds the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_planeVertVBO);
	//Buffers the vertex data
	glBufferData(GL_ARRAY_BUFFER, vertexSize, VBO_DATA, GL_STATIC_DRAW);

	//Creates the UV VBO
	glGenBuffers(1, &m_planeUVVBO);
	
	//binds the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_planeUVVBO);
	//Buffers the vertex data
	glBufferData(GL_ARRAY_BUFFER, texCoordSize, UV_DATA, GL_STATIC_DRAW);
}

GLuint VertexManager::CreateVAO()
{
	//Pushes a new VAO handle into our VAO vector
	m_VAOs.push_back(GL_NONE);
	
	//Creates a reference to it
	GLuint& tempVAO = m_VAOs[m_VAOs.size() - 1];

	//Generates a Vertex array, with a handle stored in our new VAO
	glGenVertexArrays(1, &tempVAO);

	//Returns our newly created VAO
	return tempVAO;
}

GLuint VertexManager::CreateVBO()
{
	//Pushes a new VBO handle into our VBO vector
	m_VBOs.push_back(GL_NONE);

	//Creates a reference to it
	GLuint& tempVBO = m_VBOs[m_VBOs.size() - 1];

	//Generates a buffer, with a handle stored in our new VBO
	glGenBuffers(1, &tempVBO);

	//Returns our newly created VBO
	return tempVBO;
}

GLuint VertexManager::GetPlaneVertVBO()
{
	//Returns our intialized vertex VBO
	return m_planeVertVBO;
}

GLuint VertexManager::GetPlaneUVVBO()
{
	return m_planeUVVBO;
}

void VertexManager::UnloadVAOs()
{
	//Loops through all of the loaded VAOs
	for (unsigned i = 0; i < m_VAOs.size(); i++)
	{
		//Binds the vertex array
		glBindVertexArray(m_VAOs[i]);
		//Disables the attrib arrays
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		//Deletes the vertex array
		glDeleteVertexArrays(1, &m_VAOs[i]);

		//Sets the buffer back to zero
		m_VAOs[i] = GL_NONE;
	}

	//Binds vertex array to none
	glBindVertexArray(GL_NONE);
}

void VertexManager::UnloadVBOs()
{
	//Deletes the Vertex VBO we initalized separately
	glDeleteBuffers(1, &m_planeVertVBO);
	//Sets buffer back to zero
	m_planeVertVBO = GL_NONE;

	for (unsigned i = 0; i < m_VBOs.size(); i++)
	{
		//Delete buffers
		glDeleteBuffers(1, &m_VBOs[i]);

		//Sets the buffer back to zero
		m_VBOs[i] = GL_NONE;
	}
}
