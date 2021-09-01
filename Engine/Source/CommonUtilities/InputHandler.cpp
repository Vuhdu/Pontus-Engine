#include "pch.h"
#include "InputHandler.h"

bool CommonUtilities::InputHandler::IsKeyDown(const int aKeyCode)
{
	if (myKeyState[aKeyCode].myIsDown == true)
	{
		myKeyState[aKeyCode].myIsDown = false;
		return true;
	}
	return false;
}

bool CommonUtilities::InputHandler::IsKeyDown(const eKeyCode aKeyCode)
{
	return IsKeyDown(static_cast<int>(aKeyCode));
}

bool CommonUtilities::InputHandler::IsKeyDown(const eMouseButton aMouseButton)
{
	return IsKeyDown(static_cast<int>(aMouseButton));
}

bool CommonUtilities::InputHandler::IsKeyUp(const int aKeyCode)
{
	if (myKeyState[aKeyCode].myIsUp == true)
	{
		myKeyState[aKeyCode].myIsUp = false;
		return true;
	}
	return false;
}

bool CommonUtilities::InputHandler::IsKeyUp(const eKeyCode aKeyCode)
{
	return IsKeyUp(static_cast<int>(aKeyCode));
}

bool CommonUtilities::InputHandler::IsKeyUp(const eMouseButton aMouseButton)
{
	return IsKeyUp(static_cast<int>(aMouseButton));
}

bool CommonUtilities::InputHandler::IsKeyPressed(const int aKeyCode) const
{
	return myKeyState[aKeyCode].myIsPressed;
}

bool CommonUtilities::InputHandler::IsKeyPressed(const eKeyCode aKeyCode) const
{
	return IsKeyPressed(static_cast<int>(aKeyCode));
}

bool CommonUtilities::InputHandler::IsKeyPressed(const eMouseButton aMouseButton) const
{
	return IsKeyPressed(static_cast<int>(aMouseButton));
}

POINT CommonUtilities::InputHandler::GetMousePosition() const
{
	return myMousePos;
}

POINT CommonUtilities::InputHandler::GetMousePositionClientRelative() const
{
	return myMousePosClientRelative;
}

bool CommonUtilities::InputHandler::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	bool detectMessage = false;

	for (int keyCode = 0; keyCode < 254; ++keyCode)
	{
		myKeyState[keyCode].myIsUp = false;
	}

	switch (message)
	{
	case WM_KEYDOWN:
	{
		int keyCode = static_cast<int>(wParam);

		switch (keyCode)
		{
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU:
			keyCode = GetRightOrLeftKeyCode(keyCode, lParam);
			break;
		default:
			break;
		}

		if (myKeyState[keyCode].myIsPressed == false)
		{
			myKeyState[keyCode].myIsDown = true;
		}
		myKeyState[keyCode].myIsPressed = true;
		detectMessage = true;
	} break;

	case WM_KEYUP:
	{
		int keyCode = static_cast<int>(wParam);

		switch (keyCode)
		{
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU:
			keyCode = GetRightOrLeftKeyCode(keyCode, lParam);
			break;
		default:
			break;
		}

		myKeyState[keyCode].myIsUp = true;
		myKeyState[keyCode].myIsDown = false;
		myKeyState[keyCode].myIsPressed = false;
		detectMessage = true;
	} break;

	case WM_MOUSEMOVE:
		POINT p;
		if (GetCursorPos(&p))
		{
			myMousePos.x = p.x;
			myMousePos.y = p.y;
		}
		myMousePosClientRelative.x = GET_X_LPARAM(lParam);
		myMousePosClientRelative.y = GET_Y_LPARAM(lParam);
		detectMessage = true;
		break;

	case WM_LBUTTONDOWN:
		if (myKeyState[VK_LBUTTON].myIsPressed == false)
		{
			myKeyState[VK_LBUTTON].myIsDown = true;
		}
		myKeyState[VK_LBUTTON].myIsPressed = true;
		detectMessage = true;
		break;

	case WM_LBUTTONUP:
		myKeyState[VK_LBUTTON].myIsUp = true;
		myKeyState[VK_LBUTTON].myIsDown = false;
		myKeyState[VK_LBUTTON].myIsPressed = false;
		detectMessage = true;
		break;

	case WM_RBUTTONDOWN:
		if (myKeyState[VK_RBUTTON].myIsPressed == false)
		{
			myKeyState[VK_RBUTTON].myIsDown = true;
		}
		myKeyState[VK_RBUTTON].myIsPressed = true;
		detectMessage = true;
		break;

	case WM_RBUTTONUP:
		myKeyState[VK_RBUTTON].myIsUp = true;
		myKeyState[VK_RBUTTON].myIsDown = false;
		myKeyState[VK_RBUTTON].myIsPressed = false;
		detectMessage = true;
		break;

	case WM_MBUTTONDOWN:
		if (myKeyState[VK_MBUTTON].myIsPressed == false)
		{
			myKeyState[VK_MBUTTON].myIsDown = true;
		}
		myKeyState[VK_MBUTTON].myIsPressed = true;
		detectMessage = true;
		break;

	case WM_MBUTTONUP:
		myKeyState[VK_MBUTTON].myIsUp = true;
		myKeyState[VK_MBUTTON].myIsDown = false;
		myKeyState[VK_MBUTTON].myIsPressed = false;
		detectMessage = true;
		break;

	default:
		break;
	}

	return detectMessage;
}

int CommonUtilities::InputHandler::GetRightOrLeftKeyCode(const int aVirtualKeyCode, const LPARAM lParam)
{
	int resultKeyCode = aVirtualKeyCode;

	const auto scanCode = static_cast<UINT>((lParam & 0x00ff0000) >> 16);
	const bool isExtended = (lParam & 0x01000000);

	switch (aVirtualKeyCode)
	{
	case VK_SHIFT:
		// Convert the scan code to a virtual key code
		resultKeyCode = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
		break;
	case VK_CONTROL:
		resultKeyCode = isExtended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		resultKeyCode = isExtended ? VK_RMENU : VK_LMENU;
		break;
	default:
		break;
	}

	return resultKeyCode;
}
