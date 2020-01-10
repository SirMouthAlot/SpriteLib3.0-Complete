#include "Xinput.h"
#include <iostream>
#include <string>

XInputController XInputManager::controllers[4];

XInputController::XInputController(int index)
{

	SetControllerIndex(index);
}

void XInputController::SetControllerIndex(int index)
{
	//Just sets the current controller's index
	m_index = index;
}

void XInputController::Update()
{
	//If the index is within the valid 0-3 range
	//(aka controllers 1-4)
	//We grab the info for the controller at that index
	if (m_index >= 0 && m_index <= 3)
		XInputGetState(DWORD(m_index), &m_info);
}

void XInputController::PrintControllerInfo()
{
	Triggers triggers;
	Stick sticks[2];

	GetTriggers(triggers);
	GetSticks(sticks);

	std::cout << "Controller " << m_index << std::endl;

	std::cout << "A button : " << (IsButtonPressed(A) ? "Pressed" : "Released") << std::endl;
	std::cout << "B button : " << (IsButtonPressed(B) ? "Pressed" : "Released") << std::endl;
	std::cout << "X button: " << (IsButtonPressed(X) ? "Pressed" : "Released") << std::endl;
	std::cout << "Y button : " << (IsButtonPressed(Y) ? "Pressed" : "Released") << std::endl;
	std::cout << "Left Bumper : " << (IsButtonPressed(LB) ? "Pressed" : "Released") << std::endl;
	std::cout << "Right Bumper : " << (IsButtonPressed(RB) ? "Pressed" : "Released") << std::endl;
	std::cout << "Right Trigger : " << std::to_string(triggers.RT) << std::endl;
	std::cout << "Left Trigger : " << std::to_string(triggers.LT) << std::endl;
	std::cout << "Left Stick : (" << std::to_string(sticks[0].x) << ", " << std::to_string(sticks[0].y) << ")" << std::endl;
	std::cout << "Right Stick : (" << std::to_string(sticks[1].x) << ", " << std::to_string(sticks[1].y) << ")" << std::endl;
	std::cout << "Left Thumb Stick : " << (IsButtonPressed(THUMB_LEFT) ? "Pressed" : "Released") << std::endl;
	std::cout << "Right Thumb Stick : " << (IsButtonPressed(THUMB_RIGHT) ? "Pressed" : "Released") << std::endl;
	std::cout << "DPad Up : " << (IsButtonPressed(DPAD_UP) ? "Pressed" : "Released") << std::endl;
	std::cout << "DPad Down : " << (IsButtonPressed(DPAD_DOWN) ? "Pressed" : "Released") << std::endl;
	std::cout << "DPad Left : " << (IsButtonPressed(DPAD_LEFT) ? "Pressed" : "Released") << std::endl;
	std::cout << "DPad Right : " << (IsButtonPressed(DPAD_RIGHT) ? "Pressed" : "Released") << std::endl;
	std::cout << "Select button : " << (IsButtonPressed(SELECT) ? "Pressed" : "Released") << std::endl;
	std::cout << "Start button : " << (IsButtonPressed(START) ? "Pressed" : "Released") << std::endl;
}

int XInputController::GetButtonBitmask()
{
	//If the bitmask for all the buttons is not just equal to zero
		//We return the bitmask
	//If not
		//We just save some time and return NULL
	return m_info.Gamepad.wButtons > 0 ? m_info.Gamepad.wButtons : NULL;
}

bool XInputController::IsButtonPressed(int bitmask)
{
	//Takes the bitmask for all the buttons 
	//and performs a simple &(AND) operation with the parameter bitmask
	//If the button is pressed, 
		//It'll return true, 
	//If not, 
		//It'll return false
	return m_info.Gamepad.wButtons & bitmask;
}

bool XInputController::IsButtonReleased(int bitmask)
{
	//Takes the bitmask for all the buttons
	//and performs a !(NOT) operation and an &(AND) operation with the parameter bitmask
	//If the button is pressed,
		//It'll return false,
	//If not,
		//It'll return true
	return !(m_info.Gamepad.wButtons & bitmask);
}

bool XInputController::IsButtonStroked(int bitmask)
{
	//if the button is currently pressed
	//Set the stroke for this button to true
	if (IsButtonPressed(bitmask))
		m_stroke[bitmask] = true;

	//If the button was being stroked 
	//AND the button is now released
	//It means a stroke happened, and we return true
	else if (m_stroke[bitmask] && IsButtonReleased(bitmask))
	{
		m_stroke[bitmask] = false;
		return true;
	}

	//Otherwise we return false
	return false;
}

void XInputController::SetStickDeadZone(float deadZone)
{
	m_deadZoneStick = deadZone;
}

void XInputController::SetTriggerDeadZone(float deadZone)
{
	m_deadZoneTrigger = deadZone;
}

void XInputController::GetSticks(Stick sticks[2])
{
	///LEFT STICK///
	float x = float(m_info.Gamepad.sThumbLX / 32768.f);
	float y = float(m_info.Gamepad.sThumbLY / 32768.f);


	///X///
	//This checks basically 
	//If the movement of the stick is less than the deadzone
		//Set the stick movement to zero
	//If the thumbstick movement value is less than zero
		//Move it into a value from 0 to -1
	//If the thumbstick movement value is greater than zero
		//Move it into a value from 0 to 1
	if (sqrt(x * x + y * y) < m_deadZoneStick)
		sticks[0].x = 0;
	else if (m_info.Gamepad.sThumbLX < 0)
		sticks[0].x = float(m_info.Gamepad.sThumbLX / 32768.f);
	else
		sticks[0].x = float(m_info.Gamepad.sThumbLX / 32767.f);

	///Y///
	//This checks basically 
	//If the movement of the stick is less than the deadzone
		//Set the stick movement to zero
	//If the thumbstick movement value is less than zero
		//Move it into a value from 0 to -1
	//If the thumbstick movement value is greater than zero
		//Move it into a value from 0 to 1
	if (sqrt(x * x + y * y) < m_deadZoneStick)
		sticks[0].y = 0;
	else if (m_info.Gamepad.sThumbLY < 0)
		sticks[0].y = float(m_info.Gamepad.sThumbLY / 32768.f);
	else
		sticks[0].y = float(m_info.Gamepad.sThumbLY / 32767.f);

	///RIGHT STICK///
	x = float(m_info.Gamepad.sThumbRX / 32768.f);
	y = float(m_info.Gamepad.sThumbRY / 32768.f);

	///X///
	//This checks basically 
	//If the movement of the stick is less than the deadzone
		//Set the stick movement to zero
	//If the thumbstick movement value is less than zero
		//Move it into a value from 0 to -1
	//If the thumbstick movement value is greater than zero
		//Move it into a value from 0 to 1
	if (sqrt(x * x + y * y) < m_deadZoneStick)
		sticks[1].x = 0;
	else if (m_info.Gamepad.sThumbRX < 0)
		sticks[1].x = float(m_info.Gamepad.sThumbRX / 32768.f);
	else
		sticks[1].x = float(m_info.Gamepad.sThumbRX / 32767.f);

	///Y///
	//This checks basically 
	//If the movement of the stick is less than the deadzone
		//Set the stick movement to zero
	//If the thumbstick movement value is less than zero
		//Move it into a value from 0 to -1
	//If the thumbstick movement value is greater than zero
		//Move it into a value from 0 to 1
	if (sqrt(x * x + y * y) < m_deadZoneStick)
		sticks[1].y = 0;
	else if (m_info.Gamepad.sThumbRY < 0)
		sticks[1].y = float(m_info.Gamepad.sThumbRY / 32768.f);
	else
		sticks[1].y = float(m_info.Gamepad.sThumbRY / 32767.f);
}

void XInputController::GetTriggers(Triggers & triggers)
{
	triggers =
	{
		float(m_info.Gamepad.bLeftTrigger / 255.f),
		float(m_info.Gamepad.bRightTrigger / 255.f)
	};
}

float XInputController::GetStickDeadZone()
{
	return m_deadZoneStick;
}

float XInputController::GetTriggerDeadZone()
{
	return m_deadZoneTrigger;
}

XInputManager::XInputManager()
{
	//Starting index
	int index = 0;
	//For every controller within controllers variable
	//Set the controller index for each controller 0-3
	for (auto &controller : controllers)
		controller.SetControllerIndex(index++);
}

bool XInputManager::ControllerConnected(int index)
{
	//If the index passed is less than zero and greater than 3
		//We just return false instantly as it's not a valid index
	if (index < 0 && index > 3)
		return false;

	XINPUT_STATE connected;
	return XInputGetState(index, &connected) == ERROR_SUCCESS;
}

XInputController * XInputManager::GetController(int index)
{
	//If the index passed is greater than or equal to
	//0 and less than or equal to 3
		//We return a reference to the controller at the passed index
	if (index >= 0 && index <= 3)
	{
		controllers[index].SetControllerIndex(index);
		return &controllers[index];
	}


	//Otherwise, we just return nullptr
	return nullptr;
}

void XInputManager::Update()
{
	//Loops through every index 0 to 3
	//aka controllers index 1-4
	for (unsigned index = 0; index < 4; index++)
	{
		//if there is a controller connected at this index
			//Update the controller at this index
		if (ControllerConnected(index))
		{
			controllers[index].Update();
		}
	}
}
