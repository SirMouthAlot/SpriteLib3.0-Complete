#include "Shader.h"

std::string Shader::ReadFile(const std::string & fileName) const
{
	//Creates an in file stream
	std::ifstream inStream(fileName);
	//Gives start of message
	std::cout << "Shader: " << fileName << " .";
	//If the stream isn't good
		//Give error message and return empty string
	if (!inStream.good())
	{
		std::cout << ".. failed to load!\n";
		return std::string();
	}
	//Give success message
	std::cout << ".. loaded successfully!\n";

	//read the entire file into data
	std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));
	//return data
	return data;
}

bool Shader::CompileShader(GLuint shader) const
{
	//Compile the shader
	glCompileShader(shader);
	//Success flag
	GLint success;
	//Get the shader iv, and store the success flag within success
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	//return flag
	return success;
}

void Shader::OutputShaderLog(GLuint shader) const
{
	//error log
	GLchar error[1024] = { 0 };
	//Get the shader info log
	glGetShaderInfoLog(shader, sizeof(error), NULL, error);

	//Output the error
	std::cerr << "'" << error << "'" << std::endl;
}

void Shader::OutputProgramLog() const
{
	//error log
	GLchar error[1024] = { 0 };
	//Get the program info log
	glGetProgramInfoLog(m_program, sizeof(error), NULL, error);

	//Output the error
	std::cerr << "'" << error << "'" << std::endl;
}

GLint Shader::GetUniformLocation(const std::string & uniformName) const
{
	//Stores uniform location based on the name of the uniform
	GLint uniLoc = glGetUniformLocation(m_program, static_cast<const GLchar*>(uniformName.c_str()));

	//return location
	return uniLoc;
}

Shader::Shader(const std::string & vertFile, const std::string & fragFile, const std::string & geomFile)
{
	//Load the shader
	Load(vertFile, fragFile, geomFile);
}

Shader::~Shader()
{
	//if shader is initialized
	if (m_isInit)
	{
		//Unload the shader
		Unload();
	}
}

bool Shader::Load(const std::string & vertFile, const std::string & fragFile, const std::string & geomFile)
{
	//Store file names
	m_vertShaderFile = vertFile;
	m_fragShaderFile = fragFile;
	m_geomShaderFile = geomFile;

	//Create the handles
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (geomFile != "")
	{
		m_geomShader = glCreateShader(GL_GEOMETRY_SHADER);
	}
	m_program = glCreateProgram();

	//Read in the vert file
	std::string source = ReadFile(vertFile);
	const GLchar *temp = static_cast<const GLchar*>(source.c_str());
	//set the handle's source
	glShaderSource(m_vertexShader, 1, &temp, NULL);


	//Read in the frag file
	source = ReadFile(fragFile);
	temp = static_cast<const GLchar*>(source.c_str());
	//set the handle's source
	glShaderSource(m_fragShader, 1, &temp, NULL);

	//if geom file exists
	if (geomFile != "")
	{
		//Read in geom file
		source = ReadFile(geomFile);
		temp = static_cast<const GLchar*>(source.c_str());
		//set the handle's source
		glShaderSource(m_geomShader, 1, &temp, NULL);
	}

	//If the shader fails to compile
		//Give error message
		//Output shader error log
		//Unload shader
	if (!CompileShader(m_vertexShader))
	{
		std::cout << "Vertex shader failed to compile.\n";
		OutputShaderLog(m_vertexShader);
		Unload();
		return false;
	}

	//If the shader fails to compile
		//Give error message
		//Output shader error log
		//Unload shader
	if (!CompileShader(m_fragShader))
	{
		std::cout << "Fragment shader failed to compile.\n";
		OutputShaderLog(m_fragShader);
		Unload();
		return false;
	}

	//if the shader isn't non existant
	if (geomFile != "")
	{
		//If the shader fails to compile
			//Give error message
			//Output shader error log
			//Unload shader
		if (!CompileShader(m_geomShader))
		{
			std::cout << "Geometry shader failed to compile.\n";
			OutputShaderLog(m_geomShader);
			Unload();
			return false;
		}
	}

	//Attach the shaders to the program
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragShader);
	if (geomFile != "")
	{
		glAttachShader(m_program, m_geomShader);
	}

	//If the program fails to link
		//Give error message
		//Output program error log
		//Unload shader
	if (!LinkProgram())
	{
		std::cout << "Program failed to link.\n";
		OutputProgramLog();
		Unload();
		return false;
	}

	//set init to true
	m_isInit = true;
	return true;
}

bool Shader::IsLoaded() const
{
	//return isInit
	return m_isInit;
}

void Shader::Unload()
{
	//If vert shader isn't GL_NONE
		//Detach shader from program
		//Delete the shader
	if (m_vertexShader != GL_NONE)
	{
		glDetachShader(m_program, m_vertexShader);
		glDeleteShader(m_vertexShader);
		m_vertexShader = GL_NONE;
	}

	//If frag shader isn't GL_NONE
		//Detach shader from program
		//Delete the shader
	if (m_fragShader != GL_NONE)
	{
		glDetachShader(m_program, m_fragShader);
		glDeleteShader(m_fragShader);
		m_fragShader = GL_NONE;
	}

	//If geom shader isn't GL_NONE
		//Detach shader from program
		//Delete the shader
	if (m_geomShader != GL_NONE)
	{
		glDetachShader(m_program, m_geomShader);
		glDeleteShader(m_geomShader);
		m_geomShader = GL_NONE;
	}

	//If program isn't GL_NONE
		//Delete program
	if (m_program != GL_NONE)
	{
		glDeleteProgram(m_program);
		m_program = GL_NONE;
	}
}

bool Shader::LinkProgram()
{
	//Link the program
	glLinkProgram(m_program);
	GLint success;
	//Store the success rating
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	//return success rating
	return success;
}

void Shader::Bind() const
{
	//Bind program
	glUseProgram(m_program);
}

void Shader::Unbind() const
{
	//Unbind program
	glUseProgram(GL_NONE);
}

void Shader::SendUniform(const std::string & uniformName, const int & i) const
{
	//Send integer uniform
	GLint loc = GetUniformLocation(uniformName);
	glUniform1i(loc, i);
}

void Shader::SendUniform(const std::string & uniformName, const unsigned int & i) const
{
	//Send unsigned int uniform
	GLint loc = GetUniformLocation(uniformName);
	glUniform1ui(loc, i);
}

void Shader::SendUniform(const std::string & uniformName, const float & f) const
{
	//Send float uniform
	GLint loc = GetUniformLocation(uniformName);
	glUniform1f(loc, f);
}

void Shader::SendUniform(const std::string & uniformName, const vec2 & vector) const
{
	//Send vec2 uniform
	GLint loc = GetUniformLocation(uniformName);
	GLfloat vec[2] = { vector.x, vector.y };
	glUniform2fv(loc, 1, vec);
}

void Shader::SendUniform(const std::string & uniformName, const float & x, const float & y) const
{
	//Send vec2 uniform using separate float parameters
	GLint loc = GetUniformLocation(uniformName);
	GLfloat vec[2] = { x, y };
	glUniform2fv(loc, 1, vec);
}

void Shader::SendUniform(const std::string & uniformName, const vec3 & vector) const
{
	//Send vec3 uniform
	GLint loc = GetUniformLocation(uniformName);
	GLfloat vec[3] = { vector.x, vector.y, vector.z };
	glUniform3fv(loc, 1, vec);
}

void Shader::SendUniform(const std::string & uniformName, const float & x, const float & y, const float & z) const
{
	//Send vec3 uniform using separate float parameters
	GLint loc = GetUniformLocation(uniformName);
	GLfloat vec[3] = { x, y, z };
	glUniform3fv(loc, 1, vec);
}

void Shader::SendUniform(const std::string & uniformName, const vec4 & vector) const
{
	//Send vec4 uniform
	GLint loc = GetUniformLocation(uniformName);
	GLfloat vec[4] = { vector.x, vector.y, vector.z, vector.w };
	glUniform4fv(loc, 1, vec);
}

void Shader::SendUniform(const std::string & uniformName, const float & x, const float & y, const float & z, const float & w) const
{
	//Send vec4 uniform using separate float parameters
	GLint loc = GetUniformLocation(uniformName);
	GLfloat vec[4] = { x, y, z, w };
	glUniform4fv(loc, 1, vec);
}

void Shader::SendUniform(const std::string & uniformName, const mat3 & matrix) const
{
	//Send 3x3 matrix uniform
	GLint loc = GetUniformLocation(uniformName);
	GLfloat mat[9] = {matrix.row1.x, matrix.row2.x, matrix.row3.x,
					  matrix.row1.y, matrix.row2.y, matrix.row3.y,
					  matrix.row1.z, matrix.row2.z, matrix.row3.z };
	glUniformMatrix3fv(loc, 1, GL_FALSE, mat);
}

void Shader::SendUniform(const std::string & uniformName, const mat4 & matrix) const
{
	//Send 4x4 matrix uniform
	GLint loc = GetUniformLocation(uniformName);
	GLfloat mat[16] = { matrix.row1.x, matrix.row2.x, matrix.row3.x, matrix.row4.x,
						matrix.row1.y, matrix.row2.y, matrix.row3.y, matrix.row4.y,
						matrix.row1.z, matrix.row2.z, matrix.row3.z, matrix.row4.z,
						matrix.row1.w, matrix.row2.w, matrix.row3.w, matrix.row4.w };
	glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}

