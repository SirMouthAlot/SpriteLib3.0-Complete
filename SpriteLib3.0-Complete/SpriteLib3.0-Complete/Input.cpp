#include "Input.h"

//Sets all of the key flags to initial input of zero
char Input::m_keyHandledFlag[256] = { 0 };
char Input::m_keyPressedFlag[256] = { 0 };
char Input::m_keyReleasedFlag[256] = { 0 };
char Input::m_keyWasPressed[256] = { 0 };

bool Input::m_windowFocus = true;

bool Input::GetKey(Key::In inputKey)
{
	//Checks if the key was pressed
	bool pressed = (GetAsyncKeyState(inputKey) >> 15) & 1;

	//If it was pressed set the press flag to true
	if (pressed)
	{
		m_keyWasPressed[inputKey] = 1;
	}

	//Return if it was pressed
	return pressed;
}

bool Input::GetKeyDown(Key::In inputKey)
{
	//Checks if this key was already handled this frame
	//Just skips the checks and returns true if it was
	if (m_keyHandledFlag[inputKey] == 1)
	{
		return true;
	}
	//If key was down and we didn't already have it held down, it means it was pressed
	if (GetKey(inputKey) && m_keyPressedFlag[inputKey] == 0)
	{
		//We say this key has been handled
		m_keyHandledFlag[inputKey] = 1;
		//We say the key has been pressed as well
		m_keyPressedFlag[inputKey] = 1;
		//the key was pressed, so we return true
		return true;
	}
	//If the key isn't currently being held down, we need to make the pressed flag reflect that
	if (!GetKey(inputKey))
	{
		m_keyPressedFlag[inputKey] = 0;
	}
	//And because of this we return false
	return false;
}

bool Input::GetKeyUp(Key::In inputKey)
{
	//If key is not being held down currently BUT the key WAS held down, as of last reset
	//It means that the key press was ended as of this frame
	if (!GetKey(inputKey) && m_keyWasPressed[inputKey] == 1)
	{
		//Obviously we need to return true if this is the case
		return true;
	}
	//If it's untrue, then we need to return false
	return false;
}

void Input::ResetKeys()
{
	//For each individual key we need to reset them
	for (unsigned i = 0; i < 256; i++)
	{
		//If it's not currently being pressed, we need to reset the
		//"was" pressed flag to zero because it wasn't pressed
		if (((GetAsyncKeyState(i) >> 15) & 1) == 0)
		{
			m_keyWasPressed[i] = 0;
		}

		// Every frame we need to reset the released and handled flags
		m_keyReleasedFlag[i] = 0;
		m_keyHandledFlag[i] = 0;
	}
}
