#pragma once
#include "KeyCodes.h"
#include "MouseCodes.h"
#include <unordered_map>
#include <iostream>
#include <Vec2.h>

namespace LestaTest
{

	enum class eCursorBehaviour
	{
		INVISIBLE,
		VISIBLE,
		INVISIBLE_WHEN_PRESSED,
		CUSTOM
	};

	enum class eInputState
	{
		IDLE,
		PRESSED,
		HELD,
		RELEASED
	};

	struct InputInfo
	{
		eInputState currentState = eInputState::IDLE;
		eInputState previousState = eInputState::IDLE;
	};

	class Input
	{
	public:

		static void onCursorVisibilityChanged(const Math::Vec2& mousePos)
		{
			mousePos_ = mousePos;
		}

		static void onMousePosChanged(const Math::Vec2& mousePos)
		{
			mousePos_ = mousePos;
		}

		static Math::Vec2 getMousePosition()
		{
			return mousePos_;
		}

		static void onKeyPressed(eKeyCode code)
		{
			keyMap_[code].previousState = keyMap_[code].currentState;
			keyMap_[code].currentState = eInputState::PRESSED;
		}

		static void onKeyReleased(eKeyCode code)
		{

			keyMap_[code].previousState = keyMap_[code].currentState;
			keyMap_[code].currentState = eInputState::RELEASED;
		}

		static void onMouseButtonPressed(eMouseCode code)
		{
			mouseButtonMap_[code].previousState = mouseButtonMap_[code].currentState;
			mouseButtonMap_[code].currentState = eInputState::PRESSED;
		}

		static void onMouseButtonReleased(eMouseCode code)
		{

			mouseButtonMap_[code].previousState = mouseButtonMap_[code].currentState;
			mouseButtonMap_[code].currentState = eInputState::RELEASED;
		}



		static bool isMouseButtonPressed(eMouseCode code)
		{
			if (mouseButtonMap_[code].currentState == eInputState::PRESSED || mouseButtonMap_[code].currentState == eInputState::HELD)
				return true;
			return false;
		}

		static bool isMouseButtonReleased(eMouseCode code)
		{
			if (mouseButtonMap_[code].currentState == eInputState::RELEASED)
				return true;
			if (mouseButtonMap_[code].currentState == eInputState::RELEASED)
			{
				mouseButtonMap_[code].previousState = mouseButtonMap_[code].currentState;
				mouseButtonMap_[code].currentState = eInputState::IDLE;
				return true;
			}
			return false;
		}

		static bool isMouseButtonDown(eMouseCode code)
		{
			if (mouseButtonMap_[code].currentState == eInputState::HELD)
				return false;

			if (mouseButtonMap_[code].previousState == eInputState::PRESSED && mouseButtonMap_[code].currentState == eInputState::PRESSED)
			{
				mouseButtonMap_[code].previousState = mouseButtonMap_[code].currentState;
				mouseButtonMap_[code].currentState = eInputState::HELD;
				return true;
			}

			if (mouseButtonMap_[code].currentState == eInputState::PRESSED)	
				return true;

			return false;
		}

		static bool isKeyPressed(eKeyCode code)
		{
			if (keyMap_[code].currentState == eInputState::PRESSED || keyMap_[code].currentState == eInputState::HELD)
				return true;
			return false;
		}

		static bool isKeyReleased(eKeyCode code)
		{
			//if (keyMap_[code].currentState == eInputState::RELEASED)
			//	return true;
			if (keyMap_[code].currentState == eInputState::RELEASED)
			{
				keyMap_[code].previousState = keyMap_[code].currentState;
				keyMap_[code].currentState = eInputState::IDLE;
				return true;
			}
			return false;
		}

		static bool isKeyDown(eKeyCode code)
		{

			if (keyMap_[code].currentState == eInputState::HELD)
				return true;
			if (keyMap_[code].currentState == eInputState::PRESSED)
			{
				keyMap_[code].previousState = keyMap_[code].currentState;
				keyMap_[code].currentState = eInputState::HELD;
				return true;
			}
			return false;
		}

		static eCursorBehaviour getCursorBehaviour() { return cursorBehaviour_; }
		static void setCursorBehaviour(eCursorBehaviour behaviour) { cursorBehaviour_ = behaviour; }

		static bool isCursorVisible()
		{
			return isCursorVisible_;
		}

		static void setCursorVisible(bool isVisible)
		{
			isCursorVisible_ = isVisible;
		}

	private:
		static Math::Vec2 mousePos_;
		static std::unordered_map<eKeyCode, InputInfo> keyMap_;
		static std::unordered_map<eMouseCode, InputInfo> mouseButtonMap_;

		static eCursorBehaviour cursorBehaviour_;
		static bool isCursorVisible_;
	};
}