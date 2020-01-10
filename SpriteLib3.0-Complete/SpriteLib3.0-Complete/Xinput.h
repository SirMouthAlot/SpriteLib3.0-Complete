#ifndef __XINPUT_H__
#define __XINPUT_H__

#include <Windows.h>
#include <Xinput.h>
#include <unordered_map>

#pragma comment(lib, "XInput.lib")

//Buttons used for the controller
enum Buttons
{
	DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
	DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	THUMB_LEFT = XINPUT_GAMEPAD_LEFT_THUMB,
	THUMB_RIGHT = XINPUT_GAMEPAD_RIGHT_THUMB,
	SELECT = XINPUT_GAMEPAD_BACK,
	START = XINPUT_GAMEPAD_START
};

//Stick names
enum StickNames
{
	LS,
	RS
};

//Trigger names
enum TriggerNames
{
	LT,
	RT
};

//The x and y input values
//The stick coordinates are given a value from -1 to 1
struct Stick
{
	float x;
	float y;
};

//The left and right trigger values
//The triggers are given a value between 0 & 1
struct Triggers
{
	float LT;
	float RT;
};

class XInputController
{
public:
	XInputController(int index = -1);
	~XInputController() { }

	//Sets this specific controller's index
	void SetControllerIndex(int index);
	
	//Updates the controller's state
	void Update();

	void PrintControllerInfo();
	
	//Button functions
	int GetButtonBitmask();
	bool IsButtonPressed(int bitmask);
	bool IsButtonReleased(int bitmask);
	bool IsButtonStroked(int bitmask);

	//Helper functions
	//Setters
	void SetStickDeadZone(float deadZone);
	void SetTriggerDeadZone(float deadZone);
	
	//Getters
	void GetSticks(Stick sticks[2]);
	void GetTriggers(Triggers &triggers);
	float GetStickDeadZone();
	float GetTriggerDeadZone();

private:
	int m_index;
	float m_deadZoneStick;
	float m_deadZoneTrigger;

	XINPUT_STATE m_info;

	std::unordered_map<int, bool> m_stroke;
};

class XInputManager
{
public:
	XInputManager();

	//Checks if a controller from index 0 to 3 
	//aka controller index 1-4
	//is connected
	static bool ControllerConnected(int index);

	//Gets the controller from index 0 to 3
	//aka controller index 1-4
	//is connected
	static XInputController* GetController(int index);

	//Updates all controllers connected to the manager
	static void Update();

private:
	static XInputController controllers[4];
};

#endif // !__XINPUT_H__
