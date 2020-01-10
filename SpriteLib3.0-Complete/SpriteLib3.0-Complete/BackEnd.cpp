#include "BackEnd.h"

Window* BackEnd::m_window = nullptr;

GLsizei BackEnd::m_windowWidth = 0;
GLsizei BackEnd::m_windowHeight = 0;
float BackEnd::m_aspectRatio = 0.f;

MotionEvent BackEnd::m_motion;
ClickEvent BackEnd::m_click;
WheelEvent BackEnd::m_wheel;

void BackEnd::InitBackEnd(float windowWidth, float windowHeight)
{
	//Initializes SDL
	InitSDL();

	//Sets the backend window width, height, and aspect ratio
	m_windowWidth = (GLsizei)windowWidth;
	m_windowHeight = (GLsizei)windowHeight;
	m_aspectRatio = float(m_windowWidth) / float(m_windowHeight);
	//Creates new window with name of the scene as a caption
	m_window = new Window("", BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	//Initializes GLEW
	InitGLEW();

	EffectManager::InitEffectManager(unsigned(m_windowWidth), unsigned(m_windowHeight));
	//Initializes the rendering system
	RenderingSystem::Init();
}

void BackEnd::InitGLEW()
{
	//Holds the result of the initialization of glew
	//glewInit finds every OpenGL function that our system supports
	GLenum status = glewInit();

	//Checks if glewInit didn't return okay
	if (status != GLEW_OK)
	{
		//If it didn't return okay, we print out an error message
		std::cerr << "GLEW failed to initialize!" << std::endl;
	}

	//Enable depth testing (so z-layer system works)
	glEnable(GL_DEPTH_TEST);
	//Enables 2D textures
	glEnable(GL_TEXTURE_2D);

	//Culls backfaces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Allows blending (transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable MSAA
	glEnable(GL_MULTISAMPLE);
}

void BackEnd::InitSDL()
{
	//Initializes SDL with video and event capabilities
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	///Setting attributes
	//2 to the 8th power shades of Red(256)
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	//2 to the 8th power shades of Green(256)
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	//2 to the 8th power shades of Blue(256)
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	//2 to the 8th power shades of transparency(256)
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//How many bits are allocated for each pixel? We have 32 bits of color (see above), hence we allocate 32 bits
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	//Sets the depth buffer for OpenGL, needed for z buffering)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//Allocates the space for a second "window" that won't be displayed
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Set the GL context version (forces OpenGL version 3.2)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//Set up the antialiasing (4xMSAA)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	///End Attributes
}

void BackEnd::PollEvents(entt::registry* mainReg, bool * closeWindow,
							bool * mouseMotion, bool * mouseClick,
								bool * mouseWheel)
{
	//Polls events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			//If event is quit
				//Close the window
		case SDL_QUIT:
			*closeWindow = true;
			break;
			//if the event is a window event
				//Switch through possible window events
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//if the event is window resize
					//Reshape the window
			case SDL_WINDOWEVENT_RESIZED:
				ReshapeWindow(event.window.data1, event.window.data2, mainReg);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				Input::m_windowFocus = false;
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				Input::m_windowFocus = true;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			//Mouse Movement event
			*mouseMotion = true;
			m_motion = event.motion;
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			//Mouse Button event
			*mouseClick = true;
			m_click = event.button;
			break;
		case SDL_MOUSEWHEEL:
			//Mouse Wheel event
			*mouseWheel = true;
			m_wheel = event.wheel;
			break;
		}
	}
}

void BackEnd::Draw(entt::registry* mainReg)
{
	//Call draw on the register using the rendering system
	RenderingSystem::Draw(mainReg);
	PhysicsSystem::Draw(mainReg);
}

void BackEnd::Update(entt::registry* mainReg)
{
	//Sprite update system
	RenderingSystem::Update(mainReg);
	//Animation update system
	AnimationSystem::Update(Timer::deltaTime, mainReg);
}

void BackEnd::GUI(entt::registry* reg, Scene* scene)
{
	SceneEditor::CreateTab(reg, scene);
	EffectManager::CreateTab();
}

void BackEnd::ReshapeWindow(int w, int h, entt::registry * mainReg)
{
	//Reshapes the window when the window is resized
	glViewport(0, 0, GLsizei(w), GLsizei(h));
	//Resize all framebuffers here

	m_windowWidth = w;
	m_windowHeight = h;
	m_aspectRatio = float(w) / float(h);

	//Reshape all created buffers to be proper
	EffectManager::ReshapeBuffers(unsigned(w), unsigned(h));

	//Adjusts for aspect ratio
	vec4 temp = mainReg->get<Camera>(EntityIdentifier::MainCamera()).GetOrthoSize();
	auto& tempCam = mainReg->get<Camera>(EntityIdentifier::MainCamera());

	//Set values
	mainReg->get<Camera>(EntityIdentifier::MainCamera()).SetWindowSize(vec2(float(m_windowWidth), float(m_windowHeight)));
	mainReg->get<Camera>(EntityIdentifier::MainCamera()).Orthographic(m_aspectRatio, temp.x, temp.y, temp.z, temp.w, tempCam.GetNear(), tempCam.GetFar());

	tempCam.SetPosition(tempCam.GetPosition());
}

void BackEnd::SetWindowName(std::string name)
{
	m_window->SetWindowName(name);
}

Window * BackEnd::GetWindow()
{
	return m_window;
}

GLsizei BackEnd::GetWindowWidth()
{
	return m_windowWidth;
}

GLsizei BackEnd::GetWindowHeight()
{
	return m_windowHeight;
}

float BackEnd::GetAspectRatio()
{
	return m_aspectRatio;
}

MotionEvent BackEnd::GetMotionEvent()
{
	return m_motion;
}

ClickEvent BackEnd::GetClickEvent()
{
	return m_click;
}

WheelEvent BackEnd::GetWheelEvent()
{
	return m_wheel;
}

void BackEnd::SetWindowWidth(GLsizei windowWidth)
{
	m_windowWidth = windowWidth;
}

void BackEnd::SetWindowHeight(GLsizei windowHeight)
{
	m_windowHeight = windowHeight;
}

void BackEnd::SetAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;
}
