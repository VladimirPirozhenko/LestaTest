#include "Input.h"
namespace LestaTest
{
	std::unordered_map<eKeyCode, InputInfo> Input::keyMap_;
	std::unordered_map<eMouseCode, InputInfo> Input::mouseButtonMap_;
	Math::Vec2 Input::mousePos_;

	eCursorBehaviour Input::cursorBehaviour_ = eCursorBehaviour::INVISIBLE_WHEN_PRESSED;
	bool Input::isCursorVisible_ = true;
}
